# MERIVUS 无人机多机调度系统

MERIVUS 是基于 QGroundControl、PX4、4G/TCP 链路、RTK 定位和本机 AI Agent 构建的无人机多机调度系统。当前仓库以 `MerivusGroundControl` 为核心，保留 QGC 上游工程结构，并在 `custom/`、`agent/`、`docs/`、`schemas/` 和 `configs/` 中沉淀 MERIVUS 的定制能力。

## 当前安全状态

当前版本没有 AI 真实飞行动作执行能力。LLM/Agent 不能直接控制无人机，不能发送 MAVLink，不能修改 PX4 参数，也不能绕过 QGC 原生安全流程。

AI 只允许返回自然语言回答和可选 `ActionProposal`。所有 proposal 当前都由 QGC C++ 本地策略重新校验，最终 `executable=false`，仅用于展示和预览。QGC C++ 的 `AiSchemaValidator` 与 `AiCommandPolicy` 是当前 AI 链路的安全边界。

不要向仓库提交 `.env`、Token、API Key、厂商 PDF、DOCX、Excel、截图、Ollama 模型、模型权重、真实 RTSP 凭据、物联网卡敏感信息、build/dist/staging 产物或本地日志。

## 已实现能力

- MERIVUS QGC Custom Build 主界面与多机调度 UI。
- 本机 FastAPI Local Agent，提供 `/health`、`/merivus/info`、`/merivus/agent`。
- `AiServiceSupervisor`，由 QGC 启动和守护本机 Agent。
- `AiAgentClient`，由 QGC C++ 异步访问本机 Agent，QML 不直接发网络请求。
- `MockProvider` 与 `OllamaProvider`，默认本地模型为 `qwen3:8b`。
- QGC AI 面板 Provider 选择、Provider Ready/Error/Models 显示。
- `ActionProposal`、schema 校验、本地风险和策略判定。
- 问答/指令分离：解释类问题优先回答，不显示未执行建议卡片。
- Agent Windows Release 打包 POC。

## 系统组成

- `MerivusGroundControl`：基于 QGC 的地面站客户端。
- `Merivus Local Agent`：本机 AI/规则服务，当前只运行在 `127.0.0.1`。
- `Device Gateway`：后续设备接入与多设备链路网关。
- `Cloud API`：后续账号、设备、审计、任务和遥测服务。
- `Web Console`：后续运营与管理控制台。
- `GIS Safety Service`：后续离线或服务端 GIS 安全分析。
- `Media Service`：后续视频链路、转码、录像与回放服务。
- `PX4 Flight Stack`：飞控固件与真实执行端，MERIVUS 不替代飞控安全逻辑。

## AI 链路边界

```text
用户自然语言
  -> QGC AI 面板
  -> Merivus Local Agent
  -> reply / ActionProposal
  -> QGC C++ Schema Validator
  -> QGC C++ Local Policy
  -> 仅展示，不执行
```

当前不会进入：

```text
ActionProposal -> Vehicle / MAVLink / Swarm / PX4
```

后续如果进入命令确认或执行器阶段，必须先完成独立设计、审计、仿真验证和用户确认，不能从 LLM 输出直接接入真实飞控链路。

## 仓库结构

- `custom/`：MERIVUS QGC Custom Build 入口、QML、资源和 C++ 扩展。
- `agent/`：Merivus Local Agent、Provider、schema、测试和打包 spec。
- `docs/`：架构、开发、硬件、流程和安全边界文档。
- `schemas/`：跨语言 JSON Schema 契约草案。
- `configs/`：配置模板和策略示例。
- `tools/dev/`：Windows 构建、Agent 打包和局部测试脚本。
- `src/`、`libs/`、`resources/`：QGroundControl 上游主体代码和资源。

更多说明见 [docs/INDEX.md](docs/INDEX.md)。

## 快速验证

Agent 单元测试：

```powershell
cd agent
python -m pytest
```

QGC AI 策略测试：

```powershell
powershell -ExecutionPolicy Bypass -File tools/dev/test-ai-intent-policy.ps1
```

Agent Release 打包：

```powershell
powershell -ExecutionPolicy Bypass -File tools/dev/build-agent.ps1 -Configuration Release
```

MERIVUS Release 构建：

```powershell
powershell -ExecutionPolicy Bypass -File tools/dev/build-merivus.ps1 -Configuration Release
```

本机真实模型评估必须显式 opt-in：

```powershell
cd agent
python tools/run_model_eval.py --run-real-model
```

## GitHub 提交卫生

提交前至少检查：

```powershell
git status --short
git status --ignored --short
git diff --check
```

仓库应只提交源码、文档、schema、示例配置和必要资源；本地构建产物、模型、密钥、真实设备凭据和厂商手册必须留在 Git 外。

## 许可证与上游

MERIVUS 基于开源 QGroundControl 二次开发，并保留上游项目和第三方依赖的许可证约束。相关规则见 [COPYING.md](COPYING.md) 与源文件中的许可证声明。
