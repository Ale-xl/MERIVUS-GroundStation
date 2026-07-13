# MERIVUS Agent 模型 Provider

本阶段分支：`feat/agent-model-providers`。

本阶段只在 Python Local Agent 中增加本地模型 Provider 能力：`MockProvider`、`OllamaProvider` 和 `ProviderRouter`。不接 DeepSeek、OpenAI、Gemini、MCP、云端服务、数据库，也不增加任何飞行动作执行链路。

## 为什么先接 Ollama

Ollama 运行在本机 `127.0.0.1`，可以在不引入云端 API Key、不暴露外部网络依赖、不上传飞行上下文的前提下验证真实模型交互。它适合作为 MERIVUS Local Agent 的第一类真实模型 Provider。

本阶段 `external_network_enabled=false`，表示 MERIVUS Agent 自身不访问外网；Ollama 是否已经安装模型由用户本机环境负责，Agent 不自动安装、不自动 pull 模型。

## 为什么使用 qwen3:8b

用户已经在本机完成 `qwen3:8b` 下载和运行验证。该模型体量适合本地联调，能覆盖普通问答、状态解释、日志解释和结构化建议生成。默认模型固定为 `qwen3:8b`，后续如需扩展模型列表，应先继续完善本地安全策略和评估流程。

## Provider 结构

Python Agent Provider 位于：

- `agent/app/providers/base.py`：统一接口、Provider health/info 数据结构、ProviderError。
- `agent/app/providers/mock.py`：保留原有 Mock 行为，并实现 `health()`。
- `agent/app/providers/ollama.py`：仅调用本机 Ollama HTTP API。
- `agent/app/providers/router.py`：根据配置选择 Provider，并控制显式 fallback。
- `agent/app/providers/system_prompt.py`：本地模型系统提示词与 JSON Schema 约束。

统一接口：

- `generate(request)`：返回 `reply` 和可选 `proposal`，不执行动作。
- `health()`：返回 Provider 是否 ready、错误和可用模型。
- `info()`：返回 `/merivus/info` 需要展示的 Provider 信息。

## 配置项

`.env.example` 新增：

```text
MERIVUS_AGENT_PROVIDER=mock
MERIVUS_OLLAMA_BASE_URL=http://127.0.0.1:11434
MERIVUS_OLLAMA_MODEL=qwen3:8b
MERIVUS_OLLAMA_TIMEOUT_SECONDS=60
MERIVUS_AGENT_ALLOW_MOCK_FALLBACK=false
```

默认仍使用 `mock`。切换本地模型时显式设置 `MERIVUS_AGENT_PROVIDER=ollama`。

## Ollama HTTP 接口

`OllamaProvider` 使用：

- `GET /api/tags`：检查 Ollama 服务是否可用，并确认 `qwen3:8b` 是否已安装。
- `POST /api/chat`：调用模型生成回复，设置 `stream=false`、`think=false`，并传入 JSON Schema 格式约束。

错误处理覆盖：服务不可用、请求超时、HTTP 500、模型未安装、Ollama 响应非法 JSON、模型输出非法 JSON、缺少 `reply`、`arguments` 类型错误、模型输出越权字段等。

## 结构化输出规则

模型只允许输出：

```json
{"reply":"string","proposal":null}
```

或：

```json
{"reply":"string","proposal":{"command":"string","arguments":{},"summary":"string"}}
```

Agent 层会拒绝包含这些字段的模型输出：

- `executed`
- `executable`
- `risk`
- `localRisk`
- `policyDecision`
- `requiresConfirmation`
- `mavlink`
- `shell`
- `script`
- `px4_parameters`

QGC C++ 的 `AiSchemaValidator` 和 `AiCommandPolicy` 仍是最终安全边界。即使 Agent 输出通过，QGC 仍会重新校验 schema、重算本地风险、策略决策和 `executable=false`。

## 系统提示词

位置：`agent/app/providers/system_prompt.py`。

提示词明确：MERIVUS 助手只能回答问题和生成结构化建议；不能执行飞行动作；不能发送 MAVLink；不能修改 PX4 参数；不能声称已经执行；只能使用请求 `context` 中的信息；缺失信息必须说明未知；输出必须符合 `reply/proposal` JSON 结构。

## fallback 策略

默认 `MERIVUS_AGENT_ALLOW_MOCK_FALLBACK=false`，Provider 失败不会静默回退。

只有显式设置 `MERIVUS_AGENT_ALLOW_MOCK_FALLBACK=true`，且当前 Provider 不是 mock 时，`ProviderRouter` 才会在 Provider 生成失败后回退到 MockProvider。未知 Provider 仍返回清晰错误，不作为静默 fallback 处理。

## /merivus/info 变化

`/merivus/info` 新增：

- `provider_ready`
- `provider_error`
- `available_models`
- `external_network_enabled`
- `flight_execution_enabled`

Ollama 本地模型模式下：

```json
{
  "provider": "ollama",
  "model": "qwen3:8b",
  "provider_ready": true,
  "provider_error": null,
  "available_models": ["qwen3:8b"],
  "external_network_enabled": false,
  "flight_execution_enabled": false
}
```

Ollama 未启动时：`provider_ready=false`，`provider_error="Ollama service is not available"`。

模型未安装时：`provider_ready=false`，`provider_error="Model qwen3:8b is not installed"`。

## QGC 显示变化

QGC 只增加显示字段：Provider Ready、Provider Error、Models。没有新增飞行动作执行按钮，没有绕过 ActionProposal，没有修改 Vehicle/MAVLink/Swarm/PX4，也没有恢复 QML `XMLHttpRequest`。

## 打包边界

PyInstaller spec 增加 `httpx/httpcore` 收集，用于本机 Ollama HTTP 调用。不打包 Ollama 程序，不打包 `qwen3:8b` 模型文件，不提交模型权重。

## 安全结论

本阶段没有真实飞行动作执行。Agent 不访问 MAVLink、PX4、Vehicle 或 SwarmController。所有 proposal 仍由 QGC 本地 C++ 策略层重新计算，当前阶段保持 `executable=false`。
