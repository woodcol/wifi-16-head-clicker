#include "include.h"
#include "netUtil.h"

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

WiFiServer server(23);      //socket服务器端口号

// extern bool isNetOK;       //模块是否正常连接到路由器
bool isNetOK = false;       //模块是否正常连接到路由器

bool isSerialWork = false;//是否启用串口工作模式

char RCVBUFF[128]={0,};

//尝试自动连接上次的路由器
bool autoConfig()
{
  WiFi.begin();
  for (int i = 0; i < 10; i++) //定义连接wifi时间为8秒,8秒连接不上则启动smartconfig
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
     Serial.println("AutoConfig Success");
     Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
     Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
     WiFi.printDiag(Serial); //输出wifi信息
     digitalWrite(LED, 1);
      server.begin();
      server.setNoDelay(true);
      isNetOK = true;
      return true;
    }
    else
    {
     Serial.print("AutoConfig Waiting......");
     Serial.println(wstatus);
      digitalWrite(LED, 0);
      delay(2000);
    }
  }
 Serial.println("AutoConfig Faild!" );
  return false;
}


//一键配置wifi连接
void smartConfig()
{
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    digitalWrite(LED, 0);
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      delay(5000);
      server.begin();
      server.setNoDelay(true);
      Serial.println("Ready! Use telnet ");
      digitalWrite(LED, 1);
      isNetOK = true;
      setWifiType('1');
      break;
    }
  }
}


//设置路由器密码
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

   //断开原来的网络
   isNetOK = false;
   WiFi.disconnect(false); //断开连接并清除保存的wifi
   setWifiType('0');
   untouchAll();
   for(int i =0;i < 128;i++){
      RCVBUFF[i] = 0;
    }
   //重新连接新的路由
   WiFi.begin(ssid, pswd);
  for (int i = 0; i < 15; i++) //定义连接wifi时间为8秒,8秒连接不上则启动smartconfig
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
     Serial.println("AutoConfig Success");
     Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
     Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
     WiFi.printDiag(Serial); //输出wifi信息
     delay(5000);
      digitalWrite(LED, 1);
      server.begin();
      server.setNoDelay(true);
      isNetOK = true;
      setWifiType('1');  //使用上次路由的wifi连接
      return;
    }
    else
    {
     Serial.print("set wifi Waiting......");
     Serial.println(wstatus);
     digitalWrite(LED, 0);
     delay(1000);
    }
  }
  Serial.println("set wifi Faild!" );
}


void changeToNet()
{
    isSerialWork = false;
    if(isSetWifi()){
      
      if (!autoConfig())
      {
        Serial.println("Start samrt config module");
        //开始进行smart config 设置wifi
        smartConfig();
      }
    }else{
      setWifiType('0');
      Serial.println("Start samrt config module");
        //开始进行smart config 设置wifi
      smartConfig();
    }
}
