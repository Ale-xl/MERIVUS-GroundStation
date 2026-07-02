import QtQuick          2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs  1.3
import QtQuick.Layouts  1.11

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0

Item {
    id: root

    property bool expanded: false
    property var vehicles: QGroundControl.multiVehicleManager.vehicles
    property var activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var pendingIntent: null
    property string pendingSummary: ""

    anchors.fill: parent
    z: QGroundControl.zOrderTopMost + 20

    QGCPalette { id: qgcPal; colorGroupEnabled: true }

    function tr(text) { return qsTr(text) }

    function appendMessage(role, text) {
        chatModel.append({ role: role, text: text })
        chatList.positionViewAtEnd()
    }

    function vehicleById(vehicleId) {
        if (!vehicles) return null
        for (var i = 0; i < vehicles.count; i++) {
            var vehicle = vehicles.get(i)
            if (vehicle && vehicle.id === vehicleId) return vehicle
        }
        return null
    }

    function allVehicleIds() {
        var ids = []
        if (!vehicles) return ids
        for (var i = 0; i < vehicles.count; i++) {
            var vehicle = vehicles.get(i)
            if (vehicle) ids.push(vehicle.id)
        }
        return ids
    }

    function defaultVehicleIds() {
        return activeVehicle ? [ activeVehicle.id ] : []
    }

    function normalizeIds(ids) {
        var result = []
        for (var i = 0; i < ids.length; i++) {
            if (result.indexOf(ids[i]) === -1) result.push(ids[i])
        }
        return result
    }

    function parseVehicleIds(text) {
        if (/全部|所有|全体|all/i.test(text)) return allVehicleIds()

        var ids = []
        var groupMatch = /([0-9,\s，、]+)\s*号/.exec(text)
        if (groupMatch) {
            var parts = groupMatch[1].split(/[,\s，、]+/)
            for (var p = 0; p < parts.length; p++) {
                var id = parseInt(parts[p])
                if (!isNaN(id)) ids.push(id)
            }
        }

        var match
        var numbered = /(?:UAV[-_\s]*|无人机\s*|飞机\s*)?([0-9]+)\s*号/ig
        while ((match = numbered.exec(text)) !== null) {
            var numberedId = parseInt(match[1])
            if (!isNaN(numberedId)) ids.push(numberedId)
        }

        var uavPattern = /UAV[-_\s]*([0-9]+)/ig
        while ((match = uavPattern.exec(text)) !== null) {
            var uavId = parseInt(match[1])
            if (!isNaN(uavId)) ids.push(uavId)
        }

        return normalizeIds(ids.length > 0 ? ids : defaultVehicleIds())
    }

    function parseAltitude(text) {
        var match = /([0-9]+(?:\.[0-9]+)?)\s*(?:米|m|meter|meters)/i.exec(text)
        if (!match) return 10
        var altitude = parseFloat(match[1])
        if (isNaN(altitude)) return 10
        return Math.max(1, Math.min(120, altitude))
    }

    function vehicleSummary(vehicle) {
        if (!vehicle) return ""
        var battery = vehicle.batteries && vehicle.batteries.count > 0 ? Number(vehicle.batteries.get(0).percentRemaining.rawValue).toFixed(0) + "%" : "--"
        var altitude = vehicle.altitudeRelative ? Number(vehicle.altitudeRelative.rawValue).toFixed(1) + " m" : "--"
        var mode = vehicle.flightMode ? vehicle.flightMode : "--"
        return tr("UAV-%1：%2，模式 %3，高度 %4，电量 %5")
                .arg(vehicle.id)
                .arg(vehicle.armed ? tr("已解锁") : tr("未解锁"))
                .arg(mode)
                .arg(altitude)
                .arg(battery)
    }

    function describeFleet() {
        if (!vehicles || vehicles.count === 0) return tr("当前没有连接的飞行器。")
        var lines = []
        for (var i = 0; i < vehicles.count; i++) {
            lines.push(vehicleSummary(vehicles.get(i)))
        }
        return lines.join("\n")
    }

    function buildIntent(action, ids, altitude) {
        if (!ids || ids.length === 0) {
            appendMessage("assistant", tr("没有可用飞行器。请先连接飞控，或明确指定 UAV 编号。"))
            return null
        }

        var missing = []
        for (var i = 0; i < ids.length; i++) {
            if (!vehicleById(ids[i])) missing.push(ids[i])
        }
        if (missing.length > 0) {
            appendMessage("assistant", tr("没有找到 UAV-%1。").arg(missing.join(", ")))
            return null
        }

        var title = action === "takeoff" ? tr("起飞")
                  : action === "land" ? tr("降落")
                  : action === "rtl" ? tr("返航")
                  : tr("暂停")
        return { action: action, title: title, vehicleIds: ids, altitude: altitude }
    }

    function prepareIntent(intent) {
        if (!intent) return
        var targetText = intent.vehicleIds.length === 1 ? tr("UAV-%1").arg(intent.vehicleIds[0])
                                                        : tr("%1 架飞行器（%2）").arg(intent.vehicleIds.length).arg(intent.vehicleIds.join(", "))
        pendingIntent = intent
        pendingSummary = intent.action === "takeoff"
                       ? tr("准备让 %1 起飞到 %2 米。").arg(targetText).arg(intent.altitude.toFixed(1))
                       : tr("准备让 %1 执行%2。").arg(targetText).arg(intent.title)
        appendMessage("assistant", pendingSummary + "\n" + tr("请在面板内确认后再下发。"))
    }

    function executePendingIntent() {
        if (!pendingIntent) return
        var intent = pendingIntent
        pendingIntent = null
        pendingSummary = ""

        var results = []
        for (var i = 0; i < intent.vehicleIds.length; i++) {
            var vehicle = vehicleById(intent.vehicleIds[i])
            if (!vehicle) {
                results.push(tr("UAV-%1 不在线").arg(intent.vehicleIds[i]))
                continue
            }

            if (intent.action === "takeoff") {
                vehicle.guidedModeTakeoff(intent.altitude)
                results.push(tr("UAV-%1 已发送起飞确认命令").arg(vehicle.id))
            } else if (intent.action === "land") {
                vehicle.guidedModeLand()
                results.push(tr("UAV-%1 已发送降落命令").arg(vehicle.id))
            } else if (intent.action === "rtl") {
                vehicle.guidedModeRTL(false)
                results.push(tr("UAV-%1 已发送返航命令").arg(vehicle.id))
            } else if (intent.action === "pause") {
                vehicle.pauseVehicle()
                results.push(tr("UAV-%1 已发送暂停命令").arg(vehicle.id))
            }
        }
        appendMessage("assistant", results.join("\n"))
    }

    function cancelPendingIntent() {
        if (!pendingIntent) return
        appendMessage("assistant", tr("已取消：%1").arg(pendingSummary))
        pendingIntent = null
        pendingSummary = ""
    }

    function handleUserText(text) {
        var clean = text.trim()
        if (clean.length === 0) return
        appendMessage("user", clean)

        if (/帮助|help|怎么用/i.test(clean)) {
            appendMessage("assistant", tr("我可以做基础问答和快捷指令：例如“1号起飞10米”“2号降落”“全部返航”“查看飞行器状态”。涉及飞行动作时，我会先生成待确认命令。"))
            return
        }

        if (/状态|在线|电量|高度|status|list/i.test(clean)) {
            appendMessage("assistant", describeFleet())
            return
        }

        if (/参数|parameter|说明/i.test(clean)) {
            appendMessage("assistant", tr("参数页面已增加鼠标悬停说明卡片，会直接读取 PX4/APM 参数元数据中的短说明、长说明、单位、范围和默认值。"))
            return
        }

        var ids = parseVehicleIds(clean)
        if (/起飞|take\s*off|takeoff/i.test(clean)) {
            prepareIntent(buildIntent("takeoff", ids, parseAltitude(clean)))
        } else if (/降落|着陆|land/i.test(clean)) {
            prepareIntent(buildIntent("land", ids, 0))
        } else if (/返航|rtl|return/i.test(clean)) {
            prepareIntent(buildIntent("rtl", ids, 0))
        } else if (/暂停|悬停|pause|hold/i.test(clean)) {
            prepareIntent(buildIntent("pause", ids, 0))
        } else {
            appendMessage("assistant", tr("当前版本先支持状态查询、参数说明提示，以及起飞/降落/返航/暂停快捷命令。复杂坐标和航线操作建议继续使用地图交互。"))
        }
    }

    Rectangle {
        id: assistantPanel
        anchors.top: parent.top
        anchors.topMargin: mainWindow.header ? mainWindow.header.height + 8 : 8
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        width: Math.min(390, Math.max(320, parent.width * 0.24))
        radius: 8
        color: Qt.rgba(qgcPal.window.r, qgcPal.window.g, qgcPal.window.b, 0.97)
        border.color: Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.18)
        visible: root.expanded
        clip: true

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 30
                    radius: 15
                    color: qgcPal.buttonHighlight
                    QGCLabel {
                        anchors.centerIn: parent
                        text: "AI"
                        color: qgcPal.buttonHighlightText
                        font.bold: true
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 0
                    QGCLabel {
                        Layout.fillWidth: true
                        text: tr("Merivus AI")
                        font.bold: true
                        font.pixelSize: 16
                        color: qgcPal.text
                    }
                    QGCLabel {
                        Layout.fillWidth: true
                        text: vehicles ? tr("%1 架飞行器在线").arg(vehicles.count) : tr("未连接飞行器")
                        font.pixelSize: 11
                        color: qgcPal.colorGrey
                    }
                }

                QGCButton {
                    Layout.preferredWidth: 28
                    Layout.preferredHeight: 26
                    text: "×"
                    onClicked: root.expanded = false
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.12)
            }

            ListView {
                id: chatList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 8
                model: ListModel {
                    id: chatModel
                    ListElement {
                        role: "assistant"
                        text: "你好，我可以协助查看飞行器状态、解释参数，并把起飞/降落/返航/暂停转换为待确认命令。"
                    }
                }

                delegate: Item {
                    width: chatList.width
                    height: bubble.implicitHeight

                    Rectangle {
                        id: bubble
                        width: Math.min(parent.width * 0.86, messageText.implicitWidth + 22)
                        implicitHeight: messageText.implicitHeight + 16
                        x: role === "user" ? parent.width - width : 0
                        radius: 8
                        color: role === "user" ? Qt.rgba(qgcPal.buttonHighlight.r, qgcPal.buttonHighlight.g, qgcPal.buttonHighlight.b, 0.95)
                                               : qgcPal.windowShade
                        border.color: Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.10)

                        QGCLabel {
                            id: messageText
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.margins: 8
                            text: model.text
                            wrapMode: Text.WordWrap
                            color: role === "user" ? qgcPal.buttonHighlightText : qgcPal.text
                            font.pixelSize: 12
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: pendingIntent !== null ? 74 : 0
                visible: pendingIntent !== null
                radius: 7
                color: Qt.rgba(qgcPal.buttonHighlight.r, qgcPal.buttonHighlight.g, qgcPal.buttonHighlight.b, 0.12)
                border.color: qgcPal.buttonHighlight

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 6

                    QGCLabel {
                        Layout.fillWidth: true
                        text: pendingSummary
                        color: qgcPal.text
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        QGCButton {
                            Layout.fillWidth: true
                            text: tr("确认执行")
                            onClicked: root.executePendingIntent()
                        }
                        QGCButton {
                            Layout.fillWidth: true
                            text: tr("取消")
                            onClicked: root.cancelPendingIntent()
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 5
                Repeater {
                    model: [ tr("状态"), tr("1号起飞10米"), tr("降落"), tr("返航") ]
                    QGCButton {
                        Layout.fillWidth: true
                        text: modelData
                        onClicked: root.handleUserText(modelData)
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                QGCTextField {
                    id: inputField
                    Layout.fillWidth: true
                    placeholderText: tr("输入指令或问题")
                    onAccepted: {
                        root.handleUserText(text)
                        text = ""
                    }
                }
                QGCButton {
                    text: tr("发送")
                    onClicked: {
                        root.handleUserText(inputField.text)
                        inputField.text = ""
                    }
                }
            }
        }
    }

    Rectangle {
        id: bubbleButton
        anchors.right: parent.right
        anchors.rightMargin: 18
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18
        width: 52
        height: 52
        radius: 26
        visible: !root.expanded
        color: qgcPal.buttonHighlight
        border.color: Qt.rgba(qgcPal.buttonHighlightText.r, qgcPal.buttonHighlightText.g, qgcPal.buttonHighlightText.b, 0.50)
        border.width: 1

        QGCLabel {
            anchors.centerIn: parent
            text: "AI"
            color: qgcPal.buttonHighlightText
            font.bold: true
            font.pixelSize: 16
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.expanded = true
        }

        Rectangle {
            width: 10
            height: 10
            radius: 5
            anchors.right: parent.right
            anchors.top: parent.top
            color: vehicles && vehicles.count > 0 ? qgcPal.colorGreen : qgcPal.colorOrange
            border.color: qgcPal.window
            border.width: 1
        }
    }
}
