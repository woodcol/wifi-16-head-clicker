#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
//#include <AsyncUDP.h>

#include "LFSUtil.h"
#include "webConfigData.h"
#include "strUtil.h"

#define wifiSSID "/ssid"
#define wifiPWD "/pwd"
#define BFYUID "/bfyuid"

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

void cleanWifi()
{
  LittleFS.remove(wifiSSID);
  LittleFS.remove(wifiPWD);
}

void cleanUID()
{
    //BFYUID
    LittleFS.remove(BFYUID);
}

bool saveUID(char* uid)
{
  File f = LittleFS.open(BFYUID, "w");
  if (!f) {
    Serial.println("file open wifiSSID w failed");
    return false;
  }
  f.write((const char*)uid);
  f.close();
  return true;
  
}

bool readUID(char* uid)            //读取wifi名称
{  
  File f = LittleFS.open(BFYUID, "r");
  if (!f) {
    Serial.println("file open BFYUID r type failed");
    return false;
  }
  copyStr((char*)f.readString().c_str(),uid);
  f.close();
  return true;
}


bool isLastWifiOK()//读取上一次是否成功连接wifi,当上一次成功是,本次开机就不启动配网功能而是直接连接路由器
{
  if(LittleFS.exists(wifiSSID)){
    return true;
  }else{
    return false;
  }
}
bool saveWifi(char* ssid,char* pwd)  //保存wifi名称
{
  File f = LittleFS.open(wifiSSID, "w");
  if (!f) {
    Serial.println("file open wifiSSID w failed");
    return false;
  }
  f.write((const char*)ssid);
  f.close();

  f = LittleFS.open(wifiPWD, "w");
  if (!f) {
    Serial.println("file open wifiPWD w failed");
    return false;
  }
  f.write((const char*)pwd);
  f.close();

  return true;

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

bool getWifi(char* ssid,char* pwd)            //读取wifi名称
{
  File f = LittleFS.open(wifiSSID, "r");
  if (!f) {
    Serial.println("file open wifiSSID r failed");
    return false;
  }
  copyStr((char*)f.readString().c_str(),ssid);
//  ssid = f.readString().c_str();
  f.close();

  f = LittleFS.open(wifiPWD, "r");
  if (!f) {
    Serial.println("file open wifiPWD r failed");
    return false;
  }
//  pwd = f.readString().c_str();
  copyStr((char*)f.readString().c_str(),pwd);
  f.close();
  
  return true;
}
