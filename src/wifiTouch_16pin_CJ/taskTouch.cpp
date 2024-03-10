#include "include.h"
#include <ESP8266WiFi.h>
#include "taskTouch.h"
/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/
extern WiFiServer server;

extern bool isNetOK;

extern bool isSerialWork;//是否启用串口工作模式

extern char RCVBUFF[];


WiFiClient serverClients[MAX_SRV_CLIENTS];


uint8_t tmpcount = 0;
int isRcvTime = 0;

//读取wifi数据并使用IIC接口转发给点击器从机

    void touchTask::loop() {
              delay(1);
              if(isNetOK){
              uint8_t i;
              //check if there are any new clients
              if (server.hasClient()) {  //判断是否有新的client请求进来
                for (i = 0; i < MAX_SRV_CLIENTS; i++) {
                  
                    //释放旧无效或者断开的client
                    if (!serverClients[i] || !serverClients[i].connected()) {
                      if (!serverClients[i]) {
                        //serverClients[i]    判断指定序号的客户端是否有效
                        serverClients[i].stop();  //停止指定客户端的连接
                      }
                      
                      serverClients[i] = server.available();//分配最新的client
                      Serial.print("1个新的客户端: "); 
                      Serial.println(i);
                      break; //跳出一层for循环
                    }
                }
                
                //当达到最大连接数 无法释放无效的client，需要拒绝连接
                if (i == MAX_SRV_CLIENTS) {
                  WiFiClient client = server.available();
                  client.stop();
                  Serial.println("连接已达最大值");
                }
            }


              // if (server.hasClient()){
              //   for(i = 0; i < MAX_SRV_CLIENTS; i++){
              //     //find free/disconnected spot
              //     if (!serverClients[i] || !serverClients[i].connected()){
              //       if(serverClients[i]) serverClients[i].stop();
              //       serverClients[i] = server.available();
              //       Serial.print("New client: "); 
              //       Serial.print(i);
              //       continue;
              //       // break;
              //     }
              //   }
              //   //no free/disconnected spot so reject
              //   WiFiClient serverClient = server.available();
              //   serverClient.stop();
              // }
              //check clients for data
              bool isHeaveClient = false;
              for(i = 0; i < MAX_SRV_CLIENTS; i++){
                if (serverClients[i] && serverClients[i].connected()){
                  if(serverClients[i].available()){
                    //get data from the telnet client and push it to the UART
//                    uint8_t tmpcount = 0;
                    
                    digitalWrite(LED, 0);
                    while(serverClients[i].available()){
                        
                        char tmpdat = serverClients[i].read();
//                        Serial.println(String(tmpdat).c_str());
                        
                        if(tmpdat == '<'){
                          isRcvTime = 2;//为双字节即时控制模式
                          continue;

                        }else if(tmpdat == '>'){
                            RCVBUFF[tmpcount] = '\0';
                            isRcvTime = 0;
                            tmpcount = 0;
                            touchsWith2Byte();
                            continue;
                        }else if(tmpdat == '['){
                          isRcvTime = 1;
                          continue;
                        }else if(tmpdat == ']'){
                          RCVBUFF[tmpcount] = '\0';
                          isRcvTime = 0;
                          tmpcount = 0;

                          getTouchTIme();
                          continue;
                        }else if(tmpdat == '{'){
                            isRcvTime = 3; //为设置路由器
                            continue;
                        }else if(tmpdat == '}'){
                            isRcvTime = 0;
    
                            setWifi(tmpcount);
                            tmpcount = 0;
                            break;
                        }
                        if(!isRcvTime){
                          serverClients[i].print(tmpdat);
                          touchCmd(tmpdat);
                          // netcmd = tmpdat;
                          // serverClients[i].print(tmpdat);
                          // if(EEPROMData[0] == '2'){
                          //   touchCmd(netcmd);
                          //  }
                        }else{
                          RCVBUFF[tmpcount] = tmpdat;
                          tmpcount++;
                          continue;
                        }
                        
                      }  
                  digitalWrite(LED, 1);
                  }
                  isHeaveClient = true;
                }
              }
             if(!isHeaveClient){
                  delay(1000);
                  Serial.println("IP address: ");
                  Serial.println(WiFi.localIP());
             }
              
        }else if(isSerialWork){
          
          if (Serial.available() > 0) {
          delay(1); // 等待数据传完
//          uint8_t tmpcount = 0;
         // int isRcvTime = 0;
          digitalWrite(LED, 0);
          while(Serial.available()){
                        
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
                  isRcvTime = 1; //为设置!模式间隔时间
                  continue;
              }else if(tmpdat == ']'){
                  isRcvTime = 0;
                  tmpcount = 0;
                  getTouchTIme();
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
              if(!isRcvTime){
                   Serial.print(tmpdat);
                   // Serial.flush();
                   touchCmd(tmpdat);
                   
              }else{
                   RCVBUFF[tmpcount] = tmpdat;
                   tmpcount++;
                   continue;
             }
                        
            }  
            digitalWrite(LED, 1);
          }
                  
      }
    }
