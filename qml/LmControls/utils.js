Date.prototype.format = function(fmt)
{ //author: meizz
    var o = {
        "M+" : this.getMonth()+1,                 //月份
        "d+" : this.getDate(),                    //日
        "h+" : this.getHours(),                   //小时
        "m+" : this.getMinutes(),                 //分
        "s+" : this.getSeconds(),                 //秒
        "q+" : Math.floor((this.getMonth()+3)/3), //季度
        "S"  : this.getMilliseconds()             //毫秒
    };
    if(/(y+)/.test(fmt))
        fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));
    for(var k in o)
        if(new RegExp("("+ k +")").test(fmt))
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));
    return fmt;
}

function fixNumber(n,l)
{
    var strn = n + "";
    var rn =l - strn.length;
    for(var i=0;i<rn;i++)
    {
        strn = "0" + strn;
    }
    return strn;
}
function decimalToHex(d, padding) {
    var hex = Number(d).toString(16);
    padding = typeof (padding) === "undefined" || padding === null ? padding = 2 : padding;

    while (hex.length < padding) {
        hex = "0" + hex;
    }
    return hex;
}

function requestManufacture(code,func)
{
    var doc = new XMLHttpRequest();
    doc.onreadystatechange = function() {
        console.log("on xml request state changed:" + doc.readyState);
        console.log("state code:" + doc.status);
        switch(doc.readyState)
        {
        case XMLHttpRequest.DONE:
            if(doc.status == 200)
            {
                func(doc.responseText);
            }
            break;
        }
    }

    doc.open("GET", "http://www.limutech.com/");
    doc.send();
}
