import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import com.limutech.solar 1.0
import QtQuick.Controls.Material 2.0

LmDialog{
    id:wrapper
    property var statics
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
            GridLayout{
                anchors.left:parent.left
                anchors.right: parent.right
                anchors.leftMargin:16
                anchors.rightMargin: 16
                height:childrenRect.height
                rows:6
                columns:2
                columnSpacing: 8
                Label{
                    text:qsTr("Session:")
                    color:Material.foreground
                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionName
                    Layout.fillWidth: true
                    color:Material.foreground
                }
                Label{
                    text:qsTr("Length:")
                    color:Material.foreground
                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionLength
                    Layout.fillWidth: true
                    color:Material.foreground
                }

                Label{
                    text:qsTr("Tx:")
                    color:Material.foreground
                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionTxSize
                    Layout.fillWidth: true
                    color:Material.foreground
                }
                Label{
                    text:qsTr("Rx:")
                    color:Material.foreground
                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionRxSize
                    Layout.fillWidth: true
                    color:Material.foreground

                }
                Label{
                    text:qsTr("Rx Error:")
                    color:Material.foreground

                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionRxError
                    Layout.fillWidth: true
                    color:Material.foreground

                }
                Label{
                    text:qsTr("Tx Error:")
                    color:Material.foreground

                    Layout.alignment: Qt.AlignRight
                }
                Text{
                    id:textSessionTxError
                    Layout.fillWidth: true
                    color:Material.foreground

                }

            }
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
                    wrapper.reject();
                }
            }
            Button{
                text:qsTr("Disconnect")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.accept();
                }
            }
        }

        Item{
            height:1
            Layout.fillWidth: true
        }

        Timer{
            running:wrapper.visible
            interval: 100
            repeat:true
            onTriggered: {
                if(wrapper.statics)
                {
                    textSessionName.text = statics["session_name"];
                    textSessionLength.text = jsExt.formatMSecToTimeFormat(new Date().getTime() -  statics["session_time"].getTime());
                    textSessionRxSize.text = statics["session_rx_size"] + " bytes" + "/" + statics["session_rx"] + " frame";
                    textSessionTxSize.text = statics["session_tx_size"] + " bytes" + "/" + statics["session_tx"] + " frame";
                    textSessionRxError.text = statics["session_rx_error"] + " frame";
                    textSessionTxError.text = statics["session_tx_error"] + " frame";
                }
            }
        }
    }
}
