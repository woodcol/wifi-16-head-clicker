#include <ESP8266WiFi.h>
#include "touchUtil.h"

#define Key 2

// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(10);
  pinMode(Key,INPUT);
  touchObj.initTouch();
}

//主循环
void loop() {
  int kstate = digitalRead(Key);
  if(!kstate){
      touchPin(1); //J1按下
      delay*100);
      untouchPin(1);//J1抬起
      delay(100);
  }
}
