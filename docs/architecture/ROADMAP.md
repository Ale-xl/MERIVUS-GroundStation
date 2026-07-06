# MERIVUS 分支路线图

原则：一个分支只完成一个清晰目标；每个分支必须可单独构建、可回滚、可验收。未经用户确认，不进入下一阶段。

## 阶段 0：基线与架构

分支：`docs/project-audit`  
状态：本次执行

任务：

- 检查现有代码。
- 记录当前模块。
- 完成架构文档。
- 建立风险清单。
- 建立接口边界。
- 给出 MVP 定义。
- 不修改业务逻辑。

验收：

- 文档完整。
- 当前状态和目标状态清楚。
- 未验证内容明确标记。
- Release 增量构建通过。
- 在任何 RTK 接线或配置操作前，必须完成硬件版本、输入电压、串口电平、波特率和飞控接口确认。
- Codex 不得自动修改真实硬件参数。

## 前置热修：临时安全封锁

建议分支：`hotfix/safety-containment`

目标：仅临时收敛当前高风险入口，不建立完整安全框架。

任务：

- AI 面板中的 `takeoff`、`land`、`rtl`、`pause` 改为只显示建议，不调用 `Vehicle` 执行。
- 保留 UI 和聊天功能。
- 不删除已有代码，可通过明确的 feature flag 隔离。
- feature flag 默认关闭真实 AI 飞行动作。
- `SwarmController` 的 legacy MAVLink forwarding 增加开发开关，默认关闭。
- 临时任务上传后自动 `startMission` 增加开发开关，默认关闭。
- 原生 QGC 手动操作和 Link 功能不受影响。
- 不进行大范围重构。

验收：

- AI 输入飞行动作时只产生“未执行的建议”。
- 不调用 `guidedModeTakeoff`、`guidedModeLand`、`guidedModeRTL`、`pauseVehicle`。
- 未显式启用开发开关时，不发送 legacy forwarding 消息。
- 未显式启用开发开关时，不自动 `startMission`。
- Release 构建通过。
- 只允许 Mock 或 SITL 验证。
- 不进行真实飞机测试。

依赖：阶段 0。建议在阶段 1 前完成。

## 阶段 1：保护当前可用链路

建议分支：`test/current-link-baseline`

任务：

- 记录当前 TCP Link 配置方式。
- 将 `119.45.168.211` 和真实端口视为外部配置，不写死进业务代码。
- 不破坏原生 QGC Link 功能。
- 建立 Link 状态只读诊断。
- 增加连接、断开、重连和错误日志。
- 不自建服务器。
- 不改变真实指令发送方式。

验收：

- 原有 TCP 连接仍可使用。
- 未连接时不崩溃。
- 错误信息可读。
- IP 和端口没有散落硬编码。

依赖：阶段 0。

## 阶段 2：AI 界面模型整理

建议分支：`feat/ai-panel-foundation`

任务：

- 整理现有 AI 面板状态。
- 保持 MERIVUS 当前风格。
- 建立消息列表、发送、等待、错误和离线状态。
- 使用 Mock 回复。
- 暂不连接真实模型。
- 暂不执行飞行命令。

验收：

- QML 界面不卡顿。
- 无 Agent 时可显示离线。
- 不包含 API Key。
- 不修改 `Vehicle` 执行逻辑。

依赖：阶段 0。建议在阶段 1 之后做，避免先扩大 AI 风险面。

## 阶段 3：本机 Agent 最小链路

建议分支：`feat/local-agent-http`

任务：

- 建立独立 `agent/` 目录。
- 建立 Python/FastAPI 最小服务。
- 提供 `/health` 和 `/merivus/agent`。
- 第一版返回 Mock JSON。
- 定义请求和响应 JSON Schema。
- Agent 不访问 MAVLink、PX4 或真实云模型。

验收：

- 可手动启动 Agent。
- health 正常。
- POST 返回固定 JSON。
- 单元测试通过。

依赖：阶段 2 的契约收敛。

## 阶段 4：QGC 与 Agent 通信

建议分支：`feat/qgc-agent-client`

任务：

- 实现 C++ `AiAgentClient`。
- 使用 `QNetworkAccessManager` 异步请求。
- QML 不直接访问网络。
- 解析 `reply` 和 `proposal`。
- 增加请求 ID、session ID、超时、取消和错误状态。
- 只显示回复，不执行 proposal。

验收：

- QGC 能与 Mock Agent 通信。
- Agent 离线时飞控主功能正常。
- 不阻塞 UI。
- JSON 无效时安全拒绝。

依赖：阶段 3。

## 阶段 5：Agent 进程监管

建议分支：`feat/agent-supervisor`

任务：

- 实现 `AiServiceSupervisor`。
- 使用 `QProcess` 启动 Agent。
- Agent 位于 `agent/merivus-agent.exe`。
- 定期检查 `/health`。
- QGC 退出时关闭 Agent。
- 不使用开发机绝对路径。

验收：

- 双击地面站可启动 Agent。
- Agent 崩溃不影响 QGC。
- 安装路径变化后仍能找到 Agent。
- 日志写入用户可写目录。

依赖：阶段 3、4。

## 阶段 6：结构化意图与安全白名单

建议分支：`feat/ai-intent-policy`

任务：

- 定义 `ActionProposal`。
- 定义固定 command 枚举。
- 实现 Schema validator 和 `AiCommandPolicy`。
- 第一版只支持只读或 UI 操作。
- 未知命令一律拒绝。
- 风险级别由本地规则确定。

