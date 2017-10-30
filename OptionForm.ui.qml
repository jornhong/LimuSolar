import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle{
    implicitWidth:640
    implicitHeight:960
    clip:true
    id:wrapper

    property alias listViewModes:listViewModes
    property alias listViewLanguage:listViewLangs

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

    Flickable {
        id: item1
        anchors.fill: parent
        contentHeight:columnLayout1.height
        anchors.topMargin: 12
        ColumnLayout {
            id: columnLayout1
            spacing: 12
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.topMargin: 12

            GroupBox {
                id: groupBox1
                Layout.preferredHeight: 100
                Layout.fillHeight: false
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                title: "Language(语言)"
                Layout.leftMargin: 8
                Layout.rightMargin: 8

                ListView{
                    id:listViewLangs
                    anchors.fill: parent
                    model:langModel
                    orientation: ListView.LeftToRight
                    spacing: 12

                }

            }
            GroupBox {
                id: groupBox2
                Layout.fillWidth: true
                Layout.preferredHeight:listViewModes.height+48
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                title: qsTr("Mode")
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                ListView{
                    id:listViewModes
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height:childrenRect.height
                    model:langModel
                    spacing: 2
                }
            }
        }
    }
}
