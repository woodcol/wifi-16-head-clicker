#ifndef SPIFFSUTIL_H
#define SPIFFSUTIL_H 

//初始化文件系统
void initFFS();

//格式化文件系统
void formatFFS();
//读取上一次是否成功连接wifi,当上一次成功是,本次开机就不启动配网功能而是直接连接路由器
//如果上一次wifi连接成功,则在文件系统中会有wifi.txt文件
//这里通过查看是否有wifi.txt文件判断wifi是否上次连接正常
bool isLastWifiOK();


//wifi密码和SSID会保存在一个wifi.txt的文件中
//保存方式,wifi SSID一行,wifi密码一行,中间用\n隔开
bool saveWifi(char* ssid,char* pwd);  //保存wifi
bool getWifi(char* ssid,char* pwd);    //读取wifi

void cleanWifi(); //清除之前的wifi

//清除uid
void cleanUID();

//保存uid
bool saveUID(char* uid);

//读取uid
bool readUID(char* uid);


//判断文件是否存在,存在返回true,否则返回false
bool isExistsFile(const char* fname);


//保存数据到文件
bool saveDataToFile(char* fname,char* data,size_t size);


//返回buffer读取到的指针位置,0表示没有读到数据或者读取错误
int readDataFromFile(char* fname,char* buffer,size_t size);

//保存数据到文件
bool saveStrToFile(char* fname,char* s);


//返回是否读取成功
bool readStrFromFile(char* fname,char* buffer);



#endif
