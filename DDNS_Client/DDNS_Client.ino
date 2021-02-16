#if defined(ESP8266)
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#elif defined(ESP32)
#include "WiFi.h"
#include "HTTPClient.h"
#endif
#include <ArduinoJson.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Arduino.h>

byte buffer[61] = {0};
String station1_data = "1";
int gdo0;
int signal_strenght;
char timeHour1[10];
char timeHour2[10];
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
struct tm timeinfo;


uint8_t temprature_sens_read();
uint8_t tempValue;
#include <EasyDDNS.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
const char * ssid = "ADNAN";
const char * password = "28319999";

const char* PARAM_INPUT = "value";

StaticJsonDocument<300> doc;
StaticJsonDocument<300> doc2;
JsonObject root = doc.as<JsonObject>();
JsonArray arr = doc2.to<JsonArray>();
String x[1440];
long y_room[1440];
String y_outside[1440];

#include "DHT.h"
#define DHTPIN 4
#define LIGHT 34
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);



bool switch_ = false;

uint8_t start = 0;
uint8_t elapsed = 0;
uint8_t interval = 6000000;
time_t now;
time_t now1;

time_t start1;
time_t elapsed1;

unsigned long temp = 0;
struct timeval tv_now;
unsigned long difference1;
void setup() {
  for(int i = 0; i < 1440;i++){
    y_room[i] = 255;
    y_outside[i] = "255";
  }
  Serial.begin(115200);

  pinMode(33, OUTPUT);

  CC1101CONFIG();

  dht.begin();
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP()); // Print the IP address
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  serverFunctions();
  EasyDDNS.service("noip");

  EasyDDNS.client("esp32server.ddns.net", "adnanh123", "adnan123");

  // Get Notified when your IP changes
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP) {
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });
  start = millis();
  time(&now);
  time(&now1);
  time(&start1);
  time(&elapsed1);

  addRecord(1439);
}

void loop() {
  time(&now);

  time(&elapsed1);
  unsigned long start_l= (unsigned long)start1;
  unsigned long elapsed_l= (unsigned long)elapsed1;
  difference1 = elapsed1 - start1;
  

  //Serial.print("Diff:");
  //Serial.println(difference1);

  if(difference1 >= 60){
    updateRecord();
    time(&start1);
  }
  if(difference1 % 2 == 0 && temp != difference1){
    temp = difference1;
    Serial.println("updateStation");
    updateStation();
  }
  
  unsigned long now_l= (unsigned long)now;
  unsigned long now1_l= (unsigned long)now1;
  unsigned long difference;
  if(now_l > now1_l){
    //Serial.println(now_l - now1_l);
    difference = now_l - now1_l;
  }else if(now_l <= now1_l){
    //Serial.println(now1_l - now_l);
    difference = now_l - now1_l;
  }
  if(difference > 10){
    signal_strenght = 1;
  }
  recieveData();
  
  EasyDDNS.update(10000);
}
