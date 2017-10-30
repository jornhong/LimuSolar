import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

Rectangle {
    implicitWidth: 640
    implicitHeight: 960
    ColumnLayout{
        anchors.fill: parent
        Image{
            source:"images/logo.png"
            Layout.preferredWidth: 128
            Layout.preferredHeight: 128
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 48
        }
        Text{
            Layout.alignment: Qt.AlignHCenter
            text:"LimuSolar"
            font.pointSize: 24
            color:"#333333"
            font.bold: true
        }

        Text{
            Layout.alignment: Qt.AlignHCenter
            text:"V"+jsExt.getVersionName()
            font.pointSize: 12
            color:"#888888"
        }
        Item{
            Layout.preferredHeight: 32
            Layout.preferredWidth: 1
        }
        Text{
            Layout.alignment: Qt.AlignHCenter
            text:qsTr("Solar Controller Manager For Mobile Phone")
            font.pointSize: 12
            color:"#888888"
        }

        Item{
            Layout.preferredHeight: 32
            Layout.preferredWidth: 1
        }

        RowLayout{
            id:layoutCheckUpgrade
            Layout.preferredHeight: 48
            Layout.alignment: Qt.AlignHCenter
            visible: false
            Image{
                id:imageInfo
                visible: false
                source:"images/info.png"
                Layout.preferredHeight: 24
                Layout.preferredWidth: 24
            }

            AnimatedImage{
                id:imageLoading
                source:"images/loading.gif"
                Layout.preferredHeight: 24
                Layout.preferredWidth: 24
                playing: true
            }

            Text{
                id:upgradeInfo
                text:qsTr("Checking Upgrade ...")
                font.pointSize: 16
                color:"#888888"
            }
        }
        Column{
            Layout.alignment: Qt.AlignHCenter
            Button{
                id:btnDetail
                visible: false
                text:qsTr("Detail")
                onClicked: {
                    Qt.openUrlExternally("http://fir.im/4par")
                }
            }
        }

        Item{
            Layout.fillHeight: true
        }
        Text{
            Layout.alignment: Qt.AlignHCenter
            text:"<html><a href='http://www.limutech.com/solarmobile'>www.limutech.com/solarmobile</a></html>"
            font.pointSize: 14
            color:"#3333EE"
            Layout.bottomMargin: 64
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    function requestUpdate(searchKey)
    {
        var doc = new XMLHttpRequest();
        doc.open("GET","http://api.fir.im/apps/latest/5808c1baca87a86d45000abe?api_token=4f2dd240322901eb65b000a3b16f5e34");
        doc.send();
        doc.onreadystatechange = function() {
            switch(doc.readyState)
            {
            case XMLHttpRequest.DONE:
                console.log("request complete:");
                console.log("status=" + doc.status);
                imageLoading.visible = false;
                imageInfo.visible = true;
                if(doc.status === 200)
                {
                    var jsonobj = eval(doc.responseText);
                    var newVersion = parseInt(jsonobj.version);
                    console.log("version ==== " + jsonobj.version);
                    if(newVersion > jsExt.getVersion())
                    {
                        console.log("!!!!!!!!!!!!!!!!need upgrade!");
                        upgradeInfo.text = qsTr("New Upgrade Avalible")
                        btnDetail.visible = true;
                    }else{
                        console.log("..............dont need upgrade!");
                        upgradeInfo.text = qsTr("No Upgrade Avalible")
                    }
                }else{
                        upgradeInfo.text = qsTr("Check Upgrade Failure")
                }

                break;
            }
        }
    }

    Timer{
        id:timerUpgrade
        interval: 1000
        running: false
        repeat: false
        onTriggered: {
            requestUpdate("hello");
        }
    }

    Component.onCompleted: {
        console.log("About page complete!");
        //timerUpgrade.running = true;
    }
}