验收：

- 未知命令无法执行。
- 参数缺失时拒绝。
- 模型文本不能直接成为 MAVLink。
- 有审计记录。

依赖：阶段 4。

## 阶段 7：高风险命令确认框架

建议分支：`feat/command-confirmation`

任务：

- 建立 `AiCommandExecutor`。
- 建立确认弹窗。
- 建立执行前状态检查。
- 先使用 Mock Executor。
- 为 Hold、RTL、Land 预留接口。
- 自动解锁和自动起飞继续禁止。

验收：

- 无确认不执行。
- 取消后不执行。
- 飞行器断开时拒绝。
- 所有尝试有日志。

依赖：阶段 6。

## 阶段 8：RTK 状态集成

建议分支：`feat/rtk-status-integration`

任务：

- 在任何 RTK 接线或配置操作前，必须完成硬件版本、输入电压、串口电平、波特率和飞控接口确认。
- Codex 不得自动修改真实硬件参数。
- 读取 QGC 已有 GPS/RTK 状态。
- 展示 Fix 类型、卫星数、精度和航向状态。
- 不由 QGC 重新配置 RTK 硬件。
- Hyper982 配置只做文档和显示，不随意写入飞控。

验收：

- 无 RTK 时正常降级。
- RTK 与普通 GPS 状态区分。
- 不影响原生 GPS 逻辑。

依赖：阶段 1。

## 阶段 9：设备网关 POC

建议分支：`poc/device-gateway`

任务：

- 建立最小 TCP Server。
- 支持多个模拟设备连接。
- 使用模拟 MAVLink 或回放数据。
- 建立设备 ID、心跳、连接和断开日志。
- 不替换厂商服务器。
- 不开放未认证公网端口。

验收：

- 多个模拟设备可连接。
- 数据不串机。
- 断线可识别。
- 未认证连接被拒绝。

依赖：阶段 1。建议独立 `backend/` 或独立仓库。

## 阶段 10：云端基础

建议分支：`feat/cloud-foundation`

任务：

- 用户认证、组织、角色和权限。
- 设备注册和绑定。
- PostgreSQL 基础模型。
- 审计日志。

验收：

- 用户只能访问授权设备。
- 控制权限和查看权限分开。
- 数据库迁移可重复执行。

依赖：设备网关边界确认后再启动。

## 阶段 11：遥测记录与多机状态

建议分支：`feat/telemetry-pipeline`

任务：

- 定义统一遥测事件。
- 保存关键遥测和最新状态。
- 明确实时数据和历史数据边界。
- 不阻塞控制链路写数据库。

验收：

- 多机遥测不串线。
- 网络中断后状态可恢复。
- 可查询历史轨迹。

依赖：阶段 9、10。

## 阶段 12：视频链路

建议分支：`feat/video-pipeline`

任务：

- 记录 RTSP 地址配置方式。
- 视频端口配置化。
- 复用 QGC 原生视频能力。
- 控制链路和视频链路独立。
- 增加无视频、重连和超时状态。

验收：

- 视频断开时飞控链路正常。
- RTSP 地址不硬编码。
- 多机视频可绑定飞行器。

依赖：阶段 1。

## 阶段 13：GIS 安全服务

建议分支：`feat/gis-safety-service`

任务：

- 建立 `WaypointSafetyService` 接口。
- 第一版只做离线或服务端分析。
- 输出地形、建筑、禁飞区冲突。
- 不自动修改任务。

验收：

- 输入输出 Schema 固定。
- 数据缺失时明确告警。
- 未知区域不判定为安全。

依赖：阶段 6 的 proposal 契约。

## 阶段 14：模型 Provider

建议分支：`feat/agent-model-providers`

任务：

- Agent 实现 Provider router。
- 支持 Mock、本地模型、一个云 API。
- API Key 不写入代码。
- Provider 输出统一格式并经 Schema 校验。

验收：

- 无模型时返回清晰错误。
- 模型切换不修改 QML。
- 输出格式统一。

依赖：阶段 3、6。

## 阶段 15：Windows 发布打包

建议分支：`release/windows-packaging`

任务：

- 构建 `MerivusGroundControl.exe` 或确认现有 `MERIVUS.exe` 命名。
- 收集 Qt 运行库。
- 打包 `agent/merivus-agent.exe`。
- 配置和日志写入 AppData。
- 不把源码、虚拟环境、API Key、模型文件放入基础包。

验收：

- 无开发环境电脑可启动。
- 不依赖源码路径。
- Agent 可启动。
- AI 关闭时地面站正常使用。

依赖：阶段 5、14。

## 推荐 MVP

三个月内合理 MVP：

- 保持当前 QGC 基础飞控和 TCP Link 可用。
- Link 诊断和配置基线。
- AI 面板只读/Mock/建议模式。
- 本机 Mock Agent。
- QGC C++ Agent client。
- 本地 proposal schema 和只读/低风险白名单。
- RTK 状态显示。
- 视频 RTSP 配置化与失败降级。
- 文档化硬件接入和 Windows 发布路径。

暂缓内容：

- 自建完整云服务器替代厂商服务。
- 真实多机自动任务调度。
- AI 直接执行起飞、降落、返航。
- 机载电脑、ROS、YOLO、视觉避障。
- 商业计费、完整 Web Console、大规模遥测数据平台。
