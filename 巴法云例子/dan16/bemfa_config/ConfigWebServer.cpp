#include <ESP8266WiFi.h>
#include <WiFiClient.h>
// #include <ESP8266WebServer.h>
// #include <ESPmDNS.h>
#include <ESP8266mDNS.h>
#include "webConfigData.h"
#include "indexhtml.h"
#include "ConfigWebServer.h"
#include "WifiScanUtil.h"
#include "APAndSTAMode.h"

#include "LFSUtil.h"

extern char ssid_sta[];
extern char password_sta[];

//Webserber
ESP8266WebServer *server = new ESP8266WebServer(80);

bool serverruning = false;

ESP8266WebServer* getWebServerPtr()
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
  digitalWrite(LED_BUILTIN, 0);
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

void handleWebServer() 
{
  if(serverruning){
    server->handleClient();
    delay(1);
  }
}
bool startWebServerHandle(){
  serverruning = true;
}
bool stopWebServerAndClernHandle()
{
  serverruning = false;
  server->stop();
}
void initConfigWebServer()
{
  
  server->on("/", handleRoot);
  server->on("/settings",HTTP_GET, handleRoot);
  // server->on("/settings",HTTP_POST, returnOK,configGetSSIDAndPassword);
  server->on("/settings",HTTP_POST, returnOK);
  server->on("/inline", []() {
    server->send(200, "text/plain", "this works as well");
  });
  server->onNotFound(handleNotFound);
  server->begin();
  Serial.println("wifi config HTTP server started");
  // startWebServerHandle();
  serverruning = true;
}


