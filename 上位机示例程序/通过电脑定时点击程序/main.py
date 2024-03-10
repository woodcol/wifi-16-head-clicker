#!/usr/bin/env python
# -*- coding: utf-8 -*-
# author:洪卫
 

import os,sys
import serial
import time
import json
import timetool
from sys import version_info  

def pythonVersion():
    return version_info.major

key1 = None
key2 = None
dtime = 0.040
tcount = 10000

def getTime():
    global key1,key2,dtime,tdelay,tcount
    f = open('timer.txt','r')
    dat = f.read()
    f.close()
    configs = json.loads(dat)
    ttime = int(timetool.conventTimeFromStrConfig(configs['time'])*1000)
    key1 = configs['key1']
    key2 = configs['key2']
    dtime = float(configs['tdtime']/1000.0)
    tcount = configs['tCount']
    print('time:%s'%(configs['time']))
    print(key1,key2,ttime,dtime,tcount)
    return ttime

serialT = None

def readPort():
    global serialT
    time.sleep(0.003)
    n = serialT.inWaiting()
    # print(n)
    if n > 0:
        pstr = serialT.read(n)
        if pythonVersion() == 2:
            # return pstr
            print('setial read:%s'%(pstr))
        else:
            # return str(pstr)
            print('setial read:%s'%(str(pstr)))


def sendCmd(pcmd):
    global serialT
    back = None
    if pythonVersion() > 2:
        back = serialT.write(pcmd.encode())
    else:
        back = serialT.write(pcmd)
    serialT.flush()
    print('send cmd,:%s,isOk:%s'%(pcmd,str(back)))
    return back

#'@'工作模式字典
type2Pins = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}
# type2Pins = {1:['<AAAA>','<FFFF>'],2:['<5555>','<FFFF>']} #多头抢单使用的

def main():
    global serialT
    f = open('config.txt','r')
    jstr = f.read()
    f.close()
    jdic = json.loads(jstr)
    t = serial.Serial(jdic['port'],jdic['btv'],timeout=0.5)
    serialT = t
    time.sleep(2)
    sendCmd('@')
    readPort()

    sendCmd('<FFFF>')

    ttime = getTime()
    if not (key1 and key2 and ttime):
        print('init timer erro,and exit...')
        return
    isRuned = False
    while True:
        time.sleep(0.002) #进行一个延时,防止占用CPU过高
        nowtime = int(time.time()*1000)
        if ttime <= nowtime:
            if nowtime - ttime > 10: #一分钟前已点击，这里直接认为时间为无效
                print('time is passed:%s'%(timetool.getNowDate(ttime/1000)))
                ttime = getTime()
                time.sleep(1.0)
                isRuned = False
            else:
                #key按一下
                if key1 >= 1 and key1 <= 16:
                    sendCmd(type2Pins[key1][0])
                    time.sleep(dtime)
                    sendCmd(type2Pins[key1][1])
                
                if tcount > 0:#key2一直按指定次数
                    tmpcount = tcount
                    while tmpcount > 0:
                        sendCmd(type2Pins[key2][0])
                        time.sleep(dtime)
                        sendCmd(type2Pins[key2][1])
                        time.sleep(dtime)
                        tmpcount -= 1
                else:
                    while True:
                        sendCmd(type2Pins[key2][0])
                        time.sleep(dtime)
                        sendCmd(type2Pins[key2][1])
                        time.sleep(dtime)
                        tmpcount -= 1
                isRuned = True
        elif isRuned: #定时器已经运行
            ttime = getTime()
            time.sleep(1.0)
            isRuned = False
            print('init times again')
    
if __name__ == '__main__':
    main()