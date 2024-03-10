#include <ESP8266WiFi.h>
#include "touchUtil.h"
#include "touch.h"

#define Key 2

long lastNumber = -1;  //上一个输入的数字

#define StartN 0        //开始的数字，小于1000的数字前边的0省略了，0表示为0000
#define EndN   999999     //结束的数字，9999

//以下三个为时间相关定义，所有时间的单位都为毫秒，1秒=1000毫秒
#define touchTime   35    //点击一次屏幕的时间中按下的时间
#define touchUpTime 60    //一次点击之后不按下的时间,这个时间决定了两次点击屏之间的时间间隔
#define loopTime    800   //当完成一次4位或者6位密码连续输入之后，开始进行下一轮密码输入的时间间隔

#define isJokButton  1      //输入完成后是否有确认需要点击，有这里设置为1，没有设置为0.当设置为1时，密码输入完成后会触发点击Jok

bool isRun = false;
bool isNotEnd = true;

extern HC595Touch touchObj;

// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(10);
  pinMode(Key,INPUT);
  touchObj.initTouch();
}

void touchOnePinAction(int num)
{
  int realPin = num;
  if(num == 0){  //J10
    realPin = 10;
  }
  touchPin(realPin); //J按下
  delay(touchTime);
  untouchPin(realPin);//J抬起
  if(realPin == 11){ //J11
    delay(loopTime);
  }else{
    delay(touchUpTime);
  }
}

void touchClean6(){
  for(int i = 0;i < 6;i++){
    touchPin(12); //J12按下
    delay(touchTime);
    untouchPin(12);//J12抬起
    delay(touchTime);
  }
}

void cleanWithNum(int n){
  for(int i = n;i >= 0;i--){
    touchPin(12); //J12按下
    delay(touchTime);
    untouchPin(12);//J12抬起
    delay(touchTime);
  }
}


void touchNumer(long number)
{
  digitalWrite(LED_BUILTIN, HIGH); 
  int t1 = number%10;   //要输入的个位数
  int t2 = floor((number%100)/10);   //要输入的十位数
  int t3 = floor((number%1000)/100);   //要输入的百位数
  int t4 = floor((number%10000)/1000);   //要输入的千位数
  int t5 = -1;
  int t6 = -1;
  if(EndN > 9999){
    t5 = floor((number%100000)/10000);   //要输入的万位数
    t6 = floor((number%1000000)/100000);   //要输入的十万位数
  }
  Serial.println(number);
  if(t6 >= 0){
    touchOnePinAction(t6);
    Serial.print(t6);
  }
  if(t5 >= 0){
    touchOnePinAction(t5);
    Serial.print(t5);
  }
  touchOnePinAction(t4);
  Serial.print(t4);
  touchOnePinAction(t3);
  Serial.print(t3);
  touchOnePinAction(t2);
  Serial.print(t2);
  touchOnePinAction(t1);
  Serial.println(t1);
  digitalWrite(LED_BUILTIN, LOW);
  if(isJokButton){
    touchOnePinAction(11);
    delay(10);
    touchClean6();
  }else{
    delay(loopTime);
  }
  
}

int oldNS[6] = {-1,-1,-1,-1,-1,-1};
int nowNS[6] = {-1,-1,-1,-1,-1,-1};

void touchNumber2(long old,long now){
  
  oldNS[0] = old%10;   //要输入的个位数
  oldNS[1] = floor((old%100)/10);   //要输入的十位数
  oldNS[2] = floor((old%1000)/100);   //要输入的百位数
  oldNS[3] = floor((old%10000)/1000);   //要输入的千位数
  oldNS[4] = -1;
  oldNS[5] = -1;
  if(EndN > 9999){
    oldNS[4] = floor((old%100000)/10000);   //要输入的万位数
    oldNS[5] = floor((old%1000000)/100000);   //要输入的十万位数
  }

  nowNS[0] = now%10;   //要输入的个位数
  nowNS[1] = floor((now%100)/10);   //要输入的十位数
  nowNS[2] = floor((now%1000)/100);   //要输入的百位数
  nowNS[3] = floor((now%10000)/1000);   //要输入的千位数
  nowNS[4] = -1;
  nowNS[5] = -1;
  if(EndN > 9999){
    nowNS[4] = floor((now%100000)/10000);   //要输入的万位数
    nowNS[5] = floor((now%1000000)/100000);   //要输入的十万位数
  }

  int index = 5;
  if(old == now){
    for(int i=5;i>=0;i--){
      if(nowNS[i] >= 0){
        touchOnePinAction(nowNS[i]);
        Serial.print(nowNS[i]);
      }
    }
  }else{
    for(int i=5;i>=0;i--){
      if(oldNS[i] == nowNS[i]){
        continue;
      }else{
        index = i;
        break;
      }
    }

    cleanWithNum(index);

    for(int i = 5;i > index;i--){
      if(nowNS[i] >= 0){
          Serial.print(nowNS[i]);
      }
    }

    for(int i = index;i >= 0;i--){
      touchOnePinAction(nowNS[i]);
      Serial.print(nowNS[i]);
    }
  }
  
  Serial.println("");
  
  if(isJokButton){
    touchOnePinAction(11);
    delay(10);
  }else{
    delay(loopTime);
  }
}

long fnum = 0;

bool touchNextNumber()
{
  if(lastNumber < 0 || lastNumber < StartN){
    lastNumber = StartN;
  }else{
    lastNumber++;
  }
  if(lastNumber <= EndN){
    touchNumber2(fnum,lastNumber);  //只清除最后几位不相同的数
    // touchNumer(lastNumber);
    fnum = lastNumber;
  }else{
    lastNumber = EndN + 1;
    return false;
  }
  return true;
}


//主循环
void loop() {
  pinMode(Key, INPUT); 
  int kstate = digitalRead(Key);
  if(!kstate){
      isRun = true;
      delay(3000);
  }
  
  if(isRun){
    while(isNotEnd){
      pinMode(Key, INPUT); 
      kstate = digitalRead(Key);
      if(!kstate){
        isRun = false;
        delay(3000);
        break;
      }
      pinMode(Key, OUTPUT); 
      isNotEnd = touchNextNumber();
    }
  }else{
    pinMode(Key, OUTPUT); 
    if(isNotEnd){
      delay(100);
      digitalWrite(Key, HIGH);
    }else{
      digitalWrite(Key, LOW);
      delay(100);
      digitalWrite(Key, HIGH);
      delay(100);
    }
  }
}
