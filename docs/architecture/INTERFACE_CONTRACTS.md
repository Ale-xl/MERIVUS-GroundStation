# MERIVUS 接口契约草案

本文档定义阶段 0 的接口边界。除“从代码中确认”外，均为草案，需要在对应分支中实现和测试。

## QGC 到 Agent

### Endpoint

```text
POST http://127.0.0.1:8765/merivus/agent
Content-Type: application/json
```

### 请求草案

```json
{
  "request_id": "uuid",
  "session_id": "session-id",
  "message": "用户输入",
  "context": {
    "active_vehicle_id": 1,
    "connected": true,
    "armed": false,
    "flight_mode": "Position",
    "battery_percent": 80,
    "gps_valid": true,
    "rtk_fix_type": "none"
  },
  "allowed_capabilities": [
    "vehicle.query_status",
    "mission.analyze"
  ]
}
```

### 响应草案

```json
{
  "request_id": "uuid",
  "reply": "文本回答",
  "proposal": {
    "command": "vehicle.query_status",
    "arguments": {
      "vehicle_id": 1
    },
    "risk": "low",
    "requires_confirmation": false,
    "summary": "查询一号无人机状态"
  }
}
```

### 契约规则

- `proposal` 可以为 `null`。
- QGC 不信任 Agent 返回的 `risk`，必须本地重算。
- `command` 必须来自固定枚举。
- 未知字段默认拒绝，JSON Schema 中尽量设置 `additionalProperties: false`。
- 不允许返回原始 MAVLink 参数数组作为通用执行接口。
- Agent 超时、离线、返回无效 JSON 时，QGC 只显示错误，不影响飞控主功能。

## 第一批允许 command

低风险或只读：

- `vehicle.query_status`
- `vehicle.query_battery`
- `vehicle.query_position`
- `vehicle.query_rtk`
- `log.explain_error`

UI 操作：

- `ui.select_vehicle`
- `ui.open_page`
- `map.focus_coordinate`

任务草稿与分析：

- `mission.create_draft`
- `mission.analyze`

## 第一版禁止 command

- `vehicle.arm`
- `vehicle.force_arm`
- `vehicle.takeoff`
- `vehicle.land`
- `vehicle.rtl`
- `vehicle.pause`
- `vehicle.goto`
- `mission.upload`
- `mission.start`
- `param.write`
- `mavlink.send_raw`

说明：这些可以作为未来高风险能力预留，但阶段 0/第一版 AI 不应直接实现执行。

## QGC 本地策略接口

建议后续在 C++ 中实现：

- `AiAgentClient`
  - 负责 HTTP 请求、超时、取消、错误状态、JSON 解析。
- `ActionProposal`
  - 固定字段：`command`、`arguments`、`summary`、`source`、`request_id`。
- `SchemaValidator`
  - 负责请求/响应 schema 校验。
- `AiCommandPolicy`
  - 根据本地规则重算风险和是否需要确认。
- `AiCommandExecutor`
  - 第一版仅 mock 或只读/UI 操作。
  - 高风险命令后续必须接确认弹窗、前置检查和审计。

## QGC 到 Device Gateway

当前不建议第一阶段实现自建网关。未来草案：

- TCP MAVLink 设备入口：只允许认证设备接入。
- QGC 订阅入口：建议 WebSocket 或受控 TCP relay。
- 每个设备必须有稳定 `device_id`、`vehicle_id`、`sim_or_real` 标记。
- 必须区分遥测下行、命令上行、视频链路和管理 API。

## QGC 到视频服务

当前复用 QGC 视频设置和接收能力。未来草案：

- 输入：`vehicle_id`、`stream_url`、`protocol`、`timeout_ms`。
- 输出：连接状态、最后帧时间、错误码、重连状态。
- 视频失败不得影响 MAVLink Link。

## QGC 到 GIS Safety Service

未来草案：

```json
{
  "request_id": "uuid",
  "mission_draft_id": "draft-id",
  "waypoints": [
    { "lat": 0.0, "lon": 0.0, "alt_m": 80.0, "frame": "relative" }
  ],
  "checks": ["terrain", "building", "nofly_zone"],
  "unknown_area_policy": "warn"
}
```

响应只生成报告和建议，不自动修改任务：

```json
{
  "request_id": "uuid",
  "status": "warning",
  "findings": [
    {
      "severity": "warning",
      "type": "unknown_area",
      "message": "缺少该区域的建筑物计算数据"
    }
  ],
  "suggestions": []
}
```

## 从代码中确认

- 当前 AI 面板实际请求仍为 QML `XMLHttpRequest`，字段为 `message`、`model`、`fleet`、`history`。
- 当前 AI 面板实际响应字段为 `reply`、`intent`，其中 `intent.action` 允许 `takeoff`、`land`、`rtl`、`pause`。
- 当前实现与本文推荐的 `request_id/session_id/context/allowed_capabilities/proposal` 契约不一致，需要阶段 3/4/6 逐步迁移。

## 待确认事项

- 是否保留兼容旧 `intent` 字段一段时间。
- Agent 本机端口是否固定为 `8765`，还是允许用户配置。
- 是否为本机 Agent 增加简单 token 或进程级来源校验。
- 多机场景下 `vehicle_id` 与 PX4 `sysid` 的映射规则。
