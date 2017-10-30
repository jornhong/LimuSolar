import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import com.limutech.solar 1.0
//import QtGraphicalEffects 1.0
import LmControls 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

LmApplicationWindow {
    id:mainwindow
    visible: true
    width:640
    height:960
    visibility: Window.AutomaticVisibility

    property int bluetoothState:BluetoothConnection.CSDisconnected
    property alias mainTitle:mainTitle.text
    property alias subTitle:subTitle.text
    property int pluginProtocol:0

    signal action(var action)

    Keys.onPressed: {
        console.log(event.key);
    }

    Rectangle{
        anchors.fill: parent
        color:"#000000"
    }

    BluetoothDevicesModel{
        id:devicesModel
        onRunningChanged: {
            if(running)
            {
                dialogConnection.visible = true;
            }
        }
    }

    BluetoothConnection{
        id:bluetoothConnection
    }



    Component{
        id:com1
        Rectangle{
            property alias loader:mainLoader
            Loader{
                anchors.fill: parent
                id:mainLoader
            }
        }
    }
    Component{
        id:com2
        Rectangle{
            ListView{
                anchors.fill: parent
                model:jsExt.pluginsModel
                delegate:Rectangle{
                    width:parent.width
                    height:120
                    Text{
                        anchors.centerIn: parent
                        text:name + "-" + version
                        color:"#333333"
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            jsExt.pluginsModel.instance(index);
                            console.log("set loader source!");
                            stackView.get(0).loader.source = entry;
                            stackView.pop();
                        }
                    }
                }
            }
            Component.onCompleted: {
                console.log("com2 complete!");
            }
            Component.onDestruction: {
                console.log("com2 destroied!");
            }
        }
    }
    Component{
        id:componentAbout
        About{

        }
    }

    Component{
        id:componentOptions
        Option{

        }
    }

    ColumnLayout{
        anchors.fill: parent
        spacing:0

        Rectangle{
            id:navBar
            Layout.preferredHeight: 40
            Layout.fillWidth: true
            color:mainwindow.visibility == Window.FullScreen ?"#000000":"#0099FF"
            Layout.alignment: Qt.AlignTop
            RowLayout{
                anchors.fill: parent
                LmImageButton{
                    id:buttonMenu
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    Layout.alignment: Qt.AlignVCenter
                    normalSource: "images/menu.svg"
                    visible:(jsExt.stackDepth <= 0)&&(jsExt.mainStackDepth <=0)?true:false
                    onClicked: {
                        menu.open();
                    }
                    Menu {
                        id: menu
                        y: buttonMenu.height

                        MenuItem {
                            text: qsTr("Options")
                            onTriggered: {
                                stackView.push(componentOptions);
                            }
                        }
                        /*

                        MenuItem {
                            text: qsTr("Plugins")
                            onTriggered: {
                                stackView.push(com2);
                            }

                        }*/

                        MenuItem {
                            text: qsTr("About")
                            onTriggered: {
                                //stackView.push(com2);
                                stackView.push(componentAbout);
                            }

                        }
                        MenuItem {
                            text: qsTr("Quit")
                            onTriggered: {
                                Qt.quit();
                            }
                        }
                    }

                }
                LmImageButton{
                    id:buttonBack
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    Layout.alignment: Qt.AlignVCenter
                    normalSource: "images/privous.svg"
                    visible:(jsExt.stackDepth > 0)||(jsExt.mainStackDepth > 0)?true:false
                    onClicked: {
                        jsExt.postAction(NavAction.NVKey_Back)
                    }
                }
                Item{
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    ColumnLayout{
                        anchors.centerIn: parent
                        spacing:0
                        Text{
                            id:mainTitle
                            Layout.columnSpan: 2
                            //text:"LTC2410RP"
                            color:"#FFFFFF"
                            font.pointSize: 18
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text{
                            id:subTitle
                            text:"ID:N/A"
                            color:"#FFFFFF"
                            font.pointSize: 14
                            Layout.alignment: Qt.AlignHCenter
                            visible: false
                        }
                    }
                }
                LmImageButton{
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    normalSource: bluetoothConnection.connected?"images/bluetooth.svg":"images/bluetooth_disconnect.svg"
                    Layout.alignment: Qt.AlignRight
                    onClicked:{
                        if(bluetoothConnection.connected)
                        {
                            dialogConnectionStatus.visible = true;
                            /*var uartData=[];
                            uartData[0] = 0x20;
                            uartData[1] = 0x03;
                            uartData[2] = 0x0A;
                            uartData[3] = 0x0A;
                            uartData[4] = 0x0A;
                            bluetoothConnection.sendData(uartData);*/
                        }else{
                            dialogConnection.state = "scanning";
                            devicesModel.running = true;
                        }
                    }
                }
            }
        }

        StackView{
            id:stackView
            Layout.fillHeight: true
            Layout.fillWidth: true
            initialItem: com1
            onDepthChanged: {
                jsExt.mainStackDepth = stackView.depth - 1;
            }
        }

    }

    LmConfirmDialog{
        id:dialog4
        title:qsTr("Quit?")
        text:qsTr("Quit SolarMobile?")
        onAccept: {
            Qt.quit();
        }
        onReject: {
            visible = false;
        }
    }

    LmMessageDialog{
        id:messageDialog
        onReject: {
            //visible = false;
        }
    }

    LmConnectionStatusDialog{
        id:dialogConnectionStatus
        title:qsTr("Bluetooth Connected")
        statics:bluetoothConnection.statics
        onReject: {
            visible = false;
        }
        onAccept: {
            bluetoothConnection.disconnect();
            visible = false;
        }
    }

    ListModel{
        id:testModel

        ListElement{
            name:"Limutech solar"
            address:"32:A8:12:C6:99"
        }
        ListElement{
            name:"Limutech Tv"
            address:"32:A8:12:C6:99"
        }
    }

    Timer{
        id:delayConnectionTimer
        property var triggeredFunction
        repeat: false
        running:false
        interval: 500
        onTriggered: {
            //bluetoothConnection.connect(device);
            if(triggeredFunction)
            {
                triggeredFunction();
            }
        }
    }

    LmConnectionDialog{
        id:dialogConnection
        model:devicesModel
        //model:testModel

        onReject: {
            if(devicesModel.running)
            {
                devicesModel.running = false;
            }

            visible = false;
        }
        onSelected:{
            console.log("...............................................");
            console.log("connect to " + address);
            console.log("parsing device:" + device );
            reject();
            delayConnectionTimer.triggeredFunction = function(){
                bluetoothConnection.connect(device);
            };
            delayConnectionTimer.running = true;
        }
    }

    onAction:{
        console.log("action.accepted(main):" + action.accepted);
        menu.close();
        switch(action.action)
        {
        case NavAction.NV_Main_Privous:
            console.log("on nv main privous");
            if(stackView.depth > 1)
            {
                stackView.pop();
            }
            action.accepted = true;
            break;
        case NavAction.NVKey_Back:
            console.log("on nv key back");
            action.accepted = false;
            break;
        }
    }

    Timer{
        id:delayScan
        running:false
        interval: 500
        repeat: false
        onTriggered: {
            addDevice();
        }
    }
    function addDevice()
    {
        dialogConnection.state = "scanning";
        devicesModel.running = true;
    }


    function onConnectionStateChanged()
    {
        //console.log("...................on connection state changed..............");
        //console.log(bluetoothConnection.state);
        switch(bluetoothConnection.state)
        {
        case BluetoothConnection.CSConnecting:
            if(bluetoothState === BluetoothConnection.CSDisconnected)
            {
                messageDialog.title = qsTr("Connecting");
                messageDialog.message = qsTr("Connecting ") + bluetoothConnection.statics["session_name"];
                messageDialog.visible = true;
            }
            break;
        case BluetoothConnection.CSDiscoveringService:
            messageDialog.message = qsTr("Connecting to service...")
            break;
        case BluetoothConnection.CSServiceDiscovered:
            messageDialog.message = qsTr("Validing device...")
            break;
        case BluetoothConnection.CSHandsharked:
            delayConnectionTimer.triggeredFunction = function(){
                messageDialog.visible = false;
                jsExt.toast(qsTr("Connected to ") + bluetoothConnection.statics["session_name"]);
            };
            delayConnectionTimer.running = true;
            break;
        case BluetoothConnection.CSError:
            if(bluetoothConnection.lastError === BluetoothConnection.BluetoothDeviceInvalidError)
            {
                messageDialog.message ="[" + bluetoothConnection.statics["session_name"] + "]" + qsTr(" is not a valid device");
            }else{
                messageDialog.message = qsTr("Connection Error.")
            }
            delayConnectionTimer.triggeredFunction = function(){
                messageDialog.visible = false;
            };
            delayConnectionTimer.running = true;
            break;
        case BluetoothConnection.CSDisconnected:
            if(bluetoothState !== BluetoothConnection.CSDisconnected)
            {
                messageDialog.visible = false;
                jsExt.toast(qsTr("Disconnected from ") + bluetoothConnection.statics["session_name"]);
            }

            break;

        }
        bluetoothState = bluetoothConnection.state;
    }

    function onProtocolChanged(protocol)
    {
        console.log("Protocol changed to ....." + protocol);
        if(protocol != mainwindow.pluginProtocol)
        {
            loadPlugin(protocol,0);
        }

    }

    onClosing: {
        //Qt.quit();
    }

    Timer{
        interval: 100
        repeat:true
        running:true
        onTriggered: {
        }
    }

    function loadPlugin(protocol,modal)
    {

        //var index = jsExt.indexControllModal(protocol,modal);

        //if(index >= 0)
       // {
        //    jsExt.pluginsModel.instance(index);
         //   console.log("set loader source!");
            jsExt.selectLanguage();
            stackView.get(0).loader.setSource("/LMT_D/main.qml",{connection:bluetoothConnection});
            //jsExt.setSetting("LastProtocol",protocol);
            //jsExt.setSetting("LastModal",modal);
            mainwindow.pluginProtocol = protocol;
        //}else{
        //    console.log("not valid plugin found!");
       // }
    }

    Timer{
        id:testTimer
        interval: 10000
        running: false
        repeat: true
        property int times: 0
        onTriggered: {
            times++;
            if(times % 2)
            {
                loadPlugin(0xA0,0x00);
            }else{
                loadPlugin(0x01,0x00);
            }
        }
    }

    Component.onCompleted: {
        jsExt.appWin = mainwindow;
        jsExt.onActionEvent.connect(mainwindow.action);
        jsExt.loadPlugins();
        bluetoothConnection.onStateChanged.connect(onConnectionStateChanged);
        bluetoothConnection.onProtocolChanged.connect(onProtocolChanged);
        delayScan.running = true;
        var lastProtocol = jsExt.getSetting("LastProtocol") || 1;
        var lastModal = jsExt.getSetting("LastModal") || 0;
        //testTimer.running = true;
        loadPlugin(lastProtocol,lastModal);
    }
}
