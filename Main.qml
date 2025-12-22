/*
  App wiring (data-flow):
  - `FormModel` provides form definitions + navigation items.
  - `RuleModel` stores rules in memory.
  - `RuleController` runs evaluation for a form.
  - `RuleEditorController` holds editor state for the config page.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

ApplicationWindow {
    id: root
    width: 1200
    height: 720
    visible: true
    title: "规则引擎演示"
    font.family: theme.fontBody

    Theme { id: theme }
    FormModel { id: formModel }
    RuleModel { id: ruleModel }
    RuleController { id: ruleController; model: ruleModel; formModel: formModel }
    RuleEditorController { id: ruleEditor; model: ruleModel; formModel: formModel; controller: ruleController }

    background: Rectangle { color: theme.appBackground }

    header: ToolBar {
        height: theme.headerHeight
        background: Rectangle { color: theme.panelBackground; border.color: theme.borderColor }

        RowLayout {
            anchors.fill: parent
            anchors.margins: theme.spacingMd
            spacing: theme.spacingMd

            Label {
                text: "规则引擎演示"
                font.family: theme.fontDisplay
                font.pixelSize: 18
                color: theme.textPrimary
            }

            Label {
                text: "表单驱动 · 规则配置 · 结果校验"
                color: theme.textSecondary
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }

            Item { Layout.fillWidth: true }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: theme.spacingLg
        spacing: theme.spacingLg

        Frame {
            Layout.preferredWidth: theme.navWidth
            Layout.fillHeight: true
            padding: theme.spacingMd
            background: Rectangle { color: theme.navBackground; radius: theme.radiusMd }

            ColumnLayout {
                anchors.fill: parent
                spacing: theme.spacingSm

                Label {
                    text: "导航"
                    color: theme.navText
                    font.bold: true
                }

                ListView {
                    id: navList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: formModel.navigationItems
                    currentIndex: 0
                    spacing: theme.spacingXs
                    clip: true

                    delegate: ItemDelegate {
                        width: ListView.view.width
                        highlighted: ListView.isCurrentItem
                        onClicked: navList.currentIndex = index
                        contentItem: Text {
                            text: modelData ? modelData.title : ""
                            color: highlighted ? theme.navTextActive : theme.navText
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            radius: theme.radiusSm
                            color: highlighted ? theme.navHighlight : "transparent"
                        }
                    }
                }
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: theme.spacingMd
            background: Rectangle { color: theme.panelBackground; radius: theme.radiusMd; border.color: theme.borderColor }

            StackLayout {
                anchors.fill: parent
                currentIndex: navList.currentIndex

                Repeater {
                    model: formModel.forms
                    Item {
                        required property var modelData

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        FormPage {
                            anchors.fill: parent
                            p_formDefinition: modelData
                            p_ruleController: ruleController
                            p_theme: theme
                        }
                    }
                }

                RuleConfig {
                    formModel: formModel
                    ruleEditor: ruleEditor
                    ruleController: ruleController
                    theme: theme
                }
            }
        }
    }
}
