#include <ESP8266WiFi.h>
//#include <string.h>
#include "webConfig.h"
#include "touchUtil.h"
#include "touch.h"
//巴法云服务器地址默认即可
#define TCP_SERVER_ADDR "bemfa.com"
//服务器端口，tcp创客云端口8344
#define TCP_SERVER_PORT "8344"
//********************需要修改的部分*******************//

#define LED 2
#define Key 2   //板子上的按钮引脚定义

extern HC595Touch touchObj;

String UID = "";  //用户私钥，可在控制台获取,修改为自己的UID:e04edc42beab4c40bd4bf1624abfe9f0
String TOPIC =   "fengmm521";         //主题名字，可在控制台新建
const int Pin1 = 5;              //单片机LED引脚值，GPIO0引脚
const int Pin2 = 4;  
const int Pin3 = 12;  
const int Pin4 = 13;  
//**************************************************//

//最大字节数
#define MAX_PACKETSIZE 512
//设置心跳值30s
#define KEEPALIVEATIME 60*1000

//tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;//心跳
unsigned long preTCPStartTick = 0;//连接
bool preTCPConnected = false;


char RCVBUFF[128]={0,};


void ledBlink(int n){
  for(int i = 0;i < n;i++){
      digitalWrite(LED, 0);
      delay(300);
      digitalWrite(LED, 1);
      delay(300);
   }
}

//相关函数初始化
//连接WIFI
void doWiFiTick();
void startSTA();

//TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

//led 控制函数
void turnOnLed();
void turnOffLed();



/*
  *发送数据到TCP服务器
 */
void sendtoTCPServer(String p){
  
  if (!TCPclient.connected()) 
  {
    Serial.println("Client is not readly");
    return;
  }
  TCPclient.print(p);
  Serial.println("[Send to TCPServer]:String");
  Serial.println(p);
  preHeartTick = millis();//心跳计时开始，需要每隔60秒发送一次数据
}


/*
  *初始化和服务器建立连接
*/
void startTCPClient(){
  if(TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT))){
    Serial.print("\nConnected to server:");
    Serial.printf("%s:%d\r\n",TCP_SERVER_ADDR,atoi(TCP_SERVER_PORT));
    
    String tcpTemp="";  //初始化字符串
    tcpTemp = "cmd=1&uid="+UID+"&topic="+TOPIC+"\r\n"; //构建订阅指令
    sendtoTCPServer(tcpTemp); //发送订阅指令
    tcpTemp="";//清空
    
    preTCPConnected = true;
    TCPclient.setNoDelay(true);
  }
  else{
    Serial.print("Failed connected to server:");
    Serial.println(TCP_SERVER_ADDR);
    TCPclient.stop();
    preTCPConnected = false;
  }
  preTCPStartTick = millis();
}


/*
  *检查数据，发送心跳
*/
void doTCPClientTick(){
 //检查是否断开，断开后重连
   if(WiFi.status() != WL_CONNECTED) return;

  if (!TCPclient.connected()) {//断开重连

    if(preTCPConnected == true){

      preTCPConnected = false;
      preTCPStartTick = millis();
      Serial.println();
      Serial.println("TCP Client disconnected.");
      TCPclient.stop();
    }else if(millis() - preTCPStartTick > 1*1000){//重新连接
      startTCPClient();
    }
  }else{
    if (TCPclient.available()) {//收数据
      char c =TCPclient.read();
      TcpClient_Buff +=c;
      TcpClient_BuffIndex++;
      TcpClient_preTick = millis();
      
      if(TcpClient_BuffIndex>=MAX_PACKETSIZE - 1){
        TcpClient_BuffIndex = MAX_PACKETSIZE-2;
        TcpClient_preTick = TcpClient_preTick - 200;
      }
    }
    if(millis() - preHeartTick >= KEEPALIVEATIME){//保持心跳
      preHeartTick = millis();
      Serial.println("--Keep alive:");
      sendtoTCPServer("cmd=0&msg=keep\r\n");
    }
  }
  if((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick>=200))
  {//data ready
    TCPclient.flush();
    Serial.print("Rev string: ");
    TcpClient_Buff.trim(); //去掉首位空格
    Serial.println(TcpClient_Buff); //打印接收到的消息
    String getTopic = "";
    String getMsg = "";
    if(TcpClient_Buff.length() > 15){//注意TcpClient_Buff只是个字符串，在上面开头做了初始化 String TcpClient_Buff = "";
          //此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=light002&msg=off
          int topicIndex = TcpClient_Buff.indexOf("&topic=")+7; //c语言字符串查找，查找&topic=位置，并移动7位，不懂的可百度c语言字符串查找
          int msgIndex = TcpClient_Buff.indexOf("&msg=");//c语言字符串查找，查找&msg=位置
          getTopic = TcpClient_Buff.substring(topicIndex,msgIndex);//c语言字符串截取，截取到topic,不懂的可百度c语言字符串截取
          getMsg = TcpClient_Buff.substring(msgIndex+5);//c语言字符串截取，截取到消息
          Serial.print("topic:------");
          Serial.println(getTopic); //打印截取到的主题值
          Serial.print("msg:--------");
          Serial.println(getMsg);   //打印截取到的消息值
   }
   char firstc = getMsg.charAt(0);
   Serial.println(firstc);   //打印截取到的消息值
   Serial.println(getMsg.length());   //打印截取到的消息值
   if (getMsg.length() == 1 && (firstc >= '1' && firstc <= '9' || firstc >= 'a' && firstc <= 'g' || firstc >= 'A' && firstc <= 'G')){
      firstc = tolower(firstc);
      touchCmd(firstc);
      Serial.println("touchCmd");   //打印截取到的消息值
      Serial.println(firstc);   //打印截取到的消息值
      ledBlink(1);
   }else if(getMsg.length() == 4){//四位16进制数,即一次控制16个点击头的指令
      digitalWrite(LED, 0);
      RCVBUFF[0] = firstc;
      RCVBUFF[1] = getMsg.charAt(1);
      RCVBUFF[2] = getMsg.charAt(2);
      RCVBUFF[3] = getMsg.charAt(3);
      RCVBUFF[4] = '\0';
      touchsWith2Byte();
      Serial.println("2ByteTouch");   //打印截取到的消息值
      digitalWrite(LED, 1);
   }else if(getMsg.length() == 32){
     //通过微信更改UID,这种更改是临时更新,主控板重启后会使用旧UID
     //重新连接巴法云服务器
      UID = getMsg;
      preTCPConnected = false;
      preTCPStartTick = millis();
      Serial.println();
      Serial.println("TCP Client disconnected.And reconnected!");
      TCPclient.stop();
      ledBlink(3);
   }
   TcpClient_Buff="";
   TcpClient_BuffIndex = 0;
  }
}

