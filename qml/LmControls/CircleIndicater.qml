import QtQuick 2.6
import QtGraphicalEffects 1.0

Item {
    id:container
    width: 100
    height: 100
    property string unit:""
    property Component icon:null
    //property real startAngle:(2*Math.PI)/360*(140)
    property real startAngle:0
    property real endAngle:260
    property real maxValue:48.0
    property real value:24
    property real frameRadius:0
    property point canvasCenter//:(0,0)
    property list<Area> areas
    property color masterIndicaterColor:"#222222"
    property color subIndicaterColor:"#FF0000"

    /*Rectangle{
        anchors.fill: parent
    }*/
    Canvas{
        id:canvasIndicater
        anchors.fill: parent
        rotation: 140
        onWidthChanged: {
            requestPaint();
        }

        onPaint:{
            var ctx = getContext("2d")
            ctx.strokeStyle = "#808080"
            ctx.lineWidth = 1
            ctx.textAlign = "center"
            for(var i=0;i<50;i=i+1)
            {
                ctx.beginPath();
                var angle = calcAngle(i);
                var x = (frameRadius-14)* Math.cos(angle) + canvasCenter.x;
                var y = (frameRadius-14)*Math.sin(angle) + canvasCenter.y;
                if((i%5)==0)
                {
                    var x0 = (frameRadius-22)* Math.cos(angle) + canvasCenter.x;
                    var y0 = (frameRadius-22)*Math.sin(angle) + canvasCenter.y;
                    //ctx.strokeStyle = "#000000"
                    ctx.strokeStyle = masterIndicaterColor;
                }else{
                    var x0 = (frameRadius-18)* Math.cos(angle) + canvasCenter.x;
                    var y0 = (frameRadius-18)*Math.sin(angle) + canvasCenter.y;
                    ctx.strokeStyle = subIndicaterColor;

                }

                if((i%5)==0)
                {
                    //ctx.text(i,xt,yt);
                }




                //ctx.text(".",x,y);
                ctx.moveTo(x0,y0);
                ctx.lineTo(x,y);
                ctx.stroke();
            }

        }
    }
    Canvas{
        id:canvas1
        anchors.fill: parent
        opacity: 0.8
        rotation: 140
        onPaint:{
            var ctx = getContext("2d")
            ctx.strokeStyle = "#808080"
            ctx.lineWidth = 2
            //ctx.lineCap = "round"
            ctx.beginPath();
            ctx.arc(width/2,height/2,frameRadius-8,0,2*Math.PI,false);
            //ctx.arc(width/2,height/2,30,0,2*Math.PI,false);
            ctx.stroke();
            ctx.lineWidth = 7
            for(var area in areas)
            {
                ctx.beginPath();
                ctx.strokeStyle = areas[area].color
                ctx.arc(width/2,height/2,frameRadius - 35,calcAngle(areas[area].from),calcAngle(areas[area].to),false);
                ctx.stroke();
            }
        }
    }

    Image{
        id:imagePointer
        visible: false
        source:"img/pointer.png"
        anchors.horizontalCenter:container.horizontalCenter
        anchors.verticalCenter: container.verticalCenter
    }
    DropShadow{
        anchors.fill: imagePointer
        horizontalOffset: 2
        verticalOffset: 2
        radius: 12.0
        samples: 22
        color: "black"
        source: imagePointer
        Behavior on rotation{
            SequentialAnimation{
                NumberAnimation{ easing.overshoot: 2.052; easing.type: Easing.OutBack;duration: 300}
            }
        }
        rotation : 260/maxValue*value - 40;
    }

    Text{
        id:valueText
        color:"#808080"
        anchors.horizontalCenter: parent.horizontalCenter
        y:parent.height/2 + parent.height/2 -16*2 - height/2
        font.pointSize: 14
        font.bold: true
        text:value.toFixed(2) + " " + unit
    }
    function calcAngle(v){
        //return (value / maxValue)*(endAngle - startAngle);
        return ((260/360)*2*Math.PI/maxValue)*v + startAngle;
    }
    /*Timer{
        id:timerTest
        interval: 500
        repeat: true
        onTriggered: {
            value = Math.random() * maxValue;
        }
    }*/
    onValueChanged: {
    }
    function cha()
    {
        canvas1.requestPaint();
    }

    onWidthChanged: {
        canvasIndicater.requestPaint();
        canvas1.requestPaint();

    }
    onHeightChanged: {
        canvasIndicater.requestPaint();
        canvas1.requestPaint();
    }

    Component.onCompleted: {
        //timerTest.start();
        for(var area in areas)
        {
            areas[area].onToChanged.connect(cha);
        }
        frameRadius = width < height?width/2:height/2
        canvasCenter.x = width/2
        canvasCenter.y = height/2

        //icon.createObject(container,{"anchors.horizontalCenter":container.horizontalCenter,"anchors.verticalCenter":container.verticalCenter,"width":48,"height":48});
    }
}



