#include "include.h"
#include "dataUtil.h"
#include <EEPROM.h>

/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

/*
以下为EEPROM掉电存储器相关代码
*/


uint8_t EEPROMData[EEPROMSIZE] = {'1','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z',0,50,0,50,'0','0','0','0','0','0','0','0'};
//设置点击触发的方法，
//EEPROM有32个字节，其中保存数据段的意义为:
/***************EEPROM 数据段功能*****************
1.地址0：保存设置使用那种点击，
  '1':表示使用一个字符表示一次按下并延时一段时间后抬起，即点击一次
  '2':表示使用分开的按下和抬起两个分步动作
  '0':表示对设置值恢复成默认值
2.地址段1～16：分别表示完整动作点击的触发字符，默认值如下
  地址1,'1',意思为当接收到这个字符时,J1点击一次
  地址2,'2',意思为当接收到这个字符时,J2点击一次
  地址3,'3',意思为当接收到这个字符时,J3点击一次
  地址4,'4',意思为当接收到这个字符时,J4点击一次
  地址5,'5',意思为当接收到这个字符时,J5点击一次
  地址6,'6',意思为当接收到这个字符时,J6点击一次
  地址7,'7',意思为当接收到这个字符时,J7点击一次
  地址8,'8',意思为当接收到这个字符时,J8点击一次
  地址9,'9',意思为当接收到这个字符时,J9点击一次
  地址10,'a',意思为当接收到这个字符时,J10点击一次
  地址11,'b',意思为当接收到这个字符时,J11点击一次
  地址12,'c',意思为当接收到这个字符时,J12点击一次
  地址13,'d',意思为当接收到这个字符时,J13点击一次
  地址14,'e',意思为当接收到这个字符时,J14点击一次
  地址15,'f',意思为当接收到这个字符时,J15点击一次
  地址16,'g',意思为当接收到这个字符时,J16点击一次
3.地址段17~49:分别表示每一个分开的点击和抬起动作的两个字符值,默认值如下
  地址17,'0',意思为当接收到这个字符时J1按下,地址18,'1',意思为当接收到这个字符时J1抬起不点击
  地址19,'2',意思为当接收到这个字符时J1按下,地址20,'3',意思为当接收到这个字符时J2抬起不点击
  地址21,'4',意思为当接收到这个字符时J1按下,地址22,'5',意思为当接收到这个字符时J3抬起不点击
  地址23,'6',意思为当接收到这个字符时J1按下,地址24,'7',意思为当接收到这个字符时J4抬起不点击
  地址25,'8',意思为当接收到这个字符时J1按下,地址26,'9',意思为当接收到这个字符时J5抬起不点击
  地址27,'a',意思为当接收到这个字符时J1按下,地址28,'b',意思为当接收到这个字符时J6抬起不点击
  地址29,'c',意思为当接收到这个字符时J1按下,地址30,'d',意思为当接收到这个字符时J7抬起不点击
  地址31,'e',意思为当接收到这个字符时J1按下,地址32,'f',意思为当接收到这个字符时J8抬起不点击
  地址33,'g',意思为当接收到这个字符时J1按下,地址34,'h',意思为当接收到这个字符时J9抬起不点击
  地址35,'i',意思为当接收到这个字符时J1按下,地址36,'j',意思为当接收到这个字符时J10抬起不点击
  地址37,'k',意思为当接收到这个字符时J1按下,地址38,'l',意思为当接收到这个字符时J11抬起不点击
  地址39,'m',意思为当接收到这个字符时J1按下,地址40,'n',意思为当接收到这个字符时J12抬起不点击
  地址41,'o',意思为当接收到这个字符时J1按下,地址42,'p',意思为当接收到这个字符时J13抬起不点击
  地址43,'q',意思为当接收到这个字符时J1按下,地址44,'r',意思为当接收到这个字符时J14抬起不点击
  地址45,'s',意思为当接收到这个字符时J1按下,地址46,'t',意思为当接收到这个字符时J15抬起不点击
  地址47,'u',意思为当接收到这个字符时J1按下,地址48,'v',意思为当接收到这个字符时J16抬起不点击
  地址49,'x',当收到这个字符时,所有点击头都按下
  地址50,'y',当收到这个字符时,所有点击都设置为非按下
  地址51,'z',当收到这个字符时,为测试模式,每收到这个字符,所有点击头状态从J1向J16移动一次
  地址52~53,高8位在前,低8位在后,为两个字节的点击按下延时时间,默认为50,即,点击模式为1时每次点击按下的延时时间为50ms
  地址54~55,高8位在前,低8位在后,为两个次点击的间隔时间最小值,默认也是50,即50ms
  地址62,为是否已连接网络的标志,如果已连接,这个地址保存为'1',如果未连接,这个地址保存为0或者'0',如果是使用串口工作模式,这个值为'2'
  地址63,为数据是否为初始值的标志,只要用户有修改过这个数据,这一个字节就会被设置为'1',否则值是'0'
  
********************************/

//EEPROM操作说明:
//https://www.cnblogs.com/liming19680104/p/11176481.html
//
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



void saveDataToEEPROM(bool isEnd)
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
void initEEPROM()
{ 
  beginEEPROM();
  uint8_t tmp = EEPROM.read(0);
  if( tmp == '0' || (tmp != '1' && tmp != '2')){
    saveDataToEEPROM();
  }else{
    readDataFromEEPROM();
  }
  endEEPROM();
}

void onEEPROMDataChange()
{
  EEPROMData[63] = '1';
}

//设置点击类型,'1','2','0'三种参数
//'1':表示使用一个字符表示一次按下并延时一段时间后抬起，即点击一次
//'2':表示使用分开的按下和抬起两个分步动作
//'0':表示对设置值恢复成默认值
void setTouchType(uint8_t ptype)
{
  EEPROMData[0] = ptype;
  onEEPROMDataChange();
  saveDataToEEPROM();
}

bool isSetWifi()
{
  if(EEPROMData[62] == '1'){
    return true;
  }else{
    return false;
  }
}

bool isSetSerial()
{
  if(EEPROMData[62] == '2'){
    return true;
  }else{
    return false;
  }
}

void setWifiType(uint8_t ptype)
{
  EEPROMData[62] = ptype;
  saveDataToEEPROM();
}




int char2int(char input)
{
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  return 0;
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char *src, uint8_t *target)
{
  while (*src && src[1])
  {
    *(target++) = char2int(*src) * 16 + char2int(src[1]);
    src += 2;
  }
}
