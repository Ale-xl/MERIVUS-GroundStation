# FTC 地面站阶段总结（2026-09-09）

分支 `codex/ftc-software-closure`；验证代码提交 `20d36a9601f116caefc88e284d456efdf4f48d67`。后续提交仅整理文档。

已同步 MAVLink v2、模型状态与消息超时区分、实际 ACTIVE 反馈、H/E 有效性门和详情诊断。改动集中在 `src/Vehicle/VehicleFtcStatusFactGroup.cc/.h`、`custom/res/Merivus/FtcStatusPanel.qml`、`CommandCenterOverlay.qml`、核心 XML、生成头与 `tools/dev/test-ftc-telemetry-contract.ps1`。

Release 构建、遥测消费静态检查、锁定 mavgen 生成一致性检查均 PASS。Qt 5.15.2，VS2022；PDB 缺失链接警告不影响本次 Release 完成。产物 `build/ftc-closure-release/staging/MERIVUS.exe`，SHA-256 `0aa3590fe79f49f3eead63323f9e012231974c1d1a04c69844a4771b2bc1b01e`。没有替换运行中的地面站。

新协议实际消息链路、UI 回放、断链恢复、多机型布局和飞行测试未执行。两种主动能力默认关闭，状态仍为 IMPLEMENTED_UNVERIFIED。GitNexus 已尝试，图不完整按 PARTIAL/UNKNOWN 记录；不把零影响计数当作已证明安全。

完整 A–S 报告与源文件清单见同工作区 [Firmware 阶段总结](../../../FirmwarePX4/docs/testing/FTC_SOFTWARE_STAGE_REPORT.md)。用户要求阶段收口，本轮在上述证据形成后停止，无 push/merge/部署。
