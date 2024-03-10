/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/
#include "touch.h"
#include <ESP8266WiFi.h>
#include <Scheduler.h>
#include "include.h"

/*
使用esp8266的v2.3.0版本,下载可以使用arduino首选项的附加开发者管理地址加上下边地址,然后搜esp8266的开发板选择2.3.0版本
https://arduino.esp8266.com/stable/package_esp8266com_index.json
esp8266的v2.3.0版本对应的Scheduler的多线程库是v0.1版
其他版本的没有试过,你可以尝试esp8266和Scheduler都使用最新版本

esp8266的v2.3.0开发板库离线官方下载地址:
https://github.com/esp8266/Arduino/releases/2.3.0
Scheduler库v0.1下载地址:
https://github.com/nrwiersma/ESP8266Scheduler/releases/0.1

*/


buttonTask button_task;
touchTask touch_task;

extern HC595Touch touchObj;
extern bool isSerialWork;//是否启用串口工作模式

void setup()
{
//  delay(300);
  Serial.begin(115200);

  initEEPROM();
  delay(100);

  touchObj.initTouch();
  //初始化主机IIC接口
  Serial.println("Start module");
  pinMode(Key, INPUT);
  pinMode(LED, OUTPUT);
  bool isKeyDoen = false;
//  isSerialWork = false;
  for(int i=0;i<20;i++)
  {
    digitalWrite(LED, 0);
    delay(100);
    digitalWrite(LED, 1);
    delay(100);
    int keystate = digitalRead(Key);
    if(!keystate){  
      isKeyDoen = true; 
      break;
    }
  }

  digitalWrite(LED, 1);
  delay(100);
  //判断原来的wifi是否可连接
  if(isKeyDoen){
    if(isSetSerial()){
      changeToNet();
    }else{
      setWifiType('2');
      isSerialWork = true;
    }
    delay(1000);
    digitalWrite(LED, 1);
    Serial.print("is in serial");
  }else if(isSetSerial()){
    isSerialWork = true;
    Serial.print("is set serial");
  }else{
    changeToNet();
  }
  
  
  
  // Serial.println("touchObj.initTouch");
  //启动按钮和读取网络数据线程
  Scheduler.start(&button_task);
  Scheduler.start(&touch_task);
  Scheduler.begin();
}
 
void loop(){}
