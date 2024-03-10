
#include "webConfigData.h"

#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#endif


#include "indexhtml.h"
#include "ConfigWebServer.h"
#include "WifiScanUtil.h"
#include "APAndSTAMode.h"
#include "LFSUtil.h"
#include "touchUtil.h"
#include "touch.h"

extern char ssid_sta[];
extern char password_sta[];
extern char RCVBUFF[];

bool serverruning = false;

//Webserber
#ifdef ESP32
WebServer *server = new WebServer(80);
WebServer* getWebServerPtr()
#else
ESP8266WebServer *server = new ESP8266WebServer(80);
ESP8266WebServer* getWebServerPtr()
#endif

{
  return server;
}

bool isWebServerRunning()
{
  return serverruning;
}

void handleRoot() {
  
  Serial.println("settings");
  wifi_scanrun();
  sendWifiConfigHtml(server);

}

void workRoot() {
  Serial.println("workRoot web");
  sendWifiWorkHtml(server);
}

void ledBlink(int n){
#ifndef ESP32
   for(int i = 0;i < n;i++){
      digitalWrite(LED, 0);
      delay(300);
      digitalWrite(LED, 1);
      delay(300);
   }
#endif
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }

  server->send(404, "text/plain", message);
#ifndef ESP32
   digitalWrite(LED, 0);
#endif
}

void returnOK() {
  if (server->hasArg("ssid") && server->hasArg("password")) {
        // Serial.println("get ssid and possword");
        memset(ssid_sta, 0, sizeof(*ssid_sta));
        memset(password_sta, 0, sizeof(*password_sta));
        strncpy(ssid_sta, server->arg("ssid").c_str(), server->arg("ssid").length());
        strncpy(password_sta, server->arg("password").c_str(), server->arg("password").length());
        Serial.println("ssid:");
        Serial.println(ssid_sta);
        Serial.println("password:");
        Serial.println(password_sta);
        saveWifi(ssid_sta,password_sta);//保存wifi ssid和密码
        //得到wifi密码,重启esp8266后会自动连网
        Serial.println("get ssid and password,ESP will restart to connect ssid!");
        restartESP();
        
        return;
    }
  server->send(200, "text/plain", "get wifi ssid and password,connecting network ...");
}

void handleTouch() {
  if (server->hasArg("t")) {
        int len = server->arg("t").length();
        String restr = server->arg("t");
        char firstc = restr.charAt(0);
        if (len == 1 && (firstc >= '1' && firstc <= '9' || firstc >= 'a' && firstc <= 'g' || firstc >= 'A' && firstc <= 'G')){
            firstc = tolower(firstc);
            touchCmd(firstc);
            Serial.println(firstc);
        }else if(len == 4){//四位16进制数,即一次控制16个点击头的指令
            RCVBUFF[0] = firstc;
            RCVBUFF[1] = restr.charAt(1);
            RCVBUFF[2] = restr.charAt(2);
            RCVBUFF[3] = restr.charAt(3);
            RCVBUFF[4] = '\0';
            touchsWith2Byte();
            Serial.println(RCVBUFF);
        }
        //点击程序返回发送的指令
        server->send(200, "text/plain",restr.c_str());
        return;
  }
  server->send(200, "text/plain", "cmd not find t arg...");
}

void handleWebServer() 
{
  if(serverruning){
    server->handleClient();
    delay(1);
  }
}

void initConfigWebServer()
{
  
  server->on("/", handleRoot);
  server->on("/settings",HTTP_GET, handleRoot);
  server->on("/cmd",HTTP_GET, handleTouch);
  server->on("/cmd",HTTP_POST, handleTouch);
  server->on("/settings",HTTP_POST, returnOK);
  server->on("/inline", []() {
    server->send(200, "text/plain", "this config web as well");
  });
  server->onNotFound(handleNotFound);
  server->begin();
  Serial.println("wifi config HTTP server started");
  serverruning = true;
}

void initWorkWebServer()
{
  
  server->on("/", workRoot);
  server->on("/cmd",HTTP_GET, handleTouch);
  server->on("/cmd",HTTP_POST, handleTouch);
  server->on("/inline", []() {
    server->send(200, "text/plain", "this work web as well");
    ledBlink(3);
  });
  server->onNotFound(handleNotFound);
  server->begin();
  Serial.println("wifi work HTTP server started");
  serverruning = true;
}
