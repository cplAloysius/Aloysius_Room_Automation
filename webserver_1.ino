#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#include <ESP8266Ping.h>
#include <RCSwitch.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Mitsubishi.h>
#include <ESP8266HTTPClient.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

RCSwitch mySwitch = RCSwitch();

const char* ssid     = //My SSID;
const char* password = //My password;
ESP8266WebServer server(//port number);

int key = //parameter key;

const char* remote_ip = "www.google.com";
unsigned long lastping = 0;

#define light_signal //light signal
#define fan1_on //fan1_on
#define fan1_off //fan1_off
#define fan2_on //fan2_on
#define fan2_off //fan2_off
#define door_close //door_close
#define door_open //door_ope
#define door_lock //door_lock
#define door_stop //door_stop

#define BOTtoken //Telegram bot token
String chat_id = //My telegram chat id;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 28800, 60000);
String date;
String hms;
int hh;
String mm;
String ss;

String lightState = "off";
String fanState1 = "off";
String fanState2 = "off";
String doorState = "open";
String ACState = "off";

IRMitsubishiAC ac(D7);

void setup() {
  client.setInsecure();
  Serial.begin(115200);
  mySwitch.enableTransmit(D8);
  ac.begin();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/webserver",handleArg);
  server.begin();

  timeClient.begin();
  delay(1000);
  
  timeClient.update();
  datetimenow();
  startUpMessage();
}

void loop() {
  while (millis() - lastping < 10800000) {
    server.handleClient();
  }
  if(Ping.ping(remote_ip)){
    lastping = millis();
  }
  else {
    datetimenow();
    rebootingMessage();
    ESP.restart();
  }
}

void handleArg() {
  String message = "";
  String keyStr = String(key);

//Lights
  if (server.arg("lights") == "ON" && server.arg("paramKey") == keyStr) {
    lightState = "on";
    mySwitch.send(light_signal, 24);
    Serial.println("lights on");
    }
  if (server.arg("lights") == "OFF" && server.arg("paramKey") == keyStr) {
    lightState = "off";
    mySwitch.send(light_signal, 24);
    }
//Fan
  if (server.arg("fan1") == "ON" && server.arg("paramKey") == keyStr) {
    fanState1 = "on";
    mySwitch.send(fan1_on,24);
    }
  if (server.arg("fan1") == "OFF" && server.arg("paramKey") == keyStr) {
    fanState1 = "off";
    mySwitch.send(fan1_off,24);
    }
  if (server.arg("fan2") == "ON" && server.arg("paramKey") == keyStr) {
    fanState2 = "on";
    mySwitch.send(fan2_on,24);
    }
  if (server.arg("fan2") == "OFF" && server.arg("paramKey") == keyStr) {
    fanState2 = "off";
    mySwitch.send(fan2_off,24);
    }
//Door
  if (server.arg("door") == "CLOSE" && server.arg("paramKey") == keyStr) {
    doorState = "closed";
    mySwitch.send(door_close,24);
    }
  if (server.arg("door") == "LOCK" && server.arg("paramKey") == keyStr) {
    doorState = "locked";
    mySwitch.send(door_lock,24);
    }
  if (server.arg("door") == "OPEN" && server.arg("paramKey") == keyStr) {
    doorState = "open";
    mySwitch.send(door_open,24);
    }
  if (server.arg("door") == "STOP" && server.arg("paramKey") == keyStr) {
    doorState = "stopped";
    mySwitch.send(door_stop,24);
    }
//AC
  if (server.arg("AC") == "ON" && server.arg("paramKey") == keyStr) {
    ACState = "on";
    ac.on();
    ac.setFan(1);
    ac.setMode(kMitsubishiAcCool);
    ac.setTemp(25);
    ac.setVane(kMitsubishiAcVaneAuto);
    ac.send();
    }
  if (server.arg("AC") == "OFF" && server.arg("paramKey") == keyStr) {
    ACState = "off";
    ac.off();
    ac.setFan(1);
    ac.setMode(kMitsubishiAcCool);
    ac.setTemp(25);
    ac.setVane(kMitsubishiAcVaneAuto);
    ac.send();
    }
//Error message
  if (server.arg("paramKey") != keyStr)
    {
    message = "NOPE";
    }
  if (server.arg("lights") == "" && server.arg("fan1") == "" && server.arg("fan2") == "" && 
      server.arg("door") == "" && server.arg("AC") == "" && server.arg("paramKey") == "") {
    message = "NOPE";
    }
  if ((server.arg("lights") != "ON" && server.arg("lights") != "OFF" && server.arg("lights") != "") ||
      (server.arg("fan1") != "ON" && server.arg("fan1") != "OFF" && server.arg("fan1") != "") ||
      (server.arg("fan2") != "ON" && server.arg("fan2") != "OFF" && server.arg("fan2") != "") ||
      (server.arg("AC") != "ON" && server.arg("AC") != "OFF" && server.arg("AC") != "") ||
      (server.arg("door") != "CLOSE" && server.arg("door") != "OPEN" && server.arg("door") != "LOCK" && server.arg("door") != "STOP" && server.arg("door") != ""))
    {
    message = "NOPE";
    }
  
  server.send(200, "text/plain", message);
}

void startUpMessage() {
  String locIP = "IP address: " + WiFi.localIP().toString();
  String message = "ESP Rebooted";
  message.concat("\n");
  message.concat(date);
  message.concat("\n");
  message.concat(hms);
  message.concat("\n");
  message.concat(locIP);
  if(bot.sendMessage(chat_id, message)){
    Serial.println("Telegram message sent");
  }
}

void rebootingMessage() {
  String message = "Failed to connect, rebooting ESP...";
  message.concat("\n");
  message.concat(date);
  message.concat("\n");
  message.concat(hms);
  if(bot.sendMessage(chat_id, message)){
    Serial.println("Telegram message sent");
  }
}

void wrongDateMessage() {
  String message = "DateTime error, rebooting ESP...";
  message.concat("\n");
  message.concat(date);
  message.concat("\n");
  message.concat(hms);
  if(bot.sendMessage(chat_id, message)){
    Serial.println("Telegram message sent");
  }
}

void datetimenow() {
  timeClient.update();
  
  date = "Date: " + timeClient.getFormattedDate();
  
  hh = timeClient.getHours();
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();
  int year = timeClient.getYear();
  int whileran = 0;

  if (hh>12){
    hh = hh - 12;
    hms = "Time: " + String(hh) + ":" + mm + ":" + ss + " PM";
   }
  else if (hh==12){
    hms = "Time: " + String(hh) + ":" + mm + ":" + ss + " PM";
   }
  else {
    hms = "Time: " + String(hh) + ":" + mm + ":" + ss + " AM";
   }
    
   while (year < 2020) {
    whileran++;
    Serial.println("Wrong date");
    timeClient.forceUpdate();
    if (whileran > 10) {
      wrongDateMessage();
      ESP.restart();
      }
    } 
  }
