#!/usr/bin/env python
# -*- coding: utf-8 -*-
#本代码来自所出售产品的淘宝店店主编写
#未经受权不得复制转发
#淘宝店：https://fengmm521.taobao.com/
#再次感谢你购买本店产品
import serial
import serial.tools.list_ports
import os,time
# 获取所有串口设备实例。
# 如果没找到串口设备，则输出：“无串口设备。”
# 如果找到串口设备，则依次输出每个设备对应的串口号和描述信息。

def listComPorts():
    ports_list = list(serial.tools.list_ports.comports())
    outs = []
    if len(ports_list) <= 0:
        print("无串口设备。")
    else:
        print("可用的串口设备如下：")
        for comport in ports_list:
            print(list(comport)[0], list(comport)[1])
            outs.append([list(comport)[0],list(comport)[1]])
    return outs
def chickPort(n):
    cstr = 'COM' + str(n)
    plist = listComPorts()
    for i,v in enumerate(plist):
        if v[0] == cstr:
            print('find Port COM%d:'%(n),v)
            return True
    print('not find COM%d'%(n))
    return False

def chickPortName(pname = 'USB-SERIAL CH340'):
    plist = listComPorts()
    for i,v in enumerate(plist):
        if v[1].find(pname) != -1:
            print(v[0])
            return v[0]
    print('not find:%s'%(pname))
    return None

def runCMD(cmd):
    os.system(cmd)

def main():
    isRun = False
    while True:
        if chickPort(6):
            if not isRun:
                time.sleep(2)
                cmd = 'micropython_all.bat'
                isRun = True
                runCMD(cmd)
            else:
                print('is bren ok,please change board....')
        else:
            isRun = False
        time.sleep(1)
if __name__ == '__main__':
    main()