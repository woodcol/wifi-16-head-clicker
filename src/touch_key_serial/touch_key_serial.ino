#include <ESP8266WiFi.h>
#include "touchUtil.h"
#include "touch.h"

#define Key 2

#define SCL 5
#define SDA 4


#define BaseTime 1000    //防锁屏时间，1000*10 = 10秒
#define ShuaXin 9        //防锁屏按键J值

extern HC595Touch touchObj;

int inByte = 0;         // incoming serial byte

// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(9600);
  delay(10);
  pinMode(Key,INPUT);
  pinMode(SCL,INPUT);
  pinMode(SDA,INPUT);
  touchObj.initTouch();
}

//1:00,2:01,3:02,4:10,5:11,6:12
//J1:设置，J2:分钟设置，J3:数字0，J4：数字1，J5：数字2，J6：关闭，J7：发送，J8：询问(激活屏幕)，J9：确定

void touchOnce(int p){
  touchPin(p); //J1按下
  delay(100);
  untouchPin(p);//J1抬起
  delay(100);
  delay(1000);
}

void func(int tp){
  //J1:设置
  touchOnce(1);
  //J2:分钟设置
  touchOnce(2);

  //设置分钟数
  switch (tp)
  {
  case 1:
    /* code */
    touchOnce(3);
    touchOnce(3);
    break;
  case 2:
    /* code */
    touchOnce(3);
    touchOnce(4);
    break;
  case 3:
    /* code */
    touchOnce(3);
    touchOnce(5);
    break;
  case 4:
    /* code */
    touchOnce(4);
    touchOnce(3);
    break;
  case 5:
    /* code */
    touchOnce(4);
    touchOnce(4);
    break;
  case 6:
    /* code */
    touchOnce(4);
    touchOnce(5);
    break;
  default:
    break;
  }

  //关闭
  touchOnce(6);
  touchOnce(7);//发送
  touchOnce(8);//询问
  touchOnce(9);//确定

}


int touchcount = BaseTime;
int16_t repeatcount = BaseTime;
//主循环
void loop() {
  int kstate = digitalRead(Key);
  int sclst = digitalRead(SCL);
  int sdast = digitalRead(SDA);
  if(!(kstate && sclst && sdast)){
    repeatcount = BaseTime;
    touchcount++;
    if(touchcount>6){
      touchcount = 1;
    }
    Serial.println("key start:");
      func(touchcount);
      Serial.println("key Ok:");
      Serial.println(touchcount);
  }
  if (Serial.available() > 0) {
    repeatcount = BaseTime;
    // get incoming byte:
    inByte = Serial.read();
    // read first analog input, divide by 4 to make the range 0-255:
    if(inByte >= 1 && inByte <= 6){
      Serial.println("Serial start:");
      func(inByte);
      Serial.println("Serial ok:");
      Serial.println(inByte);
    }
  }else{
    delay(10);
    repeatcount--;
    if(repeatcount <= 0){
      repeatcount = BaseTime;
      touchOnce(ShuaXin);//询问
    }
  }
}
