import QtQuick          2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts  1.11
import QGroundControl.Controls    1.0
import QGroundControl.Palette     1.0
import QGroundControl.ScreenTools 1.0

Rectangle {
    id: root

    property var vehicle
    property var statusPalette: localPalette
    readonly property var ftc: vehicle ? vehicle.ftcStatus : null
    readonly property bool fresh: !!ftc && ftc.available && !ftc.stale && ftc.protocolCompatible
    readonly property bool systemFresh: fresh && ((ftc.motorAvailable && !ftc.motorStale) || (ftc.controlAvailable && !ftc.controlStale))
    readonly property bool controlFresh: fresh && ftc.controlAvailable && !ftc.controlStale
    readonly property bool extremeFresh: fresh && ftc.extremeAvailable && !ftc.extremeStale

    radius: 5
    color: qgcPal.windowShade
    border.color: statusPalette.colorFor(systemFresh ? ftc.systemSeverity : "unavailable")
    implicitHeight: compactColumn.implicitHeight + 12

    QGCPalette { id: qgcPal; colorGroupEnabled: true }
    FtcStatusPalette { id: localPalette }

    function percentText(value) {
        return value >= 0 ? Number(value).toFixed(0) + "%" : "--"
    }

    function statusText() {
        if (!vehicle) return qsTr("未选择飞行器")
        if (!ftc) return qsTr("当前版本没有 FTC 后端")
        if (ftc.protocolVersion > 0 && !ftc.protocolCompatible) return qsTr("协议版本不兼容：v%1").arg(ftc.protocolVersion)
        if (!ftc.available) return qsTr("未收到 FTC 遥测")
        if (ftc.stale) return qsTr("FTC 数据已过期")
        if (!systemFresh) return qsTr("FTC 系统状态：N/A")
        return ftc.systemStateText
    }

    ColumnLayout {
        id: compactColumn
        anchors.fill: parent
        anchors.margins: 6
        spacing: 4

        RowLayout {
            Layout.fillWidth: true
            QGCLabel {
                Layout.fillWidth: true
                text: qsTr("FTC 状态")
                color: qgcPal.text
                font.bold: true
                font.pointSize: ScreenTools.smallFontPointSize
            }
            Rectangle {
                Layout.preferredWidth: modeLabel.implicitWidth + 12
                Layout.preferredHeight: 22
                radius: 6
                color: Qt.rgba(statusPalette.colorFor(root.controlFresh ? root.ftc.controlModeSeverity : "unavailable").r,
                               statusPalette.colorFor(root.controlFresh ? root.ftc.controlModeSeverity : "unavailable").g,
                               statusPalette.colorFor(root.controlFresh ? root.ftc.controlModeSeverity : "unavailable").b, 0.16)
                border.color: statusPalette.colorFor(root.controlFresh ? root.ftc.controlModeSeverity : "unavailable")
                QGCLabel {
                    id: modeLabel
                    anchors.centerIn: parent
                    text: root.controlFresh ? root.ftc.controlModeText : qsTr("N/A")
                    color: qgcPal.text
                    font.pointSize: ScreenTools.smallFontPointSize
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            QGCLabel {
                Layout.fillWidth: true
                text: root.statusText()
                color: statusPalette.colorFor(root.systemFresh ? root.ftc.systemSeverity : "unavailable")
                font.bold: true
            }
            QGCLabel {
                text: qsTr("最小姿态裕度 %1").arg(root.controlFresh ? root.percentText(root.ftc.minimumAttitudeAuthority) : "--")
                color: qgcPal.text
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            QGCLabel {
                Layout.fillWidth: true
                text: root.extremeFresh ? qsTr("失控：%1 · 恢复：%2").arg(root.ftc.locStateText).arg(root.ftc.recoveryStateText)
                                 : qsTr("失控与恢复状态：N/A")
                color: root.extremeFresh ? statusPalette.colorFor(root.ftc.locSeverity) : qgcPal.colorGrey
                elide: Text.ElideRight
            }
            QGCButton {
                text: qsTr("详情")
                enabled: !!root.ftc && root.ftc.available && root.ftc.protocolCompatible
                onClicked: detailsPopup.open()
            }
        }

        QGCLabel {
            Layout.fillWidth: true
            visible: root.controlFresh && root.ftc.controlMode !== 0
            text: root.ftc.controlMode === 4
                  ? qsTr("ACTIVE 表示执行器命令路径已明确接入。")
                  : qsTr("当前仅观测、影子计算或候选恢复，不向执行器下发 FTC 命令。")
            color: qgcPal.colorGrey
            wrapMode: Text.WordWrap
            font.pointSize: ScreenTools.smallFontPointSize
        }
    }

    Popup {
        id: detailsPopup
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        width: Math.min(parent.width * 0.82, 760)
        height: Math.min(parent.height * 0.86, 620)
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: qgcPal.window
            radius: 8
            border.color: root.statusPalette.colorFor(root.systemFresh ? root.ftc.systemSeverity : "unavailable")
        }

        contentItem: ColumnLayout {
            spacing: 8

            RowLayout {
                Layout.fillWidth: true
                QGCLabel { Layout.fillWidth: true; text: qsTr("FTC 遥测详情"); font.bold: true; font.pointSize: ScreenTools.mediumFontPointSize; color: qgcPal.text }
                QGCLabel { text: root.ftc ? qsTr("协议 v%1").arg(root.ftc.protocolVersion) : qsTr("协议 N/A"); color: qgcPal.colorGrey }
                QGCButton { text: qsTr("关闭"); onClicked: detailsPopup.close() }
            }

            GridLayout {
                Layout.fillWidth: true
                columns: 4
                columnSpacing: 12
                rowSpacing: 4
                QGCLabel { text: qsTr("系统"); color: qgcPal.colorGrey }
                QGCLabel { text: root.systemFresh ? root.ftc.systemStateText : "N/A"; color: root.statusPalette.colorFor(root.systemFresh ? root.ftc.systemSeverity : "unavailable") }
                QGCLabel { text: qsTr("模式"); color: qgcPal.colorGrey }
                QGCLabel { text: root.controlFresh ? root.ftc.controlModeText : "N/A"; color: root.statusPalette.colorFor(root.controlFresh ? root.ftc.controlModeSeverity : "unavailable") }
                QGCLabel { text: qsTr("控制裕度"); color: qgcPal.colorGrey }
                QGCLabel { text: root.controlFresh ? root.ftc.authorityStateText : "N/A"; color: root.statusPalette.colorFor(root.controlFresh ? root.ftc.authoritySeverity : "unavailable") }
                QGCLabel { text: qsTr("恢复状态"); color: qgcPal.colorGrey }
                QGCLabel { text: root.controlFresh || root.extremeFresh ? root.ftc.recoveryStateText : "N/A"; color: root.statusPalette.colorFor(root.controlFresh || root.extremeFresh ? root.ftc.recoverySeverity : "unavailable") }
                QGCLabel { text: qsTr("滚转 / 俯仰"); color: qgcPal.colorGrey }
                QGCLabel { text: root.controlFresh ? root.percentText(root.ftc.rollAuthority) + " / " + root.percentText(root.ftc.pitchAuthority) : "N/A"; color: qgcPal.text }
                QGCLabel { text: qsTr("偏航 / 推力"); color: qgcPal.colorGrey }
                QGCLabel { text: root.controlFresh ? root.percentText(root.ftc.yawAuthority) + " / " + root.percentText(root.ftc.thrustAuthority) : "N/A"; color: qgcPal.text }
                QGCLabel { text: qsTr("撞击 / 失控分数"); color: qgcPal.colorGrey }
                QGCLabel { text: root.extremeFresh ? root.ftc.impactTypeText + " " + root.percentText(root.ftc.impactScore) + " / " + root.percentText(root.ftc.lossOfControlScore) : "N/A"; color: qgcPal.text }
                QGCLabel { text: qsTr("模型质量"); color: qgcPal.colorGrey }
                QGCLabel { text: root.fresh && !root.ftc.motorStale ? root.percentText(root.ftc.modelQuality) : "N/A"; color: qgcPal.text }
            }

            QGCLabel { text: qsTr("电机观测（健康值不是剩余寿命）"); color: qgcPal.text; font.bold: true }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: qgcPal.windowShade
                radius: 5

                ListView {
                    anchors.fill: parent
                    anchors.margins: 6
                    clip: true
                    spacing: 4
                    model: root.ftc ? root.ftc.motors : 0
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 38
                        radius: 4
                        color: qgcPal.window
                        border.color: root.statusPalette.colorFor(severity)
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            QGCLabel { Layout.preferredWidth: 42; text: "M" + (motorIndex + 1); font.bold: true; color: qgcPal.text }
                            QGCLabel { Layout.fillWidth: true; text: available ? qsTr("健康 %1").arg(root.percentText(health)) : qsTr("健康 N/A"); color: qgcPal.text }
                            QGCLabel { Layout.fillWidth: true; text: available ? qsTr("效能 %1").arg(root.percentText(effectiveness)) : qsTr("效能 N/A"); color: qgcPal.text }
                            QGCLabel { Layout.fillWidth: true; text: available ? qsTr("故障概率 %1").arg(root.percentText(faultProbability)) : qsTr("故障概率 N/A"); color: qgcPal.text }
                            QGCLabel { Layout.preferredWidth: 110; text: available ? faultTypeText : qsTr("数据已过期"); color: root.statusPalette.colorFor(severity); elide: Text.ElideRight }
                        }
                    }
                }
            }

            QGCLabel {
                Layout.fillWidth: true
                text: root.ftc && root.ftc.diagnosticsAvailable && !root.ftc.diagnosticsStale
                      ? qsTr("诊断：模型残差 %1 · 分配残差 %2 · 振动 %3 · 原因掩码 0x%4")
                        .arg(Number(root.ftc.modelResidual).toFixed(3))
                        .arg(Number(root.ftc.allocationResidualNorm).toFixed(3))
                        .arg(Number(root.ftc.vibrationScore).toFixed(3))
                        .arg(Number(root.ftc.systemReasonMask).toString(16))
                      : qsTr("诊断数据：N/A")
                color: qgcPal.colorGrey
                wrapMode: Text.WordWrap
            }
        }
    }
}
