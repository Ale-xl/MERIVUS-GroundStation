import QtQuick          2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs  1.3
import QtQuick.Layouts  1.11
import Qt.labs.settings 1.0

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0

Item {
    id: root

    property bool expanded: false
    property bool settingsOpen: false
    property bool agentRequestRunning: false
    property bool _bubbleHovered: false
    property bool _panelHovered: false
    property var vehicles: QGroundControl.multiVehicleManager.vehicles
    property var activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var pendingIntent: null
    property string pendingSummary: ""
    property real headerOffset: mainWindow.header && mainWindow.header.visible ? mainWindow.header.height + 6 : 8
    property real availablePanelHeight: parent ? Math.max(340, parent.height - headerOffset - 10) : 760
    property real minPanelWidth: Math.min(390, parent ? parent.width * 0.82 : 390)
    property real maxPanelWidth: parent ? Math.max(minPanelWidth, Math.min(parent.width * 0.46, 620)) : 520
    property real minPanelHeight: Math.min(440, availablePanelHeight)
    property real maxPanelHeight: Math.max(minPanelHeight, availablePanelHeight)
    property real panelWidth: clamp(assistantSettings.panelWidth, minPanelWidth, maxPanelWidth)
    property real panelHeight: clamp(assistantSettings.panelHeight, minPanelHeight, maxPanelHeight)
    property real panelTopMargin: clamp(assistantSettings.panelTopMargin, headerOffset,
                                        parent ? Math.max(headerOffset, parent.height - panelHeight - 8) : headerOffset)
    readonly property string _agentGuide:
        "推荐部署方式：在地面站本机启动一个外部 Agent HTTP 服务，面板只把文本、飞行器摘要和最近消息发给该服务；服务内部再调用 OpenAI/本地大模型/MCP 工具，返回 reply 或受控 intent。飞行动作 intent 必须经过本面板白名单和人工确认后才会调用 Vehicle 接口。\n\n" +
        "默认接口：POST /merivus/agent\n" +
        "请求字段：message、model、fleet、history\n" +
        "响应字段：reply、intent。intent.action 仅允许 takeoff、land、rtl、pause。"

    anchors.fill: parent
    z: QGroundControl.zOrderTopMost + 20

    QGCPalette { id: qgcPal; colorGroupEnabled: true }

    Settings {
        id: assistantSettings
        category: "MerivusAIAssistant"

        property real panelWidth: 448
        property real panelHeight: 760
        property real panelTopMargin: 112
        property int layoutVersion: 0
        property bool agentEnabled: false
        property string agentEndpoint: "http://127.0.0.1:8765/merivus/agent"
        property string agentModel: "gpt-4.1-mini"
        property int maxMessages: 80
    }

    Component.onCompleted: Qt.callLater(resetPanelLayoutIfNeeded)

    onAvailablePanelHeightChanged: {
        assistantSettings.panelHeight = clamp(assistantSettings.panelHeight, minPanelHeight, maxPanelHeight)
        assistantSettings.panelTopMargin = clamp(assistantSettings.panelTopMargin, headerOffset,
                                                 parent ? Math.max(headerOffset, parent.height - panelHeight - 8) : headerOffset)
    }

    function tr(text) { return qsTr(text) }

    function clamp(value, minValue, maxValue) {
        return Math.max(minValue, Math.min(maxValue, value))
    }

    function resetPanelLayoutIfNeeded() {
        if (!parent || parent.height <= 0) return
        if (assistantSettings.layoutVersion < 3) {
            assistantSettings.panelWidth = clamp(parent.width * 0.235, minPanelWidth, maxPanelWidth)
            assistantSettings.panelHeight = maxPanelHeight
            assistantSettings.panelTopMargin = headerOffset
            assistantSettings.layoutVersion = 3
        }
    }

    function appendMessage(role, text) {
        chatModel.append({ role: role, text: text })
        trimHistory()
        Qt.callLater(function() { chatList.positionViewAtEnd() })
    }

    function trimHistory() {
        var limit = Math.max(12, assistantSettings.maxMessages)
        while (chatModel.count > limit) {
            chatModel.remove(1)
        }
    }

    function clearChatHistory() {
        chatModel.clear()
        chatModel.append({
            role: "assistant",
            text: tr("你好，我可以协助查看飞行器状态、解释参数，并把起飞/降落/返航/暂停转换为待确认命令。")
        })
        pendingIntent = null
        pendingSummary = ""
        Qt.callLater(function() { chatList.positionViewAtEnd() })
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
        if (!ids) return result
        for (var i = 0; i < ids.length; i++) {
            var id = parseInt(ids[i])
            if (!isNaN(id) && result.indexOf(id) === -1) result.push(id)
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
        return clamp(altitude, 1, 120)
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
        ids = normalizeIds(ids)
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
                       ? tr("准备让 %1 起飞到 %2 米。").arg(targetText).arg(Number(intent.altitude).toFixed(1))
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

    function routeLocalText(clean) {
        if (/清空|清除|删除.*历史|clear/i.test(clean)) {
            clearChatHistory()
            appendMessage("assistant", tr("已清空当前会话记录。"))
            return true
        }

        if (/智能体|大模型|agent|llm|mcp|部署/i.test(clean)) {
            appendMessage("assistant", _agentGuide)
            return true
        }

        if (/帮助|help|怎么用/i.test(clean)) {
            appendMessage("assistant", tr("我可以做基础问答和快捷指令：例如“1号起飞10米”“2号降落”“全部返航”“查看飞行器状态”。涉及飞行动作时，我会先生成待确认命令。打开配置后可接入本机 Agent 服务。"))
            return true
        }

        if (/状态|在线|电量|高度|status|list/i.test(clean)) {
            appendMessage("assistant", describeFleet())
            return true
        }

        if (/参数|parameter|说明/i.test(clean)) {
            appendMessage("assistant", tr("参数页面已增加鼠标悬停说明卡片，会直接读取 PX4/APM 参数元数据中的短说明、长说明、单位、范围和默认值。"))
            return true
        }

        var ids = parseVehicleIds(clean)
        if (/起飞|take\s*off|takeoff/i.test(clean)) {
            prepareIntent(buildIntent("takeoff", ids, parseAltitude(clean)))
            return true
        } else if (/降落|着陆|land/i.test(clean)) {
            prepareIntent(buildIntent("land", ids, 0))
            return true
        } else if (/返航|rtl|return/i.test(clean)) {
            prepareIntent(buildIntent("rtl", ids, 0))
            return true
        } else if (/暂停|悬停|pause|hold/i.test(clean)) {
            prepareIntent(buildIntent("pause", ids, 0))
            return true
        }

        return false
    }

    function chatHistoryForAgent() {
        var history = []
        var start = Math.max(0, chatModel.count - 18)
        for (var i = start; i < chatModel.count; i++) {
            var item = chatModel.get(i)
            history.push({ role: item.role, content: item.text })
        }
        return history
    }

    function handleAgentIntent(intent) {
        if (!intent || !intent.action) return false
        var action = String(intent.action).toLowerCase()
        if (["takeoff", "land", "rtl", "pause"].indexOf(action) === -1) return false
        var ids = normalizeIds(intent.vehicleIds || intent.ids || defaultVehicleIds())
        var altitude = action === "takeoff" ? clamp(Number(intent.altitude || 10), 1, 120) : 0
        prepareIntent(buildIntent(action, ids, altitude))
        return true
    }

    function callAgent(clean) {
        if (agentRequestRunning) return
        agentRequestRunning = true
        appendMessage("assistant", tr("正在发送给本机 Agent：%1").arg(assistantSettings.agentEndpoint))

        var xhr = new XMLHttpRequest()
        xhr.open("POST", assistantSettings.agentEndpoint)
        xhr.setRequestHeader("Content-Type", "application/json")
        xhr.onreadystatechange = function() {
            if (xhr.readyState !== XMLHttpRequest.DONE) return
            agentRequestRunning = false

            if (xhr.status < 200 || xhr.status >= 300) {
                appendMessage("assistant", tr("Agent 暂不可用（HTTP %1）。已保留本地规则模式，可先使用状态查询和快捷命令。").arg(xhr.status))
                return
            }

            try {
                var data = JSON.parse(xhr.responseText)
                if (data.reply) appendMessage("assistant", data.reply)
                if (data.intent) handleAgentIntent(data.intent)
                if (!data.reply && !data.intent) {
                    appendMessage("assistant", tr("Agent 已响应，但没有返回 reply 或 intent 字段。"))
                }
            } catch (e) {
                appendMessage("assistant", tr("Agent 响应不是有效 JSON：%1").arg(e))
            }
        }

        xhr.onerror = function() {
            agentRequestRunning = false
            appendMessage("assistant", tr("无法连接 Agent 服务。请确认本机服务已启动，或关闭“启用外部 Agent”。"))
        }

        xhr.send(JSON.stringify({
            message: clean,
            model: assistantSettings.agentModel,
            fleet: describeFleet(),
            history: chatHistoryForAgent()
        }))
    }

    function handleUserText(text) {
        var clean = text.trim()
        if (clean.length === 0) return
        appendMessage("user", clean)

        if (routeLocalText(clean)) return

        if (assistantSettings.agentEnabled) {
            callAgent(clean)
        } else {
            appendMessage("assistant", tr("当前使用本地规则模式：支持状态查询、参数说明提示，以及起飞/降落/返航/暂停快捷命令。复杂坐标和航线操作建议继续使用地图交互；需要大模型能力时可在右上角配置中启用外部 Agent。"))
        }
    }

    Rectangle {
        id: assistantPanel
        anchors.top: parent.top
        anchors.topMargin: root.panelTopMargin
        anchors.right: parent.right
        anchors.rightMargin: 8
        width: root.panelWidth
        height: root.panelHeight
        radius: 8
        color: Qt.rgba(qgcPal.window.r, qgcPal.window.g, qgcPal.window.b, _panelHovered ? 0.98 : 0.94)
        border.color: _panelHovered ? qgcPal.buttonHighlight : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.24)
        border.width: _panelHovered ? 2 : 1
        visible: root.expanded
        clip: true

        Behavior on width { NumberAnimation { duration: resizeLeftHandle.pressed ? 0 : 120 } }
        Behavior on height { NumberAnimation { duration: resizeTopHandle.pressed || resizeBottomHandle.pressed ? 0 : 120 } }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.NoButton
            onEntered: root._panelHovered = true
            onExited: root._panelHovered = false
        }

        Rectangle {
            id: resizeRail
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 6
            color: resizeLeftHandle.containsMouse || resizeLeftHandle.pressed ? qgcPal.buttonHighlight : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.14)
            opacity: resizeLeftHandle.containsMouse || resizeLeftHandle.pressed ? 0.95 : 0.55
            z: 4
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 6
            color: resizeTopHandle.containsMouse || resizeTopHandle.pressed ? qgcPal.buttonHighlight : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.10)
            opacity: resizeTopHandle.containsMouse || resizeTopHandle.pressed ? 0.95 : 0.0
            z: 4
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 6
            color: resizeBottomHandle.containsMouse || resizeBottomHandle.pressed ? qgcPal.buttonHighlight : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.10)
            opacity: resizeBottomHandle.containsMouse || resizeBottomHandle.pressed ? 0.95 : 0.0
            z: 4
        }

        MouseArea {
            id: resizeLeftHandle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 14
            hoverEnabled: true
            cursorShape: Qt.SizeHorCursor

            property real startX: 0
            property real startWidth: 0

            onPressed: {
                startX = mouse.x
                startWidth = assistantSettings.panelWidth
            }
            onPositionChanged: {
                if (pressed) {
                    var delta = startX - mouse.x
                    assistantSettings.panelWidth = root.clamp(startWidth + delta, root.minPanelWidth, root.maxPanelWidth)
                }
            }
        }

        MouseArea {
            id: resizeTopHandle
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 14
            hoverEnabled: true
            cursorShape: Qt.SizeVerCursor
            z: 5

            property real startY: 0
            property real startHeight: 0
            property real startTop: 0

            onPressed: {
                startY = mouse.y
                startHeight = assistantSettings.panelHeight
                startTop = assistantSettings.panelTopMargin
            }
            onPositionChanged: {
                if (!pressed || !parent.parent) return
                var delta = mouse.y - startY
                var bottom = startTop + startHeight
                var newTop = root.clamp(startTop + delta, root.headerOffset,
                                        Math.max(root.headerOffset, bottom - root.minPanelHeight))
                var newHeight = root.clamp(bottom - newTop, root.minPanelHeight, root.maxPanelHeight)
                assistantSettings.panelTopMargin = newTop
                assistantSettings.panelHeight = newHeight
            }
        }

        MouseArea {
            id: resizeBottomHandle
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 14
            hoverEnabled: true
            cursorShape: Qt.SizeVerCursor
            z: 5

            property real startY: 0
            property real startHeight: 0

            onPressed: {
                startY = mouse.y
                startHeight = assistantSettings.panelHeight
            }
            onPositionChanged: {
                if (pressed) {
                    var delta = mouse.y - startY
                    assistantSettings.panelHeight = root.clamp(startHeight + delta, root.minPanelHeight, root.maxPanelHeight)
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: 14
            anchors.rightMargin: 10
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            spacing: 8

            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.preferredWidth: 34
                    Layout.preferredHeight: 34
                    radius: 17
                    color: Qt.rgba(qgcPal.buttonHighlight.r, qgcPal.buttonHighlight.g, qgcPal.buttonHighlight.b, 0.20)
                    border.color: qgcPal.buttonHighlight
                    border.width: 1

                    QGCColoredImage {
                        anchors.centerIn: parent
                        width: 24
                        height: 24
                        source: "qrc:/qml/QGroundControl/FlightDisplay/ai-nine-star.svg"
                        color: qgcPal.buttonHighlight
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
                        text: vehicles ? tr("%1 架飞行器在线 · %2")
                                         .arg(vehicles.count)
                                         .arg(assistantSettings.agentEnabled ? tr("Agent") : tr("本地规则"))
                                       : tr("未连接飞行器 · %1").arg(assistantSettings.agentEnabled ? tr("Agent") : tr("本地规则"))
                        font.pixelSize: 11
                        color: qgcPal.colorGrey
                    }
                }

                QGCButton {
                    Layout.preferredWidth: 54
                    Layout.preferredHeight: 28
                    text: root.settingsOpen ? tr("聊天") : tr("配置")
                    onClicked: root.settingsOpen = !root.settingsOpen
                }

                QGCButton {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 28
                    text: "×"
                    onClicked: root.expanded = false
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.12)
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: root.settingsOpen ? 218 : 0
                visible: root.settingsOpen
                radius: 7
                color: Qt.rgba(qgcPal.windowShade.r, qgcPal.windowShade.g, qgcPal.windowShade.b, 0.88)
                border.color: Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.12)
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 7

                    RowLayout {
                        Layout.fillWidth: true
                        QGCCheckBox {
                            id: agentSwitch
                            text: tr("启用外部 Agent")
                            checked: assistantSettings.agentEnabled
                            onClicked: assistantSettings.agentEnabled = checked
                        }
                        QGCLabel {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignRight
                            text: agentRequestRunning ? tr("请求中") : tr("白名单确认执行")
                            color: agentRequestRunning ? qgcPal.colorOrange : qgcPal.colorGrey
                            font.pixelSize: 11
                        }
                    }

                    QGCTextField {
                        Layout.fillWidth: true
                        text: assistantSettings.agentEndpoint
                        placeholderText: tr("Agent HTTP 端点")
                        onEditingFinished: assistantSettings.agentEndpoint = text
                    }

                    QGCTextField {
                        Layout.fillWidth: true
                        text: assistantSettings.agentModel
                        placeholderText: tr("模型名称")
                        onEditingFinished: assistantSettings.agentModel = text
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        QGCLabel {
                            Layout.fillWidth: true
                            text: tr("消息保留：%1 条").arg(assistantSettings.maxMessages)
                            color: qgcPal.text
                        }
                        QGCButton {
                            Layout.preferredWidth: 32
                            text: "-"
                            onClicked: assistantSettings.maxMessages = Math.max(20, assistantSettings.maxMessages - 20)
                        }
                        QGCButton {
                            Layout.preferredWidth: 32
                            text: "+"
                            onClicked: assistantSettings.maxMessages = Math.min(200, assistantSettings.maxMessages + 20)
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        QGCButton {
                            Layout.fillWidth: true
                            text: tr("清空历史")
                            onClicked: root.clearChatHistory()
                        }
                        QGCButton {
                            Layout.fillWidth: true
                            text: tr("生成接入说明")
                            onClicked: root.appendMessage("assistant", root._agentGuide)
                        }
                    }
                }
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
                        width: Math.min(parent.width * 0.88, Math.max(92, messageText.implicitWidth + 22))
                        implicitHeight: messageText.implicitHeight + 16
                        x: model.role === "user" ? parent.width - width : 0
                        radius: 8
                        color: model.role === "user" ? Qt.rgba(qgcPal.buttonHighlight.r, qgcPal.buttonHighlight.g, qgcPal.buttonHighlight.b, 0.95)
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
                            color: model.role === "user" ? qgcPal.buttonHighlightText : qgcPal.text
                            font.pixelSize: 12
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: pendingIntent !== null ? 76 : 0
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
                QGCTextField {
                    id: inputField
                    Layout.fillWidth: true
                    placeholderText: agentRequestRunning ? tr("等待 Agent 响应...") : tr("输入指令或问题")
                    enabled: !agentRequestRunning
                    onAccepted: {
                        root.handleUserText(text)
                        text = ""
                    }
                }
                QGCButton {
                    text: tr("发送")
                    enabled: !agentRequestRunning
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
        anchors.rightMargin: root._bubbleHovered ? 18 : 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: root._bubbleHovered ? 18 : 12
        width: root._bubbleHovered ? 56 : 48
        height: width
        radius: width / 2
        visible: !root.expanded
        opacity: root._bubbleHovered ? 1.0 : 0.48
        color: root._bubbleHovered ? qgcPal.buttonHighlight : Qt.rgba(qgcPal.window.r, qgcPal.window.g, qgcPal.window.b, 0.76)
        border.color: root._bubbleHovered ? qgcPal.buttonHighlightText : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.40)
        border.width: root._bubbleHovered ? 2 : 1

        Behavior on opacity { NumberAnimation { duration: 140 } }
        Behavior on width { NumberAnimation { duration: 140 } }

        QGCColoredImage {
            anchors.centerIn: parent
            width: parent.width * 0.58
            height: width
            source: "qrc:/qml/QGroundControl/FlightDisplay/ai-nine-star.svg"
            color: root._bubbleHovered ? qgcPal.buttonHighlightText : Qt.rgba(qgcPal.text.r, qgcPal.text.g, qgcPal.text.b, 0.72)
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onEntered: root._bubbleHovered = true
            onExited: root._bubbleHovered = false
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
