import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import LmControls 1.0

TabView{
    id:wrapper
    property bool showFrame:false
    property color frameColor:"#FFFFFF"

    style: TabViewStyle {
        id: tabStyle
        frameOverlap: 0
        tab: Rectangle {
            color: styleData.selected ? "#FFFFFF" :"#009999"
            border.color:  "steelblue"
            implicitHeight: 48
            implicitWidth: tabStyle.control.width / tabStyle.control.count + 1

            radius: 2
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: styleData.selected ? "#00CCCC" : "#FFFFFF"
                font.pointSize: 14
            }
        }
        frame: Rectangle {
            //visible: wrapper.showFrame
            height:1
            color:wrapper.frameColor
        }
        leftCorner: Rectangle { color: "cyan" }
    }
}
