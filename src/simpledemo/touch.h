
/*
本程序归fengmm521.taobao.com店所有,可以转发和使用,但请保留当前信息.
开源日期:2022年3月22日
开发者:Mage
邮箱:gp@woodcol.com
微信:woodmage
硬件购买:https://fengmm521.taobao.com/
博客:https://tboy.cc/
*/

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

class HC595Touch
{
private:
   unsigned int m_data;     //锁存器中当前数据
   void initObj();
public:
  HC595Touch(void);

  void initTouch();         //初始化所有引脚为非点击
  
  void setPin(int pNumber,bool state);  //设置一个引脚状态
  void updateData();        //更新数据到hc595的输出
  void setFront8Pins(char states);       //设置前8个引脚状态,即设置J0,j1,j2,j3,j4,j5,j6,j7的状态
  void setLast8Pins(char states);        //设置后8个引脚状态,即设置J8,J9,Ja,Jb,Jc,Je,Je,Jf的状态
  void set16Pins(unsigned int states);   //设置所有引脚状态
  void clearAllTouch();                   //设置所有引脚为非点击
  void setAllTouch();                     //设置16个引脚为点击
  unsigned int getStates();               //获取当前点击状态
};

#endif /* INC_TOUCH_H_ */
