#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>

#include "touchUtil.h"
#include "touch.h"

const char *ssid     = "HUAWEI_DH_B535_A399";
const char *password = "dinghaodejia";

int day = -1;
int h_time = -1;
int m_time = -1;
int s_time = -1;

//random(min, max)


int ds[7] = {0,1,1,1,1,1,0};
#define TCOUNT 2

int hs[2] = [8,17];
int ms[2] = [30,30]



extern HC595Touch touchObj;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com",60*60*8, 30*60*1000);

void setup(){
  Serial.begin(115200);

  touchObj.initTouch();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }



  timeClient.begin();
}

void delayS(s = 1){
  for(int i = 0;i < s;i++){
    delay(1000);
  }
}

void delayM(m = 1){
  for(int i = 0;i < 60;i++){
    delaySec();
  }
}

void touchRun1(){
  Serial.println("time run1");
  int rt = random(0, 24);  //随机延时分钟数
  delayM(rt);
  int rst = random(0,60);  //随机延时秒数
  delayS(rst);
  touchCmd('7');
  delayS(5);
  touchCmd('8');
  delayS(5);
  touchCmd('9');
}

void touchRun2(){
  Serial.println("time run2");
  touchRun1();
}

#define UTimeDelay 1   //更新当前服务器时间间隔秒数


void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  day = timeClient.getDay();
  h_time = timeClient.getHours();
  m_time = timeClient.getMinutes();
  s_time = timeClient.getSeconds();
  Serial.println(day);
  Serial.println(h_time);
  Serial.println(m_time);
  Serial.println(s_time);
  if(ds[day] == 1){   //判断是不是要运行的所在星期
    if(hs[0] == h_time && ms[0] == m_time){  //判断时间1的小时和分钟是不是达到了
        touchRun1();
    }
    if(hs[1] == h_time && ms[1] == m_time){  //判断时间2的小和分钏是不是达到了
        touchRun2();
    }
  }
  for(int i = 0;i < 1;i++){
    delay(1000);
  }
}
