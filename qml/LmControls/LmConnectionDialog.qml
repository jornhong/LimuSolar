import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.1
import com.limutech.solar 1.0
import LmControls 1.0

LmDialog{
    id:wrapper
    property alias model:deviceList.model
    property alias title:titleStatus.text
    property string errorMsg:""

    signal scanError(var error)
    signal scanFinish()

    signal selected(var address,var device)

    state:"scanning"

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
                text:qsTr("Scanning devices....")
                font.pointSize: 13
                color:"#FFFFFF"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }
            AnimatedImage{
                id:scanStatusIcon
                playing:true
                Layout.preferredHeight: 16
                Layout.preferredWidth: 16
                source:"img/gifs/progress_hdpi.gif"
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
        Component{
            id:delegate
            Rectangle{
                id:delegateWrapper
                anchors.left: parent.left
                anchors.right:parent.right
                height:32
                property color highlightColor: "#48c9b0";
                ColumnLayout{
                    anchors.fill: parent
                    anchors.leftMargin:8
                    anchors.rightMargin:8
                    RowLayout{
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Text{
                            text:name
                            Layout.fillWidth: true
                            font.pointSize: 14
                            color:Material.primary
                            clip:true
                        }
                        Text{
                            text:"[" + address + "]"
                            Layout.alignment: Qt.AlignRight
                            color:"#6D6D6D"
                            font.pointSize: 11
                        }
                        Item{
                            width:4
                            Layout.fillHeight: true
                            Text{
                                //text:LmConstants.fontAwesome.fa_arrow_circle_o_right
                                font.pointSize: 18
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                color:Material.primary
                            }
                        }
                    }

                    Rectangle{
                        height:1
                        Layout.fillWidth: true
                        color:Material.primary
                    }
                }
                MouseArea{
                    id:area
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        wrapper.selected(address,device);
                    }
                }
                state: {
                    if(area.pressed)
                        "PRESSED"
                    else if(area.containsMouse)
                        "HOVERED"
                    else
                        ""
                }
                states:[
                    State{
                        name:""
                        PropertyChanges {
                            target: delegateWrapper
                            color:"#FFFFFF"
                        }
                    },
                    State{
                        name:"HOVERED"
                        PropertyChanges {
                            target: delegateWrapper
                            color:delegateWrapper.highlightColor
                        }
                    },
                    State{
                        name:"PRESSED"
                        PropertyChanges {
                            target: delegateWrapper
                            color:delegateWrapper.highlightColor
                        }
                    }

                ]
            }
        }
        ListView{
            id:deviceList
            clip:true
            spacing: 1
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            delegate: delegate
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
                text:qsTr("Cancel")
                Layout.fillWidth: true
                onClicked: {
                    wrapper.reject();
                }
            }
        }

        Item{
            Layout.preferredHeight:2
            Layout.fillWidth: true
        }
    }

    states:[
        State{
            name:"scanning"
            PropertyChanges {
                target: scanStatusIcon
                visible:true
            }
            PropertyChanges{
                target:titleStatus
                text:qsTr("Scanning devices...")
            }
        },
        State{
            name:"error"
            PropertyChanges{
                target:scanStatusIcon
                visible:false
            }
            PropertyChanges{
                target:titleStatus
                text:qsTr(qsTr("Bluetooth Error:") + errorMsg)
            }
        },
        State{
            name:"finish"
            PropertyChanges{
                target:scanStatusIcon
                visible:false
            }
            PropertyChanges{
                target:titleStatus
                text:qsTr("Scan Finished")
            }
        }

    ]

    onScanFinish: {
        wrapper.state = "finish";
    }

    onScanError:{
        wrapper.state = "error";
        switch(error)
        {
        case BluetoothDevicesModel.UnsupportedPlatformError:
            errorMsg = qsTr("Unsupported Platform")
            break;
        case BluetoothDevicesModel.PoweredOffError:
            errorMsg = qsTr("Bluetooth Power Off");
            break;
        case BluetoothDevicesModel.InvalidBluetoothAdapterError:
            errorMsg = qsTr("Invalid Bluetooth Adapter");
            break;
        case BluetoothDevicesModel.InputOutputError:
            errorMsg = qsTr("I/O Error")
            break;
        default:
            errorMsg = qsTr("Unknow Error");
        }
    }

    Component.onCompleted: {
        model.onScanError.connect(dialogConnection.scanError);
        model.onScanFinish.connect(dialogConnection.scanFinish);


    }
}


