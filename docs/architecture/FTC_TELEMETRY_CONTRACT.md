# FTC 遥测契约

## 版本与来源

- 契约版本：1
- MAVLink 传输：MAVLink 2
- 核心定义：`schemas/mavlink/merivus_ftc.xml`
- GroundStation 包装方言：`schemas/mavlink/merivus.xml`，继承 `all.xml`
- FirmwarePX4 包装方言：继承 `development.xml`
- 核心 XML SHA-256：`0ae936460c2489f33cebaaba018b2532dd258486d50d1fd835dbc91fc6fc064d`
- 生成器来源：FirmwarePX4 锁定的 MAVLink 子模块提交 `18955a04c7c7467e00ea42b704addb4a9c12b53a`

`common.xml` 未修改。GroundStation 和固件使用不同包装方言，但共享逐字节相同的 FTC 核心定义。

## 消息

| ID | 名称 | 负载 | CRC | 默认频率 | 无签名流量 |
| ---: | --- | ---: | ---: | ---: | ---: |
| 60000 | `MERIVUS_FTC_MOTOR_STATUS` | 78 B | 29 | 5 Hz | 450 B/s |
| 60001 | `MERIVUS_FTC_CONTROL_STATUS` | 24 B | 153 | 5 Hz | 180 B/s |
| 60002 | `MERIVUS_FTC_EXTREME_STATUS` | 38 B | 136 | 10 Hz | 500 B/s |
| 60003 | `MERIVUS_FTC_DIAGNOSTICS` | 65 B | 5 | 1 Hz | 77 B/s |

MAVLink 2 未签名帧按每帧 12 B 开销计算，总计约 1,207 B/s。启用 13 B 签名块后约 1,480 B/s。FMUv6C 的 TELEM1 基线为 57,600 baud；8N1 物理上限约 5,760 B/s，`MAV_0_RATE=0` 的标称发送预算约 2,880 B/s。FTC 流约占未签名预算的 42%，仍要与心跳、姿态、位置等消息共享。PX4 调度器可以降频，四条流没有设置常量速率。

## 百分比编码

`uint8_t` 的 `0..200` 表示 `0..100%`，每个计数为 `0.5%`。`255` 表示不可用。发送端先限制到 `0..1`，再编码；GroundStation 解码到 `0..100`。`201..254` 在版本 1 中保留，不应生成。

## 消息职责

### 电机状态 60000

最多 12 个电机。包含健康、效能、故障概率、置信度、故障类型、降级/失败掩码、监测状态和模型质量。健康是当前观测分数，不能称为剩余寿命。

电机监测状态不是 `VALID` 时，各电机百分比发送 `255`。GroundStation 因而显示 `N/A`，不会把标定中或无效观测的默认值显示成 0%。

### 控制状态 60001

包含滚转、俯仰、偏航、推力、最小姿态裕度、执行器余量、饱和掩码、恢复进度和控制集成阶段。

控制阶段只有以下含义：

| 模式 | 含义 |
| --- | --- |
| `DISABLED` | 监测关闭 |
| `OBSERVE` | 只观测，不计算有效影子分配 |
| `SHADOW` | 计算影子分配，不下发执行器 |
| `CANDIDATE` | 产生恢复候选，不下发执行器 |
| `ACTIVE` | 已有明确执行器命令路径；当前固件不具备 |

版本 1 的发送实现不得设置 `ACTIVE_COMMAND_PATH`，不得报告 `ACTIVE`。

### 极端状态 60002

包含撞击类型、撞击分数/置信度/严重度、失控状态与原因掩码，以及恢复触发、抑制和候选进度。`recovery.active` 只表示候选状态机处于活动阶段，不等于执行器命令路径已接管。

### 诊断 60003

低频输出模型残差、激励、机动强度、外扰、振动、影子分配残差、姿态/角速度误差和 SITL 注入状态。主操作界面不依赖该消息维持基本状态。

## 恢复状态中文映射

| 枚举 | 界面文本 |
| --- | --- |
| `DISABLED` | 关闭 |
| `MONITORING` | 监测 |
| `DISTURBANCE_DETECTED` | 检测到强扰动 |
| `RATE_DAMPING` | 角速度抑制 |
| `THRUST_VECTOR_RECOVERY` | 推力方向恢复 |
| `ATTITUDE_RECOVERY` | 姿态恢复 |
| `ALTITUDE_STABILIZATION` | 高度稳定 |
| `CONTROL_REENTRY` | 恢复正常控制 |
| `EMERGENCY_LAND` | 紧急降落 |
| `ABORTED` | 已中止 |
| `FAILED` | 恢复失败 |

## 过期与兼容

- 每条消息都带 `protocol_version=1`。
- GroundStation 对每个消息族使用 3 秒超时。过期后显示 `N/A`，不把旧值当成当前状态。
- 没有 FTC 消息的飞控保持兼容：`vehicle.ftcStatus.available=false`，标准飞行和 ESC UI 不受影响。
- 版本不匹配时停止解释业务字段，显示明确的协议不兼容提示。
- 版本 1 内只能在 MAVLink `<extensions/>` 后追加可选字段；改变现有字段类型、缩放、枚举值或语义必须升级协议版本，并评估是否需要新消息 ID。

## 变更流程

1. 同步修改两仓核心 XML并核对 SHA-256。
2. 用锁定生成器执行严格单位校验。
3. 重新生成 GroundStation 头文件。
4. 更新发送端、后端、QML 和本文档。
5. 运行两仓契约检查、SITL 收发和断流过期测试。
