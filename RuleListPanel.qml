// List view for existing rules.
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

Item {
    id: root

    // Inputs are created in `Main.qml` and passed down.
    required property RuleModel ruleModel
    required property RuleController ruleController
    required property Theme theme
    property int selectedIndex: -1

    signal ruleSelected(int index)
    signal addRequested()
    signal deleteRequested()

    ColumnLayout {
        anchors.fill: parent
        spacing: theme.spacingSm

        Label {
            text: "规则列表"
            font.family: theme.fontDisplay
            font.pixelSize: 16
            color: theme.textPrimary
        }

        Label {
            text: ruleModel ? ("共 " + ruleModel.rules.length + " 条") : "共 0 条"
            color: theme.textSecondary
            font.pixelSize: 12
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: ruleList
                anchors.fill: parent
                model: ruleModel ? ruleModel.rules : []
                currentIndex: root.selectedIndex
                spacing: theme.spacingXs
                clip: true
                ScrollBar.vertical: ScrollBar {}

                delegate: Rectangle {
                    id: ruleRow
                    width: ListView.view.width
                    height: summaryText.implicitHeight + theme.spacingSm * 2
                    radius: theme.radiusSm
                    color: index === ruleList.currentIndex ? theme.listHighlight : "transparent"

                    Text {
                        id: summaryText
                        text: ruleController ? ruleController.ruleSummary(modelData) : ""
                        color: theme.textPrimary
                        font.pixelSize: 13
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: theme.spacingSm
                        anchors.right: parent.right
                        anchors.rightMargin: theme.spacingSm
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.ruleSelected(index)
                    }
                }
            }

            Label {
                anchors.centerIn: parent
                text: "暂无规则"
                color: theme.textSecondary
                visible: ruleModel && ruleModel.rules.length === 0
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: theme.spacingSm

            Button {
                text: "新增"
                onClicked: root.addRequested()
            }

            Button {
                text: "删除"
                enabled: root.selectedIndex >= 0
                onClicked: root.deleteRequested()
            }

            Item { Layout.fillWidth: true }
        }
    }
}
