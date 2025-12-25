// Editor row for a single rule condition.
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

Item {
    id: root

    required property var conditionsModel
    required property int conditionIndex
    required property FormModel formModel
    required property RuleController ruleController
    required property string formId
    required property Theme theme

    // Used as a `ListView` delegate for `ConditionModel`:
    // role properties `key`, `op`, `valueText` are injected by the view.
    property string roleKey: typeof key === "undefined" ? "" : key
    property string roleOp: typeof op === "undefined" ? "" : op
    property string roleValueText: typeof valueText === "undefined" ? "" : valueText

    width: parent ? parent.width : 0
    implicitHeight: contentFrame.implicitHeight

    function findIndexByRole(items, roleName, value) {
        if (!items || items.length === 0) {
            return -1;
        }
        for (var i = 0; i < items.length; ++i) {
            if (items[i][roleName] === value) {
                return i;
            }
        }
        return -1;
    }

    ListValueModel {
        id: listModel
        valueText: root.roleValueText
        onValueTextChanged: {
            if (conditionsModel && conditionIndex >= 0) {
                conditionsModel.setValueText(conditionIndex, valueText);
            }
        }
    }

    Frame {
        id: contentFrame
        width: parent.width
        Layout.fillHeight: true
        padding: theme.spacingSm
        background: Rectangle {
            color: theme.panelMuted
            radius: theme.radiusSm
            border.color: theme.borderColor
        }


        // Label {
        //     text: qsTr("%1").arg(JSON.stringify(formModel.fieldsForForm(formId)))
        // }

        // Label {
        //     text: qsTr("%1").arg(JSON.stringify(ruleController.model.opOptions))
        // }

        RowLayout {
            width: parent.width
            spacing: theme.spacingSm

            ComboBox {
                id: fieldCombo
                Layout.preferredWidth: 170
                model: formModel ? formModel.fieldsForForm(formId) : []
                textRole: "label"
                valueRole: "key"
                currentIndex: {
                    var condition = conditionsModel.conditions[conditionIndex];
                    var idx = findIndexByRole(fieldCombo.model, "key", condition.key);
                    return idx >= 0 ? idx : 0;
                }
                onActivated: {
                    if (conditionsModel) {
                        conditionsModel.setKey(conditionIndex, currentValue);
                    }
                }
            }

            ComboBox {
                id: opCombo
                Layout.preferredWidth: 170
                model: ruleController && ruleController.model ? ruleController.model.opOptions : []
                textRole: "label"
                valueRole: "value"
                currentIndex: {
                    var idx = findIndexByRole(opCombo.model, "value", roleOp);
                    return idx >= 0 ? idx : 0;
                }
                onActivated: {
                    if (conditionsModel) {
                        conditionsModel.setOp(conditionIndex, currentValue);
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                implicitHeight: valueStack.implicitHeight

                StackLayout {
                    id: valueStack
                    anchors.fill: parent
                    currentIndex: ruleController && ruleController.isListOp(roleOp) ? 1 : 0

                    TextField {
                        id: valueField
                        enabled: roleOp !== "$empty"
                        placeholderText: roleOp === "$empty" ? "无需填写" : "值"
                        Binding {
                            target: valueField
                            property: "text"
                            value: roleValueText || ""
                            when: !valueField.activeFocus
                            restoreMode: Binding.RestoreNone
                        }
                        onTextEdited: {
                            if (conditionsModel) {
                                conditionsModel.setValueText(conditionIndex, text);
                            }
                        }
                    }

                    // ColumnLayout {
                    //     spacing: theme.spacingXs

                    //     Repeater {
                    //         model: listModel
                    //         delegate: RowLayout {
                    //             spacing: theme.spacingXs
                    //             Layout.fillWidth: true
                    //             property int valueIndex: index

                    //             TextField {
                    //                 Layout.fillWidth: true
                    //                 placeholderText: "选项值"
                    //                 text: itemText
                    //                 onTextEdited: listModel.setItemText(valueIndex, text)
                    //             }

                    //             Button {
                    //                 text: "删除"
                    //                 onClicked: listModel.removeItem(valueIndex)
                    //             }
                    //         }
                    //     }

                    //     Button {
                    //         text: "添加选项"
                    //         onClicked: listModel.appendItem("")
                    //     }
                    // }
                }
            }

            Button {
                text: "移除"
                onClicked: {
                    if (conditionsModel) {
                        conditionsModel.removeCondition(conditionIndex);
                    }
                }
            }
        }
    }
}
