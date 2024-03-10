
#include "webConfigData.h"
#include "LFSUtil.h"
#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include <WiFiAP.h>

#define WIFIBUFERSIZE 30
#define EEPROMSIZE 64    //申请的EEPROM大小字节数
//eeprom缓存
uint8_t EEPROMData[EEPROMSIZE] = {'0','0','0','0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
/*
第1字节为是否已配过网,'1':已配过,'0':未配网
第2字节:表示eeprom是不是修改过
第3字节:是串口工作模式还是web工作模式,0:web工作模式(默认工作方式),1:串口工作模式
第4字节:摄像头图片格式,未使用
第5~35字节为wifi的ssid
第36~64字节为wifi的密码
*/

void setWifiOK(bool ptype);

bool isEEPROM_Open = false;
void beginEEPROM()
{
  if(!isEEPROM_Open){
    EEPROM.begin(EEPROMSIZE);
    isEEPROM_Open = true;
  }
  
}
void endEEPROM(){
  if(isEEPROM_Open){
    EEPROM.end();
    isEEPROM_Open = false;
  }
}


void readDataFromEEPROM()
{
  for(int i = 0;i<EEPROMSIZE;i++){
    EEPROMData[i] = EEPROM.read(i);; 
  }
}

void saveWifi(char* ssid,char* pwd)
{
  for(int i = 0;i<WIFIBUFERSIZE;i++){
    EEPROMData[i+4] = ssid[i];
    EEPROMData[i+34] = pwd[i];
  }
   setWifiOK(true);
}

void getWifi(char* ssid,char* pwd){
  for(int i = 0;i<WIFIBUFERSIZE;i++){
    ssid[i] = EEPROMData[i+4];
    pwd[i] = EEPROMData[i+34];
  }
}

void cleanWifi(){
  for(int i = 0;i<WIFIBUFERSIZE;i++){
    EEPROMData[i+4] = '\0';
    EEPROMData[i+34] = '\0';
  }
   setWifiOK(false);
}

void saveDataToEEPROM(bool isEnd = true)
{
  beginEEPROM();
  for(int i = 0;i<EEPROMSIZE;i++){
    EEPROM.write(i, EEPROMData[i]); 
  }
  if(isEnd){
    endEEPROM();
  }
}
//判断eeprom数据是否未初始化
void initFFS()
{ 
  beginEEPROM();
  uint8_t tmp = EEPROM.read(1);
  if( tmp == '0' || tmp != '1'){//eeprom第一次使用或者初绐设置,初始化eeprom
    saveDataToEEPROM();
  }else{//eeprom已经有修改过,则读到eeprom
    readDataFromEEPROM();
  }
  endEEPROM();
}

void onEEPROMDataChange()
{
  EEPROMData[1] = '1';
}

//'1':表示wifi连接成功过
//'0':表示wifi连接数据已被清空
void setWifiOK(bool ptype)
{
    if(ptype){
        EEPROMData[0] = '1';
    }else{
        EEPROMData[0] = '0';
    }
  
  onEEPROMDataChange();
  saveDataToEEPROM();
}
bool isLastWifiOK()
{
  if(EEPROMData[0] == '1'){
    
    return true;
  }else{
    return false;
  }
}
void setWorkSerialType(bool t){
  if(t){
        EEPROMData[2] = '1';
    }else{
        EEPROMData[2] = '0';
    }
  onEEPROMDataChange();
  saveDataToEEPROM();
}


bool getWorkSerialType()      //读取上次设置的使用方式
{
  if(EEPROMData[2] == '1'){
    return true;
  }else{
    return false;
  }

}
char getEEPromData(uint8_t n)
{
    if (n >= EEPROMSIZE)
    {
        return 0;
    }
    return EEPROMData[n];
}

#else

#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
//#include <AsyncUDP.h>

#include "LFSUtil.h"

#include "strUtil.h"

#define wifiSSID "/ssid"
#define wifiPWD "/pwd"
#define BFYUID "/bfyuid"
#define SERIALTYPE "/serial"

// r      Open text file for reading.  The stream is positioned at the
//        beginning of the file.

// r+     Open for reading and writing.  The stream is positioned at the
//        beginning of the file.

// w      Truncate file to zero length or create text file for writing.
//        The stream is positioned at the beginning of the file.

// w+     Open for reading and writing.  The file is created if it does
//        not exist, otherwise it is truncated.  The stream is
//        positioned at the beginning of the file.

// a      Open for appending (writing at end of file).  The file is
//        created if it does not exist.  The stream is positioned at the
//        end of the file.

// a+     Open for reading and appending (writing at end of file).  The
//        file is created if it does not exist.  The initial file
//        position for reading is at the beginning of the file, but
//        output is always appended to the end of the file.

// seek

// file.seek(offset, mode)
// This function behaves like fseek C function. Depending on the value of mode, it moves current position in a file as follows:

// if mode is SeekSet, position is set to offset bytes from the beginning.
// if mode is SeekCur, current position is moved by offset bytes.
// if mode is SeekEnd, position is set to offset bytes from the end of the file.
// Returns true if position was set successfully.

bool isLFSInited = false;


void initFFS()
{
  if(isLFSInited){
    return;
  }
  if(LittleFS.begin())
  {
    Serial.println("LittleFS Initialize....ok");
    isLFSInited = true;
  }
  else
  {
    Serial.println("LittleFS Initialization...failed");
  }
}

//格式化
void formatFFS()
{

  if(LittleFS.format())
  {
    Serial.println("File System Formated");
  }
  else
  {
    Serial.println("File System Formatting Error");
  }

}

void setWorkSerialType(bool t){
  if(t){
    File f = LittleFS.open(SERIALTYPE, "w");
    if (!f) {
      Serial.println("file open SERIALTYPE w failed");
      return;
    }
    f.write('1');
    f.close();
  }else{
    LittleFS.remove(SERIALTYPE);
  }

}
bool getWorkSerialType()      //读取上次设置的使用方式
{
  if(LittleFS.exists(SERIALTYPE)){
    return true;
  }else{
    return false;
  }
}

void cleanWifi()
{
  LittleFS.remove(wifiSSID);
  LittleFS.remove(wifiPWD);
}

bool isLastWifiOK()//读取上一次是否成功连接wifi,当上一次成功是,本次开机就不启动配网功能而是直接连接路由器
{
  if(LittleFS.exists(wifiSSID)){
    return true;
  }else{
    return false;
  }
}
void saveWifi(char* ssid,char* pwd)  //保存wifi名称
{
  File f = LittleFS.open(wifiSSID, "w");
  if (!f) {
    Serial.println("file open wifiSSID w failed");
    return;
  }
  f.write((const char*)ssid);
  f.close();

  f = LittleFS.open(wifiPWD, "w");
  if (!f) {
    Serial.println("file open wifiPWD w failed");
    return;
  }
  f.write((const char*)pwd);
  f.close();
}

bool isExistsFile(const char* fname)
{
  if(LittleFS.exists(fname)){
    return true;
  }else{
    return false;
  }
}
bool saveDataToFile(char* fname,char* data,size_t size)
{
  File f = LittleFS.open(fname, "w");
  if (!f) {
    Serial.println("file open fname w type failed");
    return false;
  }
  f.write((const char*)data);
  f.close();
  return true;
}
int readDataFromFile(char* fname,char* buffer,size_t size)
{
  File f = LittleFS.open(fname, "r");
  if (!f) {
    Serial.println("file open readDataFromFile failed");
    return 0;
  }
  int b = f.read((uint8_t*)buffer,size);
  f.close();
  return b;
}


//保存数据到文件
bool saveStrToFile(char* fname,char* s){
  File f = LittleFS.open(fname, "w");
  if (!f) {
    Serial.println("file open saveStrToFile failed");
    return false;
  }
  f.write((const char*)s);
  f.close();
  return true;
}


//返回是否读取成功
bool readStrFromFile(char* fname,char* buffer){
  File f = LittleFS.open(fname, "r");
  if (!f) {
    Serial.println("file open readStrFromFile failed");
    return false;
  }
  copyStr((char*)f.readString().c_str(),buffer);
  f.close();
  return true;
}

void getWifi(char* ssid,char* pwd)            //读取wifi名称
{
  File f = LittleFS.open(wifiSSID, "r");
  if (!f) {
    Serial.println("file open wifiSSID r failed");
    return;
  }
  copyStr((char*)f.readString().c_str(),ssid);
//  ssid = f.readString().c_str();
  f.close();

  f = LittleFS.open(wifiPWD, "r");
  if (!f) {
    Serial.println("file open wifiPWD r failed");
    return;
  }
//  pwd = f.readString().c_str();
  copyStr((char*)f.readString().c_str(),pwd);
  f.close();

}

#endif
