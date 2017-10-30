import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Controls.Styles 1.4

SpinBox{
    id:control
    property bool valid:true
    implicitHeight: 32
    property real realValue: value/10.0
    font.pointSize: 16
    decimals: 2
    style:SpinBoxStyle{
        background:Item{

        }
        textColor:"#222222"
        selectedTextColor: "#FFFFFF"
        selectionColor: "#00D0D0"
    }

    Component.onCompleted: {
        if(!control.valid)
        {
            control.enabled = false;
            control.editable = false;
        }
    }
}
