window.setInterval(async function(){
    var res = await axios.get('http://esp32server.ddns.net/data');
    var data = await res.data;
    if(data["temp"] != null){
        document.getElementById("temp").innerHTML = data["temp"] + "&#8451;";
    }
    if(data["hum"] != null){
        document.getElementById("hum").innerHTML = data["hum"] + "%";
    }
    if(data["light"] != null){
        document.getElementById("light").innerHTML = data["light"];
    }
    if(data["station1_data"] != null){
        document.getElementById("sta1_data").innerHTML = data["station1_data"] + "&#8451;";
    }
    if(data["switch_state"] != null){
        document.getElementById("switch-button").checked = data["switch_state"];
    }
    

    data["station1_signal"] = parseInt(data["station1_signal"]);
    if(data["station1_signal"] != null && data["station1_signal"] != 1){
        console.log(data["station1_signal"] >= -30)
        if(data["station1_signal"] >= -30){
            document.getElementById("bar").style.display = "none";
            document.getElementById("bar1").style.display = "none";
            document.getElementById("bar2").style.display = "none";
            document.getElementById("bar3").style.display = "none";
            document.getElementById("bar4").style.display = "none";
            document.getElementById("bar5").style.display = "flex";
            document.getElementById("bar-none").style.display = "none";
        }else if(data["station1_signal"] < -30 && data["station1_signal"] >= -67){
            document.getElementById("bar").style.display = "none";
            document.getElementById("bar1").style.display = "none";
            document.getElementById("bar2").style.display = "none";
            document.getElementById("bar3").style.display = "none";
            document.getElementById("bar4").style.display = "flex";
            document.getElementById("bar5").style.display = "none";
            document.getElementById("bar-none").style.display = "none";
        }else if(data["station1_signal"] < -67 && data["station1_signal"] >= -70){
            document.getElementById("bar").style.display = "none";
            document.getElementById("bar1").style.display = "none";
            document.getElementById("bar2").style.display = "none";
            document.getElementById("bar3").style.display = "flex";
            document.getElementById("bar4").style.display = "none";
            document.getElementById("bar5").style.display = "none";
            document.getElementById("bar-none").style.display = "none";
        }else if(data["station1_signal"] < -70 && data["station1_signal"] >= -80){
            document.getElementById("bar").style.display = "none";
            document.getElementById("bar1").style.display = "none";
            document.getElementById("bar2").style.display = "flex";
            document.getElementById("bar3").style.display = "none";
            document.getElementById("bar4").style.display = "none";
            document.getElementById("bar5").style.display = "none";
            document.getElementById("bar-none").style.display = "none";
        }
        else if(data["station1_signal"] < -80 && data["station1_signal"] >= -90){
            document.getElementById("bar").style.display = "none";
            document.getElementById("bar1").style.display = "flex";
            document.getElementById("bar2").style.display = "none";
            document.getElementById("bar3").style.display = "none";
            document.getElementById("bar4").style.display = "none";
            document.getElementById("bar5").style.display = "none";
            document.getElementById("bar-none").style.display = "none";
        }
        else if(data["station1_signal"] < -90){
            document.getElementById("bar").style.display = "flex";
            document.getElementById("bar1").style.display = "none";
            document.getElementById("bar2").style.display = "none";
            document.getElementById("bar3").style.display = "none";
            document.getElementById("bar4").style.display = "none";
            document.getElementById("bar5").style.display = "none";
            document.getElementById("bar-none").style.display = "none";
        }
        
        //document.getElementById("sta1_sig").innerHTML = data["station1_signal"];
    }else{
        document.getElementById("bar").style.display = "none";
        document.getElementById("bar1").style.display = "none";
        document.getElementById("bar2").style.display = "none";
        document.getElementById("bar3").style.display = "none";
        document.getElementById("bar4").style.display = "none";
        document.getElementById("bar5").style.display = "none";
        document.getElementById("bar-none").style.display = "flex";
        document.getElementById("sta1_data").innerHTML = "Stanica nije u dometu...";
    }
}, 3000);


async function updateSwitch(checkbox){
    await axios.post(`http://esp32server.ddns.net/data?value=${checkbox.checked}`,{});
    //console.log(res);
}

window.onload = function () {
   
    var isObjectEqual = function(object1, object2){
        return object1.r === object2.r && object1.g === object2.g && object1.b === object2.b && object1.a === object2.a
    }

    var colorWheel = new iro.ColorPicker("#colorWheelDemo", {
        wheelLightness: true
    });

    var _lastValue = colorWheel.color.rgba;

    document.getElementById("colorWheelDemo").onclick = function(){
        console.log(_lastValue, colorWheel.color.rgba)
        if(!isObjectEqual(_lastValue, colorWheel.color.rgba)){
            updateRGB()
            _lastValue = colorWheel.color.rgba;
        }
    }

    async function updateRGB(){
        var myColor = colorWheel.color.rgba;
        let colorObj = {"r": myColor.r.toString(),
                    "g": myColor.g.toString(),
                    "b": myColor.b.toString(),
                    "a": colorWheel.color.value.toString()
                }
        let objectStr = JSON.stringify(colorObj)
        await axios.post(`http://esp32server.ddns.net/rgb?value=${objectStr}`,{});
    }
    
    var chart = new CanvasJS.Chart("chartContainer", {
        animationEnabled: false,  
        title:{
            text: "Temperatura sobe"
        },
        axisY: {
            title: "Temperatura",
            valueFormatString: "#0.",
            suffix: "C°",
            minimum: 0,
            maximum: 40,
            stripLines: [{
                value: 23,
                label: "Prosjek"
            }]
        },
        axisX: {
            intervalType: "minute",
            valueFormatString: "HH:mm:ss",
            labelMaxWidth: 100, // change label width accordingly
        },
        data: [{
            yValueFormatString: "#,### C°",
            xValueFormatString: "HH:mm:ss",
            type: "spline",
            dataPoints: []
        }],
        
    });
    chart.render();

    setInterval(function(){
        updateChart(chart);
        //updateRGB();
        chart.render();
    },1000);
}
async function updateChart(chart){
    let dataP = new Array;
    var res = await axios.get(`http://esp32server.ddns.net/data_graph`);
    //console.log(res.data);
    let strArr = res.data.split("\n");
    avg = 0;
    strArr.forEach(e => {
        //console.log(e);
        if(e != ""){
            let d = JSON.parse(e);
            if(d.x != ""){
                dataP.push({
                    x: new Date("01 Jan 1970 " + d.x + " GMT+2"),
                    y: d.y
                })
                avg += d.y;
            }
        }  
    });
    chart.options.data[0].dataPoints = dataP;
    chart.options.axisY.stripLines[0].value = avg;
    //console.log(chart.options.data[0].dataPoints);
}