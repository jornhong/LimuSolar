import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import LmControls 1.0
import QtQuick.Controls.Material 2.0

Rectangle {
    id:wrapper
    visible:false
    implicitWidth: parent.width*0.8
    implicitHeight: childrenRect.height
    default property alias contents: contentWrapper.children
    property alias titleItem:rectTitle.children
    property alias title:textTitle.text
    //readonly property alias lmConstants:LmConstants
    anchors.centerIn: parent


    signal close()
    signal accept()
    signal reject()

    radius: 8

    Rectangle{
        id:rectTitle
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right: parent.right
        height:32
        color:Material.accent
        RowLayout{
            anchors.fill: parent
            Text{
                id:textTitle
                anchors.centerIn: parent
                font.pointSize: 16
                color:"#FFFFFF"
                font.bold: true
            }
        }

    }

    Rectangle{
        id:contentWrapper
        anchors.top:rectTitle.bottom
        anchors.left:parent.left
        anchors.right: parent.right
        height:childrenRect.height
    }

    onVisibleChanged: {
        if(visible)
        {
            jsExt.showDialog(wrapper);
        }else{
            jsExt.closeDialog();
        }
    }

    Component.onCompleted: {
    }
    Component.onDestruction: {
    }
}


