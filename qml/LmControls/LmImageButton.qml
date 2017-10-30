import QtQuick 2.6
import QtGraphicalEffects 1.0

Item {
    id:wrapper
    width: 82
    height: 64
    property url normalSource
    property url hoveredSource
    property url disableSource
    property alias hoverEnabled:mouseArea.hoverEnabled
    opacity: mouseArea.pressed?0.5:1.0

    state:"normal"
    property bool checked:false
    signal clicked
    focus:true

    Image{
        id:image
        source:normalSource
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        width:parent.width*0.9
        height:parent.height*0.9
    }
    MouseArea{
        id:mouseArea
        anchors.fill: parent
        hoverEnabled:true
    }
    Component.onCompleted: {
        mouseArea.onClicked.connect(wrapper.clicked);

    }
}
