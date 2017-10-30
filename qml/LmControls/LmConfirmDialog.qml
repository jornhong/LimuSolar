import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import LmControls 1.0

LmDialog{
    id:wrapper
    property alias text:textMsg.text
    ColumnLayout{
        anchors.top:parent.top
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.leftMargin:4
        anchors.rightMargin: 4
        anchors.topMargin: 4
        spacing: 8
        height:childrenRect.height
        Item{
            height:8
        }

        Text{
            id:textMsg
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 14
            color:Material.accent
        }
        Item{
            height:8
        }

        Rectangle{
            height:1
            Layout.fillWidth: true
            color:Material.primary
        }
        RowLayout{
            Layout.fillWidth: true
            height:childrenRect.height
            spacing: 4
            Button{
                text:qsTr("OK")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.accept();
                }

            }
            LmDefaultButton{
                text:qsTr("Cancel")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.reject();
                }

            }
        }

        Item{
            height:1
            Layout.fillWidth: true
        }
    }
}
