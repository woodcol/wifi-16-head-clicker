#!/usr/bin/python
# -*- coding: utf-8 -*-
#创建SocketServerTCP服务器：
import os,sys
import socket
import time
import json
import pyutil
import threading
import pytimer

#串口接收线程
class WorkerThread(threading.Thread):
    """
    This just simulates some long-running task that periodically sends
    a message to the GUI thread.
    """
    def __init__(self):
        threading.Thread.__init__(self)

    def stop(self):
        pass
    def run(self):

        # downtool.downLoadWithStrList(self.liststr, self.countCallBack,self.savePth)
        try:
            while True:
                time.sleep(0.0001)
                dat = pyutil.func_readPort()
                # if dat != '':
                #     print('back data=' + dat)
        except Exception as e:
            print('读取串口数据错误')


#'!'工作模式
#['1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g']
#'@'工作模式
#['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z']

#'!'工作模式字典
type1Pins = {1:'1',2:'2',3:'3',4:'4',5:'5',6:'6',7:'7',8:'8',9:'9',10:'a',11:'b',12:'c',13:'d',14:'e',15:'f',16:'g'}
#'@'工作模式字典
type2Pins = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}
#这个函数是'!'工作模式下用来把延时毫秒数转换为要发送要字符串命令
def intToHexStrTime(dtime):
    strhex = str(hex(dtime))[2:]
    if len(strhex) == 3:
        strhex = '[0' + strhex + ']'
    elif len(strhex) == 2:
        strhex = '[00' + strhex + ']'
    return strhex

#恢复出厂设备,发送这个指令后,板子将需要重新配网
def resetBoard():
    # client_socket.send('$'.encode()) 
    pyutil.func_sendCMD('@')
    time.sleep(0.01) #延时10ms

#改变点击器工作方式
def changeState(pType):
    if pType == '@':
        # client_socket.send('@'.encode())  #切换为'!'工作模式
        pyutil.func_sendCMD('@')
    elif pType == '!':
        # client_socket.send('!'.encode())  #切换为'!'工作模式
        pyutil.func_sendCMD('!')
    else:
        print('erro type')
    time.sleep(0.1) #延时100ms

#改变!模式下的点击时间
def changeType1TouchTime(ptime = 100):
    #修改'!'模式按下时间为100ms
    touchDelaytimeCMD = intToHexStrTime(ptime)        #获取'!'工作模式按下时间为100ms的命令
    # client_socket.send(touchDelaytimeCMD.encode()) 
    pyutil.func_sendCMD(touchDelaytimeCMD)
    time.sleep(0.01) #延时10ms

#!模式下按一下一个按键,pin为按键编号,1,2,...16
def touchOnceWithType1(pin):
    if pin >= 1 and pin <=16:
        # client_socket.send(type1Pins[pin].encode())
        pyutil.func_sendCMD(type1Pins[pin])
        time.sleep(0.2) #延时200ms
    else:
        print('pin number erro')

#@模式下,某个键设为接下状态
def touchedWithType2(pin):
    # client_socket.send(type2Pins[pin][0].encode())
    pyutil.func_sendCMD(type2Pins[pin][0])

#@模式下,某个键设为抬起非按下状态
def untouchedWithType2(pin):
    # client_socket.send(type2Pins[pin][1].encode())
    pyutil.func_sendCMD(type2Pins[pin][1])


def getSSTFile(fpth):
    f = open(fpth,'rb')
    jstr = f.read()
    f.close()
    dictmp = json.loads(jstr)
    return dictmp

sendTime = 0
def timerCallBack(timex,datas):
    # print('*********')
    print('untouchs',datas)
    for i,v in enumerate(datas):
        untouchedWithType2(v)
        time.sleep(0.003)

def sendKeyList(ptime,pKeys):
    # outdic[ptime] = pKeys
    untouchs = []
    print(pKeys)
    global sendTime
    dtime = (ptime - sendTime)/1000.0
    sendTime = ptime
    print('delay time=',dtime)
    time.sleep(dtime)

    for i,v in enumerate(pKeys):
        touchedWithType2(v)
        time.sleep(0.003)
        untouchs.append(v)
    timerx = pytimer.pytimer()
    timerx.setTimer(0.04, timerCallBack,untouchs,isOnce = True)

def playSST(d):
    name = d[0]['name']
    print(name)
    songNotes = d[0]['songNotes']
    sendKeys = []
    lasttime = 0;
    for i,v in enumerate(songNotes):
        ktmp = int(v['key'][4:]) + 1
        if v['time'] == lasttime:
            sendKeys.append(ktmp)
        else:
            sendKeyList(lasttime,sendKeys)
            lasttime = v['time']
            sendKeys = [ktmp]
            # time.sleep(0.001)
    time.sleep(5)



#主函数,程序从这里开始运行
def main(fpth):
    global sendTime
    time.sleep(1)

    #初始化串口返回线程
    callbackTh = WorkerThread()
    callbackTh.setDaemon(True)
    callbackTh.start()

    # serth = threading.Thread(target=untouchThread,args=())
    # serth.setDaemon(True)
    # serth.start()

    if pyutil.func_isSerialOpen():
        print('尝试打开之前设置的串口')
    else:
        print('未设置串口号，或串口被占用，\n串口号可在设备管理器中查看')
        return
    time.sleep(2.0)
    pyutil.func_sendCMD('@') #发送工作模式切换字符
    time.sleep(0.1)

    tmppth = 'res/dx.txt'
    d = {}
    if fpth and os.path.exists(fpth):
        d = getSSTFile(fpth)
    else:
        d = getSSTFile(tmppth)
    while True:
        sendTime = 0
        playSST(d)

    time.sleep(5)

if __name__ == '__main__':
    args = sys.argv
    fpth = None
    if len(args) == 2 and os.path.exists(args[1]):
        fpth = args[1]
    main(fpth)


