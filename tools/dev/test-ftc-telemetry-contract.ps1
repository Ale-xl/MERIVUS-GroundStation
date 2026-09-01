$ErrorActionPreference = "Stop"

$projectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$firmwareRoot = Join-Path (Split-Path $projectRoot -Parent) "FirmwarePX4"
$coreSchema = Join-Path $projectRoot "schemas\mavlink\merivus_ftc.xml"
$backendHeader = Join-Path $projectRoot "src\Vehicle\VehicleFtcStatusFactGroup.h"
$backendSource = Join-Path $projectRoot "src\Vehicle\VehicleFtcStatusFactGroup.cc"
$panel = Join-Path $projectRoot "custom\res\Merivus\FtcStatusPanel.qml"
$overlay = Join-Path $projectRoot "custom\res\Merivus\CommandCenterOverlay.qml"

function Assert-Contains([string]$Path, [string]$Pattern, [string]$Message) {
    if (-not (Select-String -LiteralPath $Path -Pattern $Pattern -Quiet)) {
        throw $Message
    }
}

$xml = [xml](Get-Content -LiteralPath $coreSchema -Raw)
$expectedMessages = @{
    MERIVUS_FTC_MOTOR_STATUS = 60000
    MERIVUS_FTC_CONTROL_STATUS = 60001
    MERIVUS_FTC_EXTREME_STATUS = 60002
    MERIVUS_FTC_DIAGNOSTICS = 60003
}

foreach ($message in $xml.mavlink.messages.message) {
    if (-not $expectedMessages.ContainsKey([string]$message.name) -or
        [int]$message.id -ne $expectedMessages[[string]$message.name]) {
        throw "MAVLink 消息 ID 不符合契约：$($message.name)=$($message.id)"
    }

    if (-not ($message.field | Where-Object { $_.name -eq "protocol_version" })) {
        throw "$($message.name) 缺少 protocol_version"
    }
}

if ($xml.mavlink.messages.message.Count -ne $expectedMessages.Count) {
    throw "FTC 消息数量不符合契约"
}

$generatedExpectations = @{
    "mavlink_msg_merivus_ftc_motor_status.h" = @("LEN 78", "CRC 29")
    "mavlink_msg_merivus_ftc_control_status.h" = @("LEN 24", "CRC 153")
    "mavlink_msg_merivus_ftc_extreme_status.h" = @("LEN 38", "CRC 136")
    "mavlink_msg_merivus_ftc_diagnostics.h" = @("LEN 65", "CRC 5")
}

foreach ($entry in $generatedExpectations.GetEnumerator()) {
    $header = Join-Path $projectRoot "libs\mavlink\include\mavlink\v2.0\merivus_ftc\$($entry.Key)"
    foreach ($expectation in $entry.Value) {
        Assert-Contains $header ([regex]::Escape($expectation)) "$($entry.Key) 缺少 $expectation"
    }
}

foreach ($property in @(
    "enabled", "available", "lastUpdate", "stale", "systemState", "motorCount",
    "modelQuality", "rollAuthority", "pitchAuthority", "yawAuthority", "thrustAuthority",
    "minimumAttitudeAuthority", "actuatorHeadroom", "impactType", "locState", "recoveryState"
)) {
    Assert-Contains $backendHeader ("Q_PROPERTY\([^\r\n]*\b" + $property + "\b") "后端缺少属性 $property"
}

foreach ($stateText in @(
    "关闭", "监测", "检测到强扰动", "角速度抑制", "推力方向恢复", "姿态恢复",
    "高度稳定", "恢复正常控制", "紧急降落", "已中止", "恢复失败"
)) {
    Assert-Contains $backendSource ([regex]::Escape($stateText)) "恢复状态缺少中文映射：$stateText"
}

Assert-Contains $panel "vehicle\.ftcStatus" "FTC 面板没有使用 Vehicle 类型化后端"
Assert-Contains $overlay "ftcStatus\.motors\.get" "电机卡片没有接入 FTC 电机模型"

if (Select-String -LiteralPath $panel, $overlay -Pattern "mavlink_msg_|MAVLINK_MSG_ID_" -Quiet) {
    throw "QML 不得解析原始 MAVLink"
}

if (Test-Path $firmwareRoot) {
    $firmwareCore = Join-Path $firmwareRoot "src\modules\mavlink\message_definitions\v1.0\merivus_ftc.xml"
    if ((Get-FileHash $firmwareCore -Algorithm SHA256).Hash -ne (Get-FileHash $coreSchema -Algorithm SHA256).Hash) {
        throw "固件与地面站 FTC XML 不一致"
    }

    $controlStream = Join-Path $firmwareRoot "src\modules\mavlink\streams\MERIVUS_FTC_CONTROL_STATUS.hpp"
    if (Select-String -LiteralPath $controlStream -Pattern "MERIVUS_FTC_CONTROL_(MODE_ACTIVE|FLAGS_ACTIVE_COMMAND_PATH)" -Quiet) {
        throw "当前固件不得报告 ACTIVE 或 ACTIVE_COMMAND_PATH"
    }
}

Write-Host "FTC 遥测字段、后端属性、QML 消费与安全语义检查通过。"
