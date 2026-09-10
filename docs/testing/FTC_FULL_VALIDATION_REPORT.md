# FTC 本轮地面站验证与收尾

2026-09-10 按用户要求收尾。完整跨仓报告位于 `E:/MERIVUS/FirmwarePX4/docs/testing/FTC_FULL_VALIDATION_REPORT.md`。

生产修复提交 `31638ea`，测试探针提交 `5a1931af64704487a8fc4eae03d93c679e794310`。Windows Release 已在生产修复点构建通过，后续只修改测试及文档，未重复完整构建。应用 SHA-256 为 `8b7cfe912342c079a20d929b06b94279a8e45d3ead1cd785ce79eb90355d88cb`。

## 修改

`src/Vehicle/VehicleFtcStatusFactGroup.cc/.h` 修复了两项实际问题：一次不兼容消息不再永久锁死协议状态，各类 stream 独立检查，其他正常 stream 也不能掩盖不兼容；电机模型过期时通知 H/E、sigma、age、有效性和状态文本的变化。

`test/FTC/` 直接编译实际生产后端，独立 FactGroup 桩只隔离应用注册表。8 个 Qt Test 结果项全部通过，包含 6 个测试方法和初始化/清理。修复前两个协议恢复测试失败，修复后通过。此结果不等同于完整 Vehicle、QML 或飞行控制验证。

## 真实连接与停流

最新 MERIVUS.exe 已启动。隔离 Ubuntu SITL 经 SSH 双向桥接向实际地面站送入 MAVLink，Windows 可访问性树观察到 UAV-1。同一真实数据流送入独立后端探针，不生成虚假 FTC 状态。

探针累计解码 26092 包、856 个心跳、0 CRC 错误。约 63.496 s 进入 STALE，约 93.996 s 恢复实时状态；暂停 FTC 的部分窗口中，心跳计数从 188 增至 278，证明过期并非单纯由整个连接中断造成。桥接还转发了地面站向 SITL 的 296 个数据报。

停止后的 H/E 为 N/A；恢复后无效模型显示“模型不可用”或“估计质量不足”，不混同“消息已过期”。真实飞行模型仍未通过最终准入门，未执行 Active。

## 视觉边界

`computer-use` 截图接口报 `SetIsBorderRequired 0x80004002`，点击接口报 `coordinate input geometry is unavailable`。能读取窗口和 UAV-1 的可访问性信息，但不能据此确认 FTC 详情面板的最终视觉样式。视觉复核仍未完成。

## 证据与 Git

本地证据位于 `E:/MERIVUS-ftc-full-validation-20260909/groundstation/`：

- `backend/before.txt`、`after.txt`、`live-probe-regression.txt`：最小失败及回归。
- `release-build.log`：实际 Release 构建。
- `live-backend-08.jsonl`、`live-backend-08-summary.json`：真实后端状态与停流恢复。
- `bridge-08/mavlink.bin`、`result.json`：收发记录。

分支 `codex/ftc-full-validation`，测试前 tag `archive/ftc-full-validation-pre-20260909`。按逻辑提交，未 push、未 merge，未操作真实飞机。旧阶段报告保留为历史证据，本轮状态以本文为准。
