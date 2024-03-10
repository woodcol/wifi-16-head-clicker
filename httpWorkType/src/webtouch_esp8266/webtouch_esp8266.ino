#include "webConfig.h"
#ifdef ESP32

#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "touchUtil.h"
#include "touch.h"

extern HC595Touch touchObj;

char RCVBUFF[128]={0,};

bool isUsedSerial = false;

// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(10);
  initFFS();
  delay(10);
//  pinMode(LED,OUTPUT);
  pinMode(Key,INPUT);
  touchObj.initTouch();
  untouchAll();
  Serial.println("Beginning...");
  connectWifi();

  isUsedSerial = getWorkSerialType();
}
void setWifi(uint8_t count){
   char ssid[64] = {0,};
   char pswd[64] = {0,};
   char *pwd = pswd;
   char *sid = ssid;
   uint8_t pssid = 0;
   uint8_t ppswd = 0;
   bool isPwd = false;
   Serial.println(RCVBUFF); 
   for(uint8_t i = 0;i < count;i++){
     if(RCVBUFF[i] == ','){
        isPwd = true;
        ppswd = i;
        continue;
     }
     if(isPwd){
       *pwd = RCVBUFF[i];
       pwd++;
      }else{
        *sid = RCVBUFF[i];
        sid++;
      }
   }
   *pwd = '\0';
   *sid = '\0';
   Serial.print("ssid: "); 
   Serial.println(ssid); 
   Serial.print("password: "); 
   Serial.println(pswd); 
   saveWifi(ssid,pswd);//保存wifi ssid和密码
   setWorkSerialType(false);  //工作在web模式下
   ledBlink(2);
   restartESP();
}

uint8_t tmpcount = 0;
int isRcvTime = 0;

void serialReciveData(){
  
  while(Serial.available()){
#ifndef ESP32
   digitalWrite(LED, 0);
#endif
    char tmpdat = Serial.read();
    if(tmpdat == '<'){
      tmpcount = 0;
      isRcvTime = 2;//为双字节即时控制模式
      continue;
    }else if(tmpdat == '>'){
        isRcvTime = 0;
        tmpcount = 0;
        touchsWith2Byte();
        continue;
    }else if(tmpdat == '{'){
        tmpcount = 0;
        isRcvTime = 3; //为设置路由器密码
        continue;
    }else if(tmpdat == '}'){
        isRcvTime = 0;
        setWifi(tmpcount);
        tmpcount = 0;
        break;
    }
    if(!isRcvTime){ //为0,表示接收单字节指令
         Serial.print(tmpdat);
         touchCmd(tmpdat);
    }else{
         RCVBUFF[tmpcount] = tmpdat;
         tmpcount++;
         continue;
    }
#ifndef ESP32
   digitalWrite(LED, 1);
#endif
  }
}

//循环
void loop() {
  if(isUsedSerial){//通过串口设置wifi
    delay(1);
    serialReciveData();//串口接收数据
    if(!digitalRead(Key)){
      isUsedSerial = false;
      setWorkSerialType(isUsedSerial);
      Serial.print("is runing web config!"); 
      delay(1000);
    }
  }else{
    handleWebServer();
    if(!digitalRead(Key)){
      isUsedSerial = true;
      setWorkSerialType(isUsedSerial);
      Serial.print("is runing Serial config!"); 
      delay(1000);
    }
  }
}
