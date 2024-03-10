#ifndef APANDSTAMODE_H
#define APANDSTAMODE_H 


void setAPMode();
bool autoConfig();
void restartESP();

//清除之前的路由信息,并重启esp设备
void resetWiFiAndRestart();

void runWebConfig();//运行web配网程序


//连接wifi,当没有设置wifi时就会自动启动web配网
void connectWifi(bool isRepeate = false);

//获取当前网络状态
//1:sta已连上路由器,2:ap工作模式,0:sta方式还没有连上路由
int getNetState();
bool getNetOK();

#endif