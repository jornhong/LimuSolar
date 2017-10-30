import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.0
import LmControls 1.0

Rectangle {
    id:root
    //implicitWidth: 100
    color:defaultColor
    property bool checked:false
    property color defaultColor: "#6d6d6d";
    property color pressColor: ;
    property color highlightColor: "#48c9b0";
    property color checkedColor: Material.primary

    property string text: "Button"
    property color textColor: "white";
    property color textPressColor: textColor
    property color textHighlightColor: textColor
    property color textCheckedColor: textColor
    readonly property color disabledTextColor: "#EAECED";

    property int pointSize: 16;

    signal clicked

    radius:6


    //    height:childrenRect.height+40
    height:64

    Text{
        anchors.centerIn: parent
        text:root.text
        font.pointSize:root.pointSize
        font.bold: true
        color: {
            if(!root.enabled)
                root.disabledTextColor
            else if(root.pressed)
                root.textPressColor
            else if(root.hovered)
                root.textHighlightColor
            else if(root.checked)
                root.textCheckedColor
            else
                root.textColor
        }
        Behavior on color { ColorAnimation {}}

    }

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        preventStealing: true

        onClicked: root.clicked()
        //        onEntered: tooltipTimer.start()
        //        onExited: tooltipTimer.stop()
    }

    state: {
        if(!enabled)
            ""
        else if(root.checked)
            "CHECKED"
        else if(area.pressed)
            "PRESSED"
        else if(area.containsMouse)
            "HOVERED"
        else
            ""
    }

    states: [
        State {
            name: ""
            when: !root.enabled
            PropertyChanges {
                target: root
                color: root.defaultColor
            }
        },

        State {
            name: "HOVERED"
            when: area.containsMouse && !area.pressed
            PropertyChanges {
                target: root
                color: root.highlightColor
            }
        },

        State {
            name: "PRESSED"
            when: area.pressed
            PropertyChanges {
                target: root
                color: root.pressColor
            }
        },

        State {
            name: "CHECKED"
            when: root.checked
            PropertyChanges {
                target: root
                color: root.checkedColor
            }
        }
    ]

    transitions: Transition {ColorAnimation { duration: 250 } }
    Component.onCompleted: {
        root.defaultColor = root.color
    }
}
