// Form rendering and validation result display.
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import qt_demo

Item {
    id: root

    required property var p_formDefinition
    required property RuleController p_ruleController
    required property Theme p_theme

    readonly property string formTitle: p_formDefinition ? p_formDefinition.title : "表单"
    readonly property var formFields: p_formDefinition ? p_formDefinition.fields : []

    readonly property var formData: formController.formData ? formController.formData : undefined
    readonly property var evaluation: formController.evaluation ? formController.evaluation : undefined

    FormController {
        id: formController
        ruleController: root.p_ruleController
        formId: p_formDefinition ? p_formDefinition.id : ""
        fields: root.formFields
    }

    Component {
        id: textInputComponent
        TextField {
            property var field

            id: textField
            placeholderText: field.placeholder || ""
            Binding {
                target: textField
                property: "text"
                value: formData[field.key] === undefined ? "" : String(formData[field.key])
                when: !textField.activeFocus
                restoreMode: Binding.RestoreNone
            }
            onTextEdited: formController.setFieldText(field.key, text)
        }
    }

    Component {
        id: selectInputComponent

        ComboBox {
            property var field

            id: selectCombo
            model: field.options || []
            Binding {
                target: selectCombo
                property: "currentIndex"
                value: formController.optionIndex(field.options || [], formData[field.key])
                when: !selectCombo.activeFocus
                restoreMode: Binding.RestoreNone
            }
            onActivated: formController.setFieldValue(field.key, currentText)
        }
    }

    Component {
        id: numberInputComponent
        TextField {
            property var field

            id: numberField
            placeholderText: field.placeholder || ""
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            Binding {
                target: numberField
                property: "text"
                value: formData[field.key] === undefined ? "" : String(formData[field.key])
                when: !numberField.activeFocus
                restoreMode: Binding.RestoreNone
            }
            onTextEdited: formController.setFieldText(field.key, text)
        }
    }

    Component {
        id: boolInputComponent
        Switch {
            property var field

            checked: !!root.formData[field.key]
            text: checked ? "是" : "否"
            onToggled: formController.setFieldValue(field.key, checked)
        }
    }

    Component {
        id: multilineInputComponent
        TextArea {
            property var field

            id: textArea
            placeholderText: field.placeholder || ""
            wrapMode: TextEdit.Wrap
            Layout.preferredHeight: 90
            Binding {
                target: textArea
                property: "text"
                value: formData[field.key] === undefined ? "" : String(formData[field.key])
                when: !textArea.activeFocus
                restoreMode: Binding.RestoreNone
            }
            onTextChanged: {
                if (activeFocus) {
                    formController.setFieldText(field.key, text);
                }
            }
        }
    }

    // 表单主体
    ScrollView {
        anchors.fill: parent
        // contentWidth: container

        ColumnLayout {
            width: parent.width
            spacing: p_theme.spacingXl

            Label {
                text: root.formTitle
                font.family: p_theme.fontDisplay
                font.pixelSize: 24
                color: p_theme.textPrimary
            }

            Label {
                text: qsTr("%1").arg(JSON.stringify(formData))
            }

            Label {
                text: qsTr("%1").arg(JSON.stringify(p_formDefinition.fields))
            }

            Frame {
                Layout.fillWidth: true
                padding: p_theme.spacingMd
                background: Rectangle {
                    color: p_theme.panelMuted
                    radius: p_theme.radiusMd
                }

                ColumnLayout {
                    anchors.fill: parent
                    spacing: p_theme.spacingSm

                    Repeater {
                        model: root.formFields

                        RowLayout {
                            id: fieldRow
                            spacing: p_theme.spacingSm
                            Layout.fillWidth: true

                            required property var modelData

                            Label {
                                text: modelData.label
                                Layout.preferredWidth: p_theme.formLabelWidth
                                verticalAlignment: Text.AlignVCenter
                                color: p_theme.textPrimary
                            }

                            Loader {
                                id: fieldLoader
                                property var field: fieldRow.modelData

                                Layout.preferredWidth: p_theme.formInputWidth
                                Layout.maximumWidth: p_theme.formInputMaxWidth
                                sourceComponent: {
                                    if (field.type === "select") {
                                        return selectInputComponent;
                                    }
                                    if (field.type === "number") {
                                        return numberInputComponent;
                                    }
                                    if (field.type === "bool") {
                                        return boolInputComponent;
                                    }
                                    if (field.type === "multiline") {
                                        return multilineInputComponent;
                                    }
                                    return textInputComponent;
                                }
                            }
                            Binding {
                                target: fieldLoader.item
                                property: "field"
                                value: fieldLoader.field
                                when: fieldLoader.status === Loader.Ready
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: p_theme.spacingSm

                        Button {
                            text: "验证"
                            onClicked: formController.validate()
                        }

                        Button {
                            text: "重置"
                            onClicked: formController.resetForm()
                        }

                        Item { Layout.fillWidth: true }
                    }
                }
            }

            Frame {
                Layout.fillWidth: true
                padding: p_theme.spacingMd
                background: Rectangle {
                    color: p_theme.panelBackground
                    radius: p_theme.radiusMd
                    border.color: p_theme.borderColor
                }

                ColumnLayout {
                    anchors.fill: parent
                    spacing: p_theme.spacingSm

                    Label {
                        text: "验证结果"
                        font.bold: true
                        color: p_theme.textPrimary
                    }

                    Label {
                        text: evaluation.matchedRules.length > 0
                              ? ("命中规则数：" + evaluation.matchedRules.length)
                              : "未命中规则。"
                        color: evaluation.matchedRules.length > 0 ? p_theme.accent : p_theme.textSecondary
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.preferredHeight: Math.min(contentHeight, 220)
                        contentWidth: availableWidth

                        ColumnLayout {
                            width: parent.width
                            spacing: p_theme.spacingXs

                            Repeater {
                                model: evaluation.resultLabels
                                delegate: Label {
                                    text: modelData
                                    color: p_theme.textPrimary
                                }
                            }

                            Repeater {
                                model: evaluation.matchedRules
                                delegate: Label {
                                    text: {
                                        var resultCode = modelData["$result"];
                                        if (resultCode === undefined || resultCode === "") {
                                            resultCode = modelData.result || "";
                                        }
                                        return "规则：" + modelData.id + "，结果：" + resultCode;
                                    }
                                    color: p_theme.textSecondary
                                }
                            }
                        }
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
