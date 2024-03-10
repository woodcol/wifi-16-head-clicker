#include "webConfigData.h"

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiAP.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiAP.h>
#endif


#include <WiFiClient.h>
#include "webConfigData.h"
#include "APAndSTAMode.h"
#include "LFSUtil.h"
#include "ConfigWebServer.h"



const byte DNS_PORT = 53;

const char *ssid_ap = "fengmm521.taobao.com";
const char *password_ap = "";

char ssid_sta[64] = {0};
char password_sta[64] = {0};

bool isNetOK = false;       //模块是否正常连接到路由器

// IPAddress local_IP(192,168,1,1);
// IPAddress gateway(192,168,1,1);
// IPAddress subnet(255,255,255,0);


bool isWorkerMode = false;

void resetWiFiAndRestart()
{
  WiFi.disconnect(false); //断开连接并清除保存的wifi
  cleanWifi();
  delay(1000);
  ESP.restart();  //复位esp32
}
//0:ap工作模式,1:sta已连上路由器,2:sta方式还没有连上路由
int getNetState(){
  if(isWorkerMode){
      if(isNetOK){
        return 1;
      }else{
        return 2;
      }
  }else{
      return 0;
  }
}

bool getNetOK(){
  return isNetOK;
}

void setAPMode()
{
    initFFS();
    isWorkerMode = false;
    WiFi.mode(WIFI_AP);//配置为AP模式
    // WiFi.softAPConfig(local_IP, gateway, subnet);
    bool result = WiFi.softAP(ssid_ap, password_ap); //开启WIFI热点
    if (result)
      {

        IPAddress myIP = WiFi.softAPIP();
        //打印相关信息
        Serial.println("");
        Serial.print("Soft-AP IP address = ");
        Serial.println(myIP);
      } else {  //开启热点失败

        Serial.println("WiFiAP Failed");

        delay(3000);
        ESP.restart();  //复位esp32
      }

    if (MDNS.begin("fengmm521")) 
    {
    Serial.println("MDNS responder started");
    }
// 原文链接：https://blog.csdn.net/u014091490/article/details/99709431

}


//wifi event handler,当已经连到到路由器并得到IP地址时的回调方法
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
#ifdef ESP32
      case SYSTEM_EVENT_STA_GOT_IP:
#else
      case WIFI_EVENT_STAMODE_GOT_IP:
#endif
      
          //When connected set 
          Serial.println("");
          Serial.print("Connected to ");
          Serial.println(ssid_sta);
          Serial.print("IP address: ");
          Serial.println(WiFi.localIP());

          initWorkWebServer();

          break;
#ifdef ESP32
      case SYSTEM_EVENT_STA_DISCONNECTED:
#else
      case WIFI_EVENT_STAMODE_DISCONNECTED:
#endif
          Serial.println("WiFi lost connection");
          isNetOK = false;
          break;
      default: break;
    }

    Serial.println("wifi envent end!");
}

//尝试自动连接上次的路由器
bool autoConfig()
{
  initFFS();

  isWorkerMode = true;
  if(isLastWifiOK()){
    getWifi(ssid_sta,password_sta);//保存wifi ssid和密码
    Serial.printf("lastSSID:%s\r\n", ssid_sta);
    Serial.printf("lastPSW:%s\r\n", password_sta);
    WiFi.begin(ssid_sta,password_sta);
    WiFi.onEvent(WiFiEvent);
    for (int i = 0; i < 10; i++)
    {
      int wstatus = WiFi.status();
      if (wstatus == WL_CONNECTED)
      {
       Serial.println("AutoConfig Success");
       Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
       Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

       WiFi.printDiag(Serial); //输出wifi信息
       digitalWrite(LED, 1);
       isNetOK = true;
       return true;
      }
      else
      {
       Serial.print("AutoConfig Waiting......");
       Serial.println(wstatus);
       digitalWrite(LED, 0);
       delay(500);
       digitalWrite(LED, 1);
       delay(500);
      }
    }
  }else{
    Serial.println("last wifi ssid and password not find!" );
  }
  Serial.println("AutoConfig Faild!" );
  return false;
}

void runWebConfig(){
  if(getNetState() != 0){//说明当前在wifi工作在sta模式,则清除原来的wifi
    Serial.println("runwebconfig clean wifi sta mode!" );
    WiFi.disconnect(false); //断开连接并清除保存的wifi
    cleanWifi();
  }
    setAPMode();
    initConfigWebServer();
    delay(100);
}
void connectWifi(bool isRepeate)
{
  if(autoConfig())
  {
   //连接路由器成功 
   Serial.println("connected wifi ok!" );
  }else{
    //连接路由器未成功,启动web配网
    if(isRepeate && isLastWifiOK()){
      restartESP();
    }else{
      Serial.println("start webConfig!" );
      runWebConfig();
    }
  }
}
void restartESP()
{
    Serial.println("ESP restart!");
    ESP.restart();  //复位esp32
}
