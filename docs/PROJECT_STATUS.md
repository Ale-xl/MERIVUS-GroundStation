# 项目状态

更新日期：2026-09-01

## 当前基线

- 仓库：`MERIVUS-GroundStation`
- 上游基础：QGroundControl Custom Build
- 开发分支：`research/groundstation-ftc-ui`
- 开始本轮工作前的提交：`8ea4380a472e08ca7bf03c7bc8bd82eddcf7eee0`
- 本地保护分支：`backup/groundstation-current-20260901`
- 本地保护标签：`archive/groundstation-current-20260901`

## 已落地

- 主界面、多机列表、人工 Guided/Swarm 确认链路和视频区域。
- 本机 Agent、结构化建议与 QGC 侧安全策略；AI 仍不能直接控制飞行器。
- ESC 基础遥测：`ESC_INFO`、`ESC_STATUS` 到 `VehicleEscStatusFactGroup` 和电机卡片。
- MERIVUS FTC MAVLink 方言：4 个版本化消息，消息 ID 为 `60000` 至 `60003`。
- FTC 后端：`VehicleFtcStatusFactGroup` 负责解码、过期判定、状态文本和电机列表模型。
- Fly View FTC 面板：系统状态、模式、控制裕度、撞击/失控/恢复摘要及详情。
- GitNexus 本地索引已刷新到本轮提交：127,409 个节点、175,295 条边；数据库留在被忽略的 `.gitnexus/`。

## 尚未完成

- 本轮尚未完成 Windows 全量编译和真实飞控联调；当前机器没有 Qt 5.15.2/MSVC 2019 构建工具链。
- FTC 遥测没有目标硬件带宽实测、断链复测和长时间稳定性数据。
- 当前固件只发送观测、影子分配和恢复候选数据。没有 FTC 执行器命令路径，不能显示成主动容错控制已经接管。
- UI 首版重点覆盖四电机紧凑卡片；详情模型可显示最多 12 个电机，更多机型仍需实机布局验证。

## 当前验证结论

- 两端核心 XML SHA-256 相同：`0ae936460c2489f33cebaaba018b2532dd258486d50d1fd835dbc91fc6fc064d`。
- 锁定的 `pymavlink` 生成器可通过严格单位校验并复现头文件。
- 字段—后端属性—QML 消费闭环检查通过。
- 完整构建、SITL 运行和真机飞行验证仍是发布前置条件，不由静态检查替代。
