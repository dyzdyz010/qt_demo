// Rule editor form and conditions list.
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

Item {
    id: root

    // Inputs are created in `Main.qml` and passed down.
    required property RuleEditorController ruleEditor
    required property RuleController ruleController
    required property FormModel formModel
    required property Theme theme

    RulePreviewDialog {
        id: previewDialog
        theme: root.theme
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: theme.spacingLg

        Label {
            text: "规则编辑"
            font.family: theme.fontDisplay
            font.pixelSize: 18
            color: theme.textPrimary
        }

        Frame {
            Layout.fillWidth: true
            padding: theme.spacingMd
            background: Rectangle { color: theme.panelMuted; radius: theme.radiusMd }

            ColumnLayout {
                anchors.fill: parent
                spacing: theme.spacingSm

                RowLayout {
                    Layout.fillWidth: true
                    spacing: theme.spacingSm

                    Label { text: "规则编号"; Layout.preferredWidth: 80 }
                    TextField {
                        text: ruleEditor ? ruleEditor.ruleId : ""
                        readOnly: true
                        Layout.fillWidth: true
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: theme.spacingSm

                    Label { text: "表单"; Layout.preferredWidth: 80 }
                    ComboBox {
                        id: formCombo
                        Layout.fillWidth: true
                        model: formModel ? formModel.forms : []
                        textRole: "title"
                        valueRole: "id"
                        currentIndex: {
                            if (!formModel || !formModel.forms || !ruleEditor) {
                                return -1;
                            }
                            var forms = formModel.forms;
                            for (var i = 0; i < forms.length; ++i) {
                                if (forms[i].id === ruleEditor.ruleFormId) {
                                    return i;
                                }
                            }
                            return -1;
                        }
                        onActivated: {
                            if (ruleEditor) {
                                ruleEditor.ruleFormId = currentValue;
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: theme.spacingSm

                    Label { text: "命中结果"; Layout.preferredWidth: 80 }
                    ComboBox {
                        id: resultCombo
                        Layout.fillWidth: true
                        model: ruleEditor && ruleEditor.model ? ruleEditor.model.resultOptions : []
                        textRole: "label"
                        valueRole: "code"
                        currentIndex: {
                            if (!ruleEditor || !ruleEditor.model) {
                                return -1;
                            }
                            var items = ruleEditor.model.resultOptions;
                            for (var i = 0; i < items.length; ++i) {
                                if (items[i].code === ruleEditor.ruleResult) {
                                    return i;
                                }
                            }
                            return -1;
                        }
                        onActivated: {
                            if (ruleEditor) {
                                ruleEditor.ruleResult = currentValue;
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: theme.spacingSm

                    Label { text: "启用"; Layout.preferredWidth: 80 }
                    Switch {
                        checked: ruleEditor ? ruleEditor.ruleEnabled : false
                        onToggled: {
                            if (ruleEditor) {
                                ruleEditor.ruleEnabled = checked;
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: theme.spacingSm

            Label { text: "条件"; font.bold: true; color: theme.textPrimary }
            Item { Layout.fillWidth: true }
            Button {
                text: "添加条件"
                onClicked: {
                    if (ruleEditor) {
                        ruleEditor.addCondition();
                    }
                }
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: theme.spacingSm
            background: Rectangle {
                color: theme.panelBackground
                radius: theme.radiusMd
                border.color: theme.borderColor
            }

            ListView {
                id: conditionList
                anchors.fill: parent
                model: ruleEditor ? ruleEditor.conditionsModel : null
                spacing: theme.spacingSm
                clip: true
                reuseItems: false
                ScrollBar.vertical: ScrollBar {}
                delegate: ConditionRow {
                    conditionsModel: ruleEditor ? ruleEditor.conditionsModel : null
                    conditionIndex: index
                    formModel: root.formModel
                    ruleController: root.ruleController
                    formId: ruleEditor ? ruleEditor.ruleFormId : ""
                    theme: root.theme
                }
            }

            Label {
                anchors.centerIn: parent
                text: "暂无条件"
                color: theme.textSecondary
                visible: conditionList.count === 0
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: theme.spacingSm

            Item { Layout.fillWidth: true }

            Button {
                text: "预览规则数据"
                enabled: ruleEditor ? ruleEditor.selectedIndex >= 0 : false
                onClicked: {
                    if (ruleEditor) {
                        previewDialog.previewText = ruleEditor.previewJson();
                        previewDialog.open();
                    }
                }
            }

            Button {
                text: "保存"
                enabled: ruleEditor ? ruleEditor.selectedIndex >= 0 : false
                onClicked: {
                    if (ruleEditor) {
                        ruleEditor.saveRule();
                    }
                }
            }
        }
    }
}
