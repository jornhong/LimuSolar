import QtQuick 2.6
import com.limutech.solar 1.0
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import LmControls 1.0

LmDialog{
    id:wrapper
    property alias title:titleStatus.text
    property string errorMsg:""

    titleItem: Rectangle{
        //color:lmConstants.wetAsphalt
        color:Material.primary
        anchors.fill: parent
        RowLayout{
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin:8
            anchors.rightMargin: 8
            Text{
                id:titleStatus
                anchors.verticalCenter: parent.verticalCenter
                text:qsTr("Save Current Settings")
                font.pointSize: 13
                color:"#FFFFFF"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    ColumnLayout{
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.leftMargin:4
        anchors.rightMargin: 4
        anchors.topMargin: 4
        spacing: 2
        Item{
            height:2
        }
        Item{
            clip:true
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Text{
                anchors.fill: parent
                text:qsTr("Language Changed,You Must Restart App To Take Effects!")
                font.pointSize: 12
                wrapMode: Text.WordWrap

            }
        }

        Rectangle{
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color:Material.primary
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight:childrenRect.height
            Button{
                text:qsTr("OK")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.reject();
                }
            }
            /*Button{
                text:qsTr("Restart")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.accept();
                }
            }*/
        }

        Item{
            Layout.preferredHeight:2
            Layout.fillWidth: true
        }
    }

    Component.onCompleted: {

    }
}
