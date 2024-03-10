
#ifndef configData_h
#define configData_h 

// #define ESP32

#ifdef ESP32
#define Key 34   //板子上的按钮引脚定义
#else
#define Key 2   //板子上的按钮引脚定义
#endif

#define LED 2 

#endif
