import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as QtControls1
import com.limutech.solar 1.0

ApplicationWindow {
    id:mainwindow
    visible: true
    width:640
    height:960

    Rectangle{
        id:dialogMask
        anchors.fill: parent
        objectName: "dialogMask"
        z:1000
        color:"#000000"
        visible:false
        state:"closed"
        MouseArea{
            anchors.fill: parent
        }

        states: [
            State {
                name: "closed"
                PropertyChanges {
                    target:dialogMask
                    opacity: 0.0
                }
            },
            State {
                name: "visibled"
                PropertyChanges {
                    target: dialogMask
                    opacity: 0.5
                }
            }
        ]
        transitions: [
            Transition {
                from: "closed"
                to: "visibled"
                SequentialAnimation {
                    ScriptAction{script:(dialogMask.visible = true)}
                    PropertyAnimation{target:dialogMask;properties: "opacity"}
                }
            },
            Transition {
                from: "visibled"
                to: "closed"
                SequentialAnimation {
                    PropertyAnimation{target:dialogMask;properties: "opacity"}
                    ScriptAction{script:(dialogMask.visible = false)}
                }
            }
        ]

    }
    Item{
        objectName:"dialogArea"
        anchors.fill: parent
        z:10001
    }

    QtControls1.Action{
        shortcut: "back"
        onTriggered: {
            jsExt.postAction(NavAction.NVKey_Back);
        }
    }
    /*Action{
        shortcut:"F3"
        onTriggered:{
            jsExt.postAction(NavAction.NVKey_Back);
        }
    }*/

    /*Action{
        shortcut:"home"
        onTriggered:{
            jsExt.postAction(NavAction.NVKey_Home)
        }
    }
    Action{
        shortcut: "menu"
        onTriggered:{
            jsExt.postAction(NavAction.NVKey_Menu)
        }
    }*/

    onClosing:{
        close.accepted = false;
    }

}
