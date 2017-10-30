import QtQuick 2.6
import QtQuick.Controls 2.0
import LmControls 1.0

ComboBox {
    id:control
    implicitWidth: 156
    implicitHeight: 38
    background: Rectangle{
        implicitWidth: control.width
        implicitHeight: control.height
        color:LMConstants.controlBackground
        border.width:LMConstants.borderWidth
        border.color: LMConstants.controlBorder
        radius:LMConstants.defaultRadius
    }

    contentItem: Text {
        leftPadding: control.mirrored && control.indicator ? control.indicator.width + control.spacing : 0
        rightPadding: !control.mirrored && control.indicator ? control.indicator.width + control.spacing : 0

        text: control.displayText
        font: control.font
        color: control.visualFocus ? "#0066ff" : "#353637"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
