// Dialog to preview rule JSON.
import QtQuick
import QtQuick.Controls

Dialog {
    id: root

    property string previewText: ""
    property var theme

    modal: true
    title: "规则数据预览"
    standardButtons: Dialog.NoButton
    width: 560
    height: 380

    contentItem: TextArea {
        text: root.previewText
        readOnly: true
        wrapMode: TextEdit.Wrap
        font.family: theme ? theme.fontBody : "Microsoft YaHei UI"
        color: theme ? theme.textPrimary : "#1b242b"
    }

    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        Button {
            text: "关闭"
            onClicked: root.close()
        }
    }
}