void startSTA(){
  connectWifi();
}

/**************************************************************************
                                 WIFI
***************************************************************************/
/*
  WiFiTick
  检查是否需要初始化WiFi
  检查WiFi是否连接上，若连接成功启动TCP Client
  控制指示灯
*/
void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    startSTA();
    Serial.printf("Heap size:%d\r\n", ESP.getFreeHeap());
  }

  //未连接1s重连
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  //连接成功建立
  else {
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}

bool isUsedSerial = false;

// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(10);
  initFFS();
  delay(10);
  pinMode(LED,OUTPUT);

  

//读取UID
  if(readUID(RCVBUFF)){
    UID = String(RCVBUFF);
    Serial.println(UID);
  }
  Serial.println("Beginning...");
  untouchAll();
  if(UID.length() <= 1){//当前未设置巴法云UID,通过串口设置UID
    isUsedSerial = true;
    digitalWrite(LED, 0);
  }else{
    int delayCount = 2;  //定义，如果按钮被常按3秒以上，则运行smartconfig重新设置wifi模块
    int delayCountSerial = 60; //定义最长等待按键时间为6秒,
    while (delayCountSerial < 0)
    {
      digitalWrite(LED, 0);
      delay(100);
      int keystate = digitalRead(Key);
      if(!keystate){
        delayCount--;
        delayCountSerial--;
      }else{
        if(delayCount <= 0 && delayCountSerial > 0){
          Serial.println("key1");
        //使用串口设置参数巴法云uid
          isUsedSerial = true;
          break;
        }
      }
      digitalWrite(LED, 1);
    }
  }
  touchObj.initTouch();
}

void setUID(){
  UID = String(RCVBUFF);
  saveUID(RCVBUFF);
  Serial.println(UID.c_str()); 
  ledBlink(3);
  if(isLastWifiOK()){
     restartESP();
   }
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
   ledBlink(2);
   if(UID.length() == 32){
     restartESP();
   }
}

uint8_t tmpcount = 0;
int isRcvTime = 0;

void serialReciveData(){
  
  while(Serial.available()){
    digitalWrite(LED, 0);
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
    }else if(tmpdat == '['){
        tmpcount = 0;
        isRcvTime = 1; //为设置巴法云UID
        continue;
    }else if(tmpdat == ']'){
      if(tmpcount != 32){
        Serial.println("uid char size erro,The UID must 32");
        tmpcount = 0;
        isRcvTime = 0;
        continue;
      }
        isRcvTime = 0;
        RCVBUFF[tmpcount] = '\0';
        tmpcount = 0;
        setUID();
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
    digitalWrite(LED, 1);
  }
  
}

//循环
void loop() {
  if(isUsedSerial){//通过串口设置巴法云UID
    delay(1);
    serialReciveData();//串口接收数据
  }else{
    if(getNetOK()){
      doTCPClientTick();
    }else{
      if(!isWebServerRunning()){
        doWiFiTick();
      }
    }
    handleWebServer();
  }
}
