import QtQuick 2.4
import QtQuick.Controls 2.0
import LmControls 1.0

OptionForm {
    Component{
        id:languageDelegate
        RadioDelegate{
            text:label
            checked: ListView.view.currentIndex === index;
            onCheckedChanged: {
                if(checked)
                {
                    ListView.view.currentIndex = index;
                }
            }
        }
    }

    Component{
        id:modesDelegate
        RadioDelegate{
            text:descripts[jsExt.getCurrentLANG()]
            checked: ListView.view.currentIndex === index;
            font.pointSize: 12
            width:parent.width
            onCheckedChanged: {
                if(checked)
                {
                    ListView.view.currentIndex = index;
                }
            }
        }
    }

    listViewModes.model: jsExt.pluginsModel
    listViewLanguage.delegate:languageDelegate
    listViewModes.delegate: modesDelegate

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
        var lang = jsExt.getCurrentLANG();
        if(lang === "en")
        {
            listViewLanguage.currentIndex = 0;
        }
        if(lang === "cn")
        {
            listViewLanguage.currentIndex = 1;
        }

        var lastProtocol = jsExt.getSetting("LastProtocol") || 1;
        var lastModal = jsExt.getSetting("LastModal") || 0;
        var index = jsExt.indexControllModal(lastProtocol,lastModal);
        listViewModes.currentIndex = index;

        listViewLanguage.onCurrentIndexChanged.connect(function(){
            jsExt.selectLanguage(listViewLanguage.model.get(listViewLanguage.currentIndex).lang);
            delayShowDialog.running = true;
        });

        listViewModes.onCurrentIndexChanged.connect(function(){
            var lastProtocol = listViewModes.model.get(listViewModes.currentIndex).protocol;
            var lastModal = listViewModes.model.get(listViewModes.currentIndex).modals[0];
            jsExt.setSetting("LastProtocol",lastProtocol);
            jsExt.setSetting("LastModal",lastModal);
            delayShowDialog.running = true;
        }

        )
    }
}
