import QtQuick 2.6
//import QtQuick.Controls 2.0
import QtQuick.Controls 1.5
import QtQuick.Templates 2.0 as T
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Item{
    id:wrapper
    implicitWidth: 120
    implicitHeight: 36
    property alias value:spinbox.value
    property alias minimumValue:spinbox.minimumValue
    property alias maximumValue:spinbox.maximumValue
    property alias stepSize:spinbox.stepSize
    property real realValue: spinbox.value
    property bool editable:true
    property alias decimals:spinbox.decimals
    property alias suffix:spinbox.suffix
    property alias font:spinbox.font
    RowLayout{
        anchors.fill: parent
        spacing: 2
        Rectangle{
            id:downIndicator
            Layout.fillHeight: true
            Layout.preferredWidth: height
            color:Material.primary
            border.width: 1
            border.color: "#EEEFEF"
            opacity: mouseArea1.pressed?0.8:1.0
            radius:4
            Text {
                text: "-"
                font.pointSize: 24
                color: "#FFFFFF"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                id:mouseArea1
                anchors.fill: parent
                onClicked:{
                    spinbox.__decrement();
                }
            }
            Timer{
                running:true
                repeat:true
                interval: 100
                id:timerDecrement
                property string state:"release"
                property int pressDelay:0
                onTriggered: {
                    switch(state)
                    {
                    case "release":
                        if(mouseArea1.pressed)
                        {
                            if(++pressDelay >= 10)
                            {
                                state = "pressed";
                            }
                        }else{
                            pressDelay = 0;
                        }

                        break;
                    case "pressed":
                        if(!mouseArea1.pressed)
                        {
                            pressDelay = 0;
                            state = "release";
                        }else{
                            spinbox.__decrement();
                        }

                        break;
                    }
                }
            }
        }
        SpinBox{
            id:spinbox
            Layout.fillWidth: true
            Layout.fillHeight: true
            decimals:2
            font.pointSize: 12
            style:SpinBoxStyle{
                incrementControl: QtObject{}
                decrementControl: QtObject{}
            }
        }
        Rectangle{
            id:upIndicator
            Layout.fillHeight: true
            Layout.preferredWidth: height
            color:Material.primary
            border.width: 1
            border.color: "#EEEFEF"
            radius:4
            Text {
                text: "+"
                font.pointSize: 24
                color: "#FFFFFF"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                id:mouseArea2
                anchors.fill: parent
                onClicked:{
                    spinbox.__increment();
                }
            }
            Timer{
                running:true
                repeat:true
                interval: 100
                id:timerIncrement
                property string state:"release"
                property int pressDelay:0
                onTriggered: {
                    switch(state)
                    {
                    case "release":
                        if(mouseArea2.pressed)
                        {
                            if(++pressDelay >= 10)
                            {
                                state = "pressed";
                            }
                        }else{
                            pressDelay = 0;
                        }

                        break;
                    case "pressed":
                        if(!mouseArea2.pressed)
                        {
                            pressDelay = 0;
                            state = "release";
                        }else{
                            spinbox.__increment();
                        }

                        break;
                    }
                }
            }
        }

    }

}

/*T.SpinBox{
    id:control
    implicitWidth: 80
    implicitHeight: 32
    property int decimals: 2
    property real minimumValue:0
    property real maximumValue:100.0
    readonly property real realValue:value/100.0
    from:minimumValue*100
    to:maximumValue*100
    stepSize: decimals>0?10:100

    up.indicator: Rectangle{
        id:upIndicator
        implicitHeight: control.height
        implicitWidth: height
        x:parent.width-width
        color:Material.primary
        border.width: 1
        border.color: "#EEEFEF"
        radius:4
        Text {
            text: "+"
            font.pixelSize: control.font.pixelSize * 2
            color: "#FFFFFF"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    down.indicator: Rectangle{
        id:downIndicator
        implicitHeight: control.height
        implicitWidth: height
        x:0
        color:Material.primary
        border.width: 1
        border.color: "#EEEFEF"
        radius:4
        Text {
            text: "-"
            font.pixelSize: control.font.pixelSize * 2
            color: "#FFFFFF"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    contentItem:TextInput{
        z:2
        clip:true
        width: control.width-2*control.height
        height:control.height
        x:control.height
        id:textInput
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        font:control.font
        text: control.textFromValue(control.value, control.locale)
        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
    }

    Timer{
        running:false
        repeat:true
        interval: 1000
        onTriggered: {
            console.log("=====================================")
            console.log(control.value);
        }
    }


    validator: DoubleValidator {
        bottom: Math.min(control.from, control.to)
        top:  Math.max(control.from, control.to)
        decimals: control.decimals
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', control.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }
}*/
