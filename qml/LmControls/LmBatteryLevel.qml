import QtQuick 2.5
import QtQuick.Layouts 1.2

Item {
    id: wrapper
    width: 320
    height: 120
    clip:true
    property int value:0
    property bool flash:false
    Image{
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        source:"../../images/battery_frame.svg"
    }
    Item{
        anchors.fill: parent
        RowLayout{
            anchors.fill: parent
            anchors.leftMargin: parent.width*0.15
            anchors.rightMargin: parent.width*0.07
            spacing:parent.width*0.05
            Rectangle{
                id:rectLevel4
                Layout.fillWidth: true
                Layout.preferredHeight: wrapper.height*0.8
                radius: 6
                color:"#FFFFFF"
                /*Text{
                    text:wrapper.value
                }*/
                opacity: wrapper.value >= 4
            }
            Rectangle{
                id:rectLevel3
                Layout.fillWidth: true
                Layout.preferredHeight: wrapper.height*0.8
                radius:6
                color:"#FFFFFF"
                opacity: wrapper.value >= 3
            }
            Rectangle{
                id:rectLevel2
                Layout.fillWidth: true
                Layout.preferredHeight: wrapper.height*0.8
                radius:6
                color:"#FFFFFF"
                opacity: wrapper.value >= 2
            }
            Rectangle{
                id:rectLevel1
                Layout.fillWidth: true
                Layout.preferredHeight: wrapper.height*0.8
                radius:6
                color:wrapper.value > 0?"#FFFFFF":"#00FFFFFF"
                border.width: wrapper.value > 0?0:1
                border.color: "#FFFFFF"
                opacity: wrapper.value >= 0
            }
        }
    }
}
