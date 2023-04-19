#include <ESP8266WiFi.h>
 

#ifndef STASSID
#define STASSID "test"
#define STAPSK  "a12345678"
#endif
 
const char* ssid = STASSID;
const char* password = STAPSK;
 

WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
 
  
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
 
  //连接WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
 
  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());
}
 
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));
 
  client.setTimeout(5000); //默认为 1000
 
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);
 
  int val;
  if (req.indexOf(F("/gpio/0")) != -1) {
    val = 1;
    Serial.println("开灯");
 
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    val = 0;
     Serial.println("关灯");
  } else {
    Serial.println(F("invalid request"));
    val = digitalRead(D2);
  }
 
 
  digitalWrite(D2, val);
 
  while (client.available()) {
    client.read();
  }
 
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<meta charset=\"utf-8\">LED灯控制 : "));
  client.print((val) ? F("关") : F("开"));
  client.print(F("<br><br> <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>  点击开灯  </a> , 或  <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>  点击关灯  </a> </html>"));
 
  Serial.println(F("Disconnecting from client"));
}
