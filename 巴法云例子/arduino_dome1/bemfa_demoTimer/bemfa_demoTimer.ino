#include <ESP8266WiFi.h>
#include "touchUtil.h"
#include "touch.h"
//巴法云服务器地址默认即可
#define TCP_SERVER_ADDR "bemfa.com"
//服务器端口，tcp创客云端口8344
#define TCP_SERVER_PORT "8344"
//********************需要修改的部分*******************//

#define DEFAULT_STASSID  "WIFI_SSID"              //WIFI名称，区分大小写，不要写错
#define DEFAULT_STAPSW   "Wifi_PWD"               //WIFI密码
String UID = "0a56f28cdbdc490ea6750ef983251022";  //用户私钥，可在控制台获取,修改为自己的UID:
String TOPIC =   "fengmm521";                     //主题名字，可在控制台新建

//**************************************************//

//最大字节数
#define MAX_PACKETSIZE 512
//设置心跳值30s
#define KEEPALIVEATIME 60*1000

#define LED 2


//tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;//心跳
unsigned long preTCPStartTick = 0;//连接
bool preTCPConnected = false;

char RCVBUFF[128]={0,};

char touchBuff[5] = {0,};
int h_time = -1;
int m_time = -1;
int s_time = -1;


extern HC595Touch touchObj;

void cleanBuffer(){
  for(int i = 0;i < 128;i++){
    RCVBUFF[i] = '\0';
  }
}

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
  }
  else if(millis() - preTCPStartTick > 1*1000)//重新连接
    startTCPClient();
  }
  else
  {
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
    String timeBuf = "";
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
   }else if(getMsg.length() == 6){
     //通过微信设置定时点击的时间
      timeBuf = getMsg.substring(0,2);
      h_time = atoi(timeBUf.c_str());
      timeBuf = getMsg.substring(2,4);
      m_time = atoi(timeBUf.c_str());
      timeBuf = getMsg.substring(4,6);
      s_time = atoi(timeBUf.c_str());
      timeBuf = getMsg.substring(6);
      // touchBuff[0] = getMsg.charAt(6);
      // touchBuff[1] = getMsg.charAt(7);
      // touchBuff[2] = getMsg.charAt(8);
      // touchBuff[3] = getMsg.charAt(9);
      // touchBuff[4] = '\0';
      preTCPConnected = false;
      preTCPStartTick = millis();
      Serial.println();
      Serial.println("TCP Client disconnected.And reconnected!");
      TCPclient.stop();
      ledBlink(3);
   }
   TcpClient_Buff="";
   TcpClient_BuffIndex = 0;
   cleanBuffer();
  }
}

void startSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);
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
      digitalWrite(LED,HIGH);
      startTCPClient();
      
    }
  }
}
////打开灯泡
//void turnOnLed(){
//  Serial.println("Turn ON");
//  digitalWrite(LED_Pin,LOW);
//}
////关闭灯泡
//void turnOffLed(){
//  Serial.println("Turn OFF");
//    digitalWrite(LED_Pin,HIGH);
//}


// 初始化，相当于main 函数
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(10);
  pinMode(LED,OUTPUT);
 
  Serial.println("Beginning...");

  touchObj.initTouch();
}

//循环
void loop() {
  doWiFiTick();
  doTCPClientTick();
}
