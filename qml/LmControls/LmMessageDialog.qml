import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import com.limutech.solar 1.0

LmDialog{
    id:wrapper
    property bool showButtons:false
    property string message:""
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
        Item{
            Layout.fillWidth: true
            height:childrenRect.height

            Text{
                text:wrapper.message
                color:Material.primary
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }

        Item{
            height:8
        }

        Rectangle{
            visible:wrapper.showButtons
            height:1
            Layout.fillWidth: true
            color:Material.primary
        }
        RowLayout{
            visible: wrapper.showButtons
            Layout.fillWidth: true
            height:childrenRect.height
            spacing: 4
            Button{
                text:qsTr("OK")
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
