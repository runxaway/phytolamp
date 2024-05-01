// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getValues(){
    websocket.send("getValues");
}

function toggle() {
    var tog = document.getElementById("toggle").innerHTML;
    console.log(tog);
    if (tog == "Лампа выключена") {
        websocket.send("Лампа включена")
    } else if (tog == "Лампа включена") {
        websocket.send("Лампа выключена")
    }
}

function move() {
    var mov = document.getElementById("move").innerHTML;
    console.log(mov);
    if (mov == "Адаптация излучения OFF") {
        websocket.send("Адаптация излучения ON")
    } else if (mov == "Адаптация излучения ON") {
        websocket.send("Адаптация излучения OFF")
    }
}

setInterval(function updateSensorsData(){
    websocket.send("getReadings");
}, 15000);

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    getValues();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function updateSliderPWM(element) {
    var sliderNumber = element.id.charAt(element.id.length-1);
    var sliderValue = document.getElementById(element.id).value;
    document.getElementById("sliderValue"+sliderNumber).innerHTML = sliderValue;
    websocket.send(sliderNumber+"s"+sliderValue.toString());
}


function onMessage(event) {
    console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);
    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        console.log(myObj[key]);
        // document.getElementById(key).innerHTML = myObj[key];
        // document.getElementById("slider"+ (i+1).toString()).value = myObj[key];
        document.getElementById(key).innerHTML = myObj[key];
        if (key == "humidity") {
            document.getElementById(key).innerHTML = myObj[key];
            // document.getElementById("humidity").value = myObj[key];
        } else if (key == "temperature") {
            document.getElementById(key).innerHTML = myObj[key];
            // document.getElementById("temperature").value = myObj[key];
        } else if (myObj[key] == "Лампа выключена") {
            document.getElementById(key).innerHTML = myObj[key];
            document.getElementById("Toggle-rounded").checked = false;
            document.getElementById("slider1").setAttribute("disabled", "");
            document.getElementById("slider2").setAttribute("disabled", "");
            document.getElementById("slider3").setAttribute("disabled", "");
            document.getElementById("slider4").setAttribute("disabled", "");
            document.getElementById("slider5").setAttribute("disabled", "");
            document.getElementById("Toggle-move").setAttribute("disabled", "");
            // document.getElementById("temperature").value = myObj[key];
        } else if (myObj[key] == "Лампа включена") {
            document.getElementById(key).innerHTML = myObj[key];
            document.getElementById("Toggle-rounded").checked = true;
            document.getElementById("slider1").removeAttribute("disabled");
            document.getElementById("slider2").removeAttribute("disabled");
            document.getElementById("slider3").removeAttribute("disabled");
            document.getElementById("slider4").removeAttribute("disabled");
            document.getElementById("slider5").removeAttribute("disabled");
            document.getElementById("Toggle-move").removeAttribute("disabled");
            // document.getElementById("temperature").value = myObj[key];
        } else if (myObj[key] == "Адаптация излучения OFF") {
            document.getElementById("Toggle-move").checked = false;
            if (document.getElementById("Toggle-rounded").checked == true) {
                document.getElementById(key).innerHTML = myObj[key];
                document.getElementById("slider3").removeAttribute("disabled");
            }
        } else if (myObj[key] == "Адаптация излучения ON") {
            document.getElementById("Toggle-move").checked = true;
            document.getElementById(key).innerHTML = myObj[key];
            document.getElementById("slider3").setAttribute("disabled", "");
        } else {
            document.getElementById(key).innerHTML = myObj[key];
            document.getElementById("slider"+ (i+1).toString()).value = myObj[key];
        }
    }
}