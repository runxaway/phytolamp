#include <Arduino.h>
#include "WiFi.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTPIN 23 //here we use pin IO14 of ESP32 to read data
#define DHTTYPE DHT11 //our sensor is DHT22 type


// Replace with your network credentials
const char* ssid = "TP-Link_B763";
const char* password = "26644554";

// const char* ssid = "MegaFonMR150-7_E093";
// const char* password = "70774332";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create a WebSocket object

AsyncWebSocket ws("/ws");

// unsigned long lastTime = 0;
// unsigned long timerDelay = 3000;

DHT dht(DHTPIN, DHTTYPE);

// Set LED GPIO
const int ledPin1 = 25;
const int ledPin2 = 26;
const int ledPin3 = 27;
const int ledPin4 = 14;
const int ledPin5 = 12;

String message = "";
String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";
String sliderValue4 = "0";
String sliderValue5 = "0";
String toggleoff = "Лампа выключена";
String toggleon = "Лампа включена";
String toggle = "Лампа выключена";

int dutyCycle1;
int dutyCycle2;
int dutyCycle3;
int dutyCycle4;
int dutyCycle5;

// setting PWM properties
const int freq = 5000;
const int ledChannel1 = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int ledChannel4 = 3;
const int ledChannel5 = 4;

const int resolution = 8;

// JSONVar readings;

// String getSensorReadings() {
//   readings["temperature"] = String(dht.readTemperature());
//   readings["humidity"] =  String(dht.readHumidity());
  
//   String jsonStringDHT = JSON.stringify(readings);
//   return jsonStringDHT;
// }

//Json Variable to Hold Slider Values
JSONVar sliderValues;

// Get Slider Values
String getSliderValues(String m) {
  sliderValues["sliderValue1"] = String(sliderValue1);
  sliderValues["sliderValue2"] = String(sliderValue2);
  sliderValues["sliderValue3"] = String(sliderValue3);
  sliderValues["sliderValue4"] = String(sliderValue4);
  sliderValues["sliderValue5"] = String(sliderValue5);
  sliderValues["temperature"] = String(dht.readTemperature());
  sliderValues["humidity"] =  String(dht.readHumidity());
  if (m == "Лампа включена") {
    // sliderValues["toggle"] = String(toggleon);
    toggle = String("Лампа включена");
  }
  if (m == "Лампа выключена") {
    // sliderValues["toggle"] = String(toggleoff);
    toggle = String("Лампа выключена");
  }
  sliderValues["toggle"] = String(toggle);

  String jsonString = JSON.stringify(sliderValues);
  return jsonString;
}

// Initialize SPIFFS
void initFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
   Serial.println("SPIFFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String prop) {
  ws.textAll(prop);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    Serial.print(message);
    if (message == "Лампа включена") {
      dutyCycle1 = map(sliderValue1.toInt(), 0, 100, 0, 255);
      dutyCycle2 = map(sliderValue2.toInt(), 0, 100, 0, 255);
      dutyCycle3 = map(sliderValue3.toInt(), 0, 100, 0, 255);
      dutyCycle4 = map(sliderValue4.toInt(), 0, 100, 0, 255);
      dutyCycle5 = map(sliderValue5.toInt(), 0, 100, 0, 255);
      notifyClients(getSliderValues(message));
    }
    if (message == "Лампа выключена") {
      dutyCycle1 = 0;
      dutyCycle2 = 0;
      dutyCycle3 = 0;
      dutyCycle4 = 0;
      dutyCycle5 = 0;
      notifyClients(getSliderValues(message));
    }
    // Serial.print(message.indexOf("1s"));
    if (message == "getReadings") {
      // Serial.print(getSensorReadings());
      // notifyClientsDHT(getSensorReadings());
      // Serial.print(getSensorReadings());
      notifyClients(getSliderValues(message));
    }
    if ((message.indexOf("1s") >= 0)) {
      sliderValue1 = message.substring(2);
      dutyCycle1 = map(sliderValue1.toInt(), 0, 100, 0, 255);
      // Serial.println(dutyCycle1);
      // Serial.print(getSliderValues());
      notifyClients(getSliderValues(message));
    }
    if ((message.indexOf("2s") >= 0)) {
      sliderValue2 = message.substring(2);
      dutyCycle2 = map(sliderValue2.toInt(), 0, 100, 0, 255);
      // Serial.println(dutyCycle2);
      // Serial.print(getSliderValues());
      notifyClients(getSliderValues(message));
    }    
    if ((message.indexOf("3s") >= 0)) {
      sliderValue3 = message.substring(2);
      dutyCycle3 = map(sliderValue3.toInt(), 0, 100, 0, 255);
      // Serial.println(dutyCycle3);
      // Serial.print(getSliderValues());
      notifyClients(getSliderValues(message));
    }
    if ((message.indexOf("4s") >= 0)) {
      sliderValue4 = message.substring(2);
      dutyCycle4 = map(sliderValue4.toInt(), 0, 100, 0, 255);
      // Serial.println(dutyCycle4);
      // Serial.print(getSliderValues());
      notifyClients(getSliderValues(message));
    }
    if ((message.indexOf("5s") >= 0)) {
      sliderValue5 = message.substring(2);
      dutyCycle5 = map(sliderValue5.toInt(), 0, 100, 0, 255);
      // Serial.println(dutyCycle5);
      // Serial.print(getSliderValues());
      notifyClients(getSliderValues(message));
    }
    if ((strcmp((char*)data, "getValues") == 0)) {
      notifyClients(getSliderValues(message));
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);
  ledcSetup(ledChannel4, freq, resolution);
  ledcSetup(ledChannel5, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin1, ledChannel1);
  ledcAttachPin(ledPin2, ledChannel2);
  ledcAttachPin(ledPin3, ledChannel3);
  ledcAttachPin(ledPin4, ledChannel4);
  ledcAttachPin(ledPin5, ledChannel5);

  initFS();
  initWiFi();
  initWebSocket();
  server.begin();
  
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");

  server.on("/img", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ETUlogo.png", "image/png");
  });

  // Start server
  dht.begin();
}

void loop() {
  ledcWrite(ledChannel1, dutyCycle1);
  ledcWrite(ledChannel2, dutyCycle2);
  ledcWrite(ledChannel3, dutyCycle3);
  ledcWrite(ledChannel4, dutyCycle4);
  ledcWrite(ledChannel5, dutyCycle5);

  ws.cleanupClients();
}