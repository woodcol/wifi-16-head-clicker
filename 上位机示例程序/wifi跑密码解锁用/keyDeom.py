#!/usr/bin/python
# -*- coding: utf-8 -*-
#创建SocketServerTCP服务器：

import socket
import time

host = '192.168.31.37' #板子连上路由器之后分配的IP地址
port = 23                #板子的端口号,这个是固定烧写在板子里的,不用动

client_socket = None

def connectNet():
    global client_socket
    client_socket = socket.socket()      # 创建连接板子的客户端socket对象
    client_socket.connect((host, port))  # 连接到板子上

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
    client_socket.send('$'.encode()) 
    time.sleep(0.01) #延时10ms

#改变点击器工作方式
def changeState(pType):
    if pType == '@':
        client_socket.send('@'.encode())  #切换为'!'工作模式
    elif pType == '!':
        client_socket.send('!'.encode())  #切换为'!'工作模式
    else:
        print('erro type')
    time.sleep(0.1) #延时100ms

#改变!模式下的点击时间
def changeType1TouchTime(ptime = 100):
    #修改'!'模式按下时间为100ms
    touchDelaytimeCMD = intToHexStrTime(ptime)        #获取'!'工作模式按下时间为100ms的命令
    client_socket.send(touchDelaytimeCMD.encode()) 
    time.sleep(0.01) #延时10ms

#!模式下按一下一个按键,pin为按键编号,1,2,...16
def touchOnceWithType1(pin):
    if pin >= 1 and pin <=16:
        client_socket.send(type1Pins[pin].encode())
        time.sleep(0.2) #延时200ms
    else:
        print('pin number erro')

#@模式下,某个键设为接下状态
def touchedWithType2(pin):
    client_socket.send(type2Pins[pin][0].encode())
    time.sleep(0.001) #延时1ms

#@模式下,某个键设为抬起非按下状态
def untouchedWithType2(pin):
    client_socket.send(type2Pins[pin][1].encode())
    time.sleep(0.001) #延时1ms

#延时s秒时间
def delayTime(s):
    time.sleep(s)

#所按数字和点击头接口对应字典
keyNumber = {'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9,'0':10}

def touchP(p):
    touchedWithType2(p)    #@模式设置J1为按下
    delayTime(0.08)        #J1按下500ms
    untouchedWithType2(p)  #@模式设置J1为非按下状态
    delayTime(0.08)        #J1非按下500ms

def touchWithString(s):
    cmd = s.zfill(4)
    print(cmd)
    for i,v in enumerate(cmd):
        touchP(keyNumber[v])
        delayTime(0.005)
#主函数,程序从这里开始运行
def main():
    connectNet()
    delayTime(1)           #等1秒时间,让程序和板子连接稳定
    changeState('@')       #切换为@工作模式
    for i in range(10000):
        touchWithString(str(i))
    client_socket.close()  #断开与板子的网络连接,因为板子每次只能有一个网络连接有效,所以这里做了一下断开网络连接,防止下次再运行程序时上一个网络没有完全断开造成新连接失败的情况
    #下边是一个死循环,为了不让指令发送完之后,和板子断开了连接,所以这里让程序进入死循环,程序运行后,可以用control+c强行退行程序
    while True:
        delayTime(1) 
def test():
    t = intToHexStrTime(500)
    print(t)
if __name__ == '__main__':
    main()
    # test()


