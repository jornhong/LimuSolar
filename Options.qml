import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import LmControls 1.0

Rectangle {
    id:wrapper
    //    width:720
    //   height:1080

    ListModel{
        id:langModel
        ListElement{
            labelTranslated:qsTr("English")
            label:"English"
            lang:"en"
        }
        ListElement{
            labelTranslated:"简体中文"
            label:"简体中文"
            lang:"cn"
        }
    }

    ColumnLayout{
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.top:parent.top
        anchors.topMargin: 8
        anchors.bottom: parent.bottom


        Label{
            Layout.fillWidth: true
            text:{
                if(jsExt.getCurrentLANG()!=="en")
                    qsTr("Language") + "(Language)"
                else
                    "Language"
            }
            Layout.alignment: Qt.AlignTop
        }
        Rectangle{
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            color:"green"
        }

        ListView{
            id:listViewLangs
            Layout.fillHeight: true
            Layout.fillWidth: true
            model:langModel
            delegate: RadioDelegate{
                text:label
                checked: ListView.isCurrentItem
                onCheckedChanged: {
                    console.log(index);
                    listViewLangs.currentIndex = index;
                }
            }
            orientation: ListView.LeftToRight
            spacing: 12
            currentIndex: {
                var lang = jsExt.getCurrentLANG();
                if(lang === "en")
                {
                    return 0
                }
                if(lang === "cn")
                {
                    return 1;
                }
            }

        }

    }

    LmRestartDialog{
        id:dialogRestart
        title:qsTr("Restart Confirm")
        onReject: {
            visible = false;
        }
        onAccept: {
            visible = false;
            jsExt.restartApp();
        }
    }
    Timer{
        id:delayShowDialog
        interval: 500
        running:false
        repeat:false
        onTriggered: {
            dialogRestart.visible = true;
        }
    }

    Component.onCompleted: {
        listViewLangs.onCurrentIndexChanged.connect(function(){
            jsExt.selectLanguage(listViewLangs.model.get(listViewLangs.currentIndex).lang);
            delayShowDialog.running = true;

        }
        );

    }
}


