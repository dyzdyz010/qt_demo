import QtQuick

QtObject {
    // Shared theme tokens for the demo UI (keep it small and predictable).
    readonly property string fontDisplay: "Microsoft YaHei UI"
    readonly property string fontBody: "Microsoft YaHei UI"

    readonly property color appBackground: "#eef1f4"
    readonly property color panelBackground: "#ffffff"
    readonly property color panelMuted: "#f4f6f8"
    readonly property color borderColor: "#d7dbe0"
    readonly property color navBackground: "#1f2933"
    readonly property color navText: "#dce3e8"
    readonly property color navTextActive: "#1f2933"
    readonly property color navHighlight: "#fbd38d"
    readonly property color listHighlight: "#ffd166"
    readonly property color textPrimary: "#1a202c"
    readonly property color textSecondary: "#666666"
    readonly property color accent: "#2b6cb0"

    readonly property int navWidth: 240
    readonly property int headerHeight: 52

    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 12
    readonly property int spacingLg: 16
    readonly property int spacingXl: 20

    readonly property int radiusSm: 6
    readonly property int radiusMd: 8

    readonly property int formLabelWidth: 120
    readonly property int formInputWidth: 360
    readonly property int formInputMaxWidth: 420
}
