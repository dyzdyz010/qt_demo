// Rule configuration page layout.
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

Item {
    id: root

    // Inputs are created in `Main.qml` and passed down.
    required property FormModel formModel
    required property RuleEditorController ruleEditor
    required property RuleController ruleController
    required property Theme theme

    RowLayout {
        anchors.fill: parent
        spacing: theme.spacingLg

        Frame {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            padding: theme.spacingMd
            background: Rectangle {
                color: theme.panelMuted
                radius: theme.radiusMd
            }

            RuleListPanel {
                anchors.fill: parent
                ruleModel: ruleEditor ? ruleEditor.model : null
                ruleController: root.ruleController
                theme: root.theme
                selectedIndex: ruleEditor ? ruleEditor.selectedIndex : -1
                onRuleSelected: {
                    if (ruleEditor) {
                        ruleEditor.selectedIndex = selectedIndex;
                    }
                }
                onAddRequested: {
                    if (ruleEditor) {
                        ruleEditor.addRule();
                    }
                }
                onDeleteRequested: {
                    if (ruleEditor) {
                        ruleEditor.removeRule();
                    }
                }
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: theme.spacingLg
            background: Rectangle {
                color: theme.panelBackground
                radius: theme.radiusMd
                border.color: theme.borderColor
            }

            RuleEditorPanel {
                anchors.fill: parent
                ruleEditor: root.ruleEditor
                ruleController: root.ruleController
                formModel: root.formModel
                theme: root.theme
            }
        }
    }
}
