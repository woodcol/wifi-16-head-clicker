#!/usr/bin/env python
# -*- coding: utf-8 -*-
#创建SocketServerTCP服务器：

import socket
import time
import random

host = '192.168.0.100' #板子连上路由器之后分配的IP地址
port = 23                #板子的端口号,这个是固定烧写在板子里的,不用动

client_socket = socket.socket()      # 创建连接板子的客户端socket对象
print('开始连接地址:%s,端口:%d'%(host,port))
client_socket.connect((host, port))  # 连接到板子上
#client_socket.send('$'.encode())     #板子恢复到出厂设置,这个可以不运行

print('点击器已连接成功')
#'!'工作模式
#['1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g']
#'@'工作模式
#['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','z']

time.sleep(1)

#切换为'@'工作模式
print('开始发送@')
client_socket.send('@'.encode())
print('@发送完成')

#定义点击器硬件点击头编号
HardDif = {1:['0','1'],2:['2','3'],3:['4','5'],4:['6','7'],5:['8','9'],6:['a','b'],7:['c','d'],8:['e','f'],9:['g','h'],10:['i','j'],11:['k','l'],12:['m','n'],13:['o','p'],14:['q','r'],15:['s','t'],16:['u','v']}

#定义上移按钮点击头编号是J1:
up = 1
#定交下移按钮点击头编号是J2:
down = 2
#定义左移按钮点击头编号是J3:
left = 3
#定义右移按钮点击头编号是J4:
right = 4
#定义技能A按钮点击头编号是J5:
A = 5
#定义技能B按钮点击头编号是J6:
B = 6

#定义一下函数,下边为参数说明:
#pin:数据类型为整数1~16,表示要按下扭钮的点击头编号(1~16),
#downtime:数据类型为浮点数(即实数),下按时间,时间单位:秒
#uptime:数据类型为浮点数(即实数),抬起时间,时间单位:秒
def touchWithDelay(pin,downtime,uptime):
    print('开始发送:%s'%(HardDif[pin][0]))
    client_socket.send(HardDif[pin][0].encode())   #J3按下（左移）
    print('%s发送完成'%(HardDif[pin][0]))
    time.sleep(downtime)   #按下时长
    print('开始发送:%s'%(HardDif[pin][1]))
    client_socket.send(HardDif[pin][1].encode())   #J3抬起（左移）
    print('%s发送完成'%(HardDif[pin][1]))
    time.sleep(uptime)   #抬起时常
#左移
def moveLeft(downtime,uptime):
    touchWithDelay(left, downtime, uptime)
#左移
def moveRight(downtime,uptime):
    touchWithDelay(right, downtime, uptime)
#上移
def moveUp(downtime,uptime):
    touchWithDelay(up, downtime, uptime)
#下移
def moveDown(downtime,uptime):
    touchWithDelay(down, downtime, uptime)

#A技能按下一次,按下和抬起时间长度从0.2秒到0.4秒随机
#参为times,表示A技能连续按下的次数,不传参数时默认为1次
def touchA(times = 1,downtime = 0,uptime = 0):
    for i in range(times):
        dtime = downtime
        utime = uptime
        if dtime == 0:
            #生成按下时间随机在0.20到0.40之间
            t = random.randint(20,40)
            dtime = t/100.0
        if utime == 0:
            #生成按下时间随机在0.20到0.40之间
            t = random.randint(20,40)
            utime = t/100.0
        touchWithDelay(A, dtime, utime)

#B技能按一下,按下和抬起时间长度从0.2秒到0.4秒随机
#参为times,表示A技能连续按下的次数,不传参数时默认为1次
def touchB(times = 1,downtime = 0,uptime = 0):
    for i in range(times):
        dtime = downtime
        utime = uptime
        if dtime == 0:
            #生成按下时间随机在0.200到0.400之间
            t = random.randint(200,400)
            dtime = t/1000.0
        if utime == 0:
            #生成按下时间随机在0.200到0.400之间
            t = random.randint(200,400)
            utime = t/1000.0
        touchWithDelay(B, dtime, utime)


for i in range(21):

    #准备工作
    #角色达到20级
    #全身装备修理
    #自动吃药设置
    #连招技能设置

    #进图
    # print('开始发送0')
    # client_socket.send('4'.encode())   #J3按下（左移）
    # print('4发送完成')
    # time.sleep(3.0)   #按下时长
    # print('开始发送1')
    # client_socket.send('5'.encode())   #J3抬起（左移）
    # print('5发送完成')
    # time.sleep(1.0)   #抬起时常
    #左移
    moveLeft(3.0,1.0) #过图,左移按钮J3,按下3.0秒,抬起1.0秒

    #选择地图按钮
    # print('开始发送0')
    # client_socket.send('g'.encode())   #J9按下（下移）
    # print('g发送完成')
    # time.sleep(0.5)   #按下时长
    # print('开始发送1')
    # client_socket.send('h'.encode())   #J9抬起（下移）
    # print('h发送完成')
    # time.sleep(0.5)   #抬起时常
    touchWithDelay(9, 0.5, 0.5) #择地图按钮J9被按一下

    #开始战斗按钮
    # print('开始发送0')
    # client_socket.send('c'.encode())   #J7按下（右移）
    # print('c发送完成')
    # time.sleep(0.5)   #按下时长
    # print('开始发送1')
    # client_socket.send('d'.encode())   #J7抬起（右移）
    # print('d发送完成')
    # time.sleep(9.0)   #抬起时常
    touchWithDelay(7, 0.5, 9.0) ##J7被按一下,抬起时间为9秒


    #图一
    #技能A按下8次,时间为随机时间
    touchA(8)


    #下移
    # print('开始发送0')
    # client_socket.send('2'.encode())   #J2按下（下移）
    # print('2发送完成')
    # time.sleep(2.0)   #按下时长
    # print('开始发送1')
    # client_socket.send('3'.encode())   #J2抬起（下移）
    # print('3发送完成')
    # time.sleep(0.5)   #抬起时常
    moveDown(2.0, 0.5)#下移,按下2.0秒,抬起0.5秒


    #技能A按下12次
    touchA(12)



    #上移
    # print('开始发送0')
    # client_socket.send('0'.encode())   #J1按下（上移）
    # print('0发送完成')
    # time.sleep(1.5)   #按下时长
    # print('开始发送1')
    # client_socket.send('1'.encode())   #J1抬起（上移）
    # print('1发送完成')
    # time.sleep(0.3)   #抬起时常
    moveUp(1.5, 0.3)#上移

    #右移
    # print('开始发送0')
    # client_socket.send('6'.encode())   #J4按下（右移）
    # print('6发送完成')
    # time.sleep(0.3)   #按下时长
    # print('开始发送1')
    # client_socket.send('7'.encode())   #J4抬起（右移）
    # print('7发送完成')
    # time.sleep(0.3)   #抬起时常
    moveRight(0.3, 0.3)#右移

    #技能A按下16次
    touchA(16)



    #左移
    # print('开始发送0')
    # client_socket.send('4'.encode())   #J3按下（上移）
    # print('0发送完成')
    # time.sleep(0.2)   #按下时长
    # print('开始发送1')
    # client_socket.send('5'.encode())   #J3抬起（上移）
    # print('1发送完成')
    # time.sleep(0.2)   #抬起时常
    moveLeft(0.2, 0.2)#左移


    #技能A按下10次
    touchA(10)



    #下移
    # print('开始发送0')
    # client_socket.send('2'.encode())   #J2按下（下移）
    # print('2发送完成')
    # time.sleep(0.2)   #按下时长
    # print('开始发送1')
    # client_socket.send('3'.encode())   #J2抬起（下移）
    # print('3发送完成')
    # time.sleep(0.2)   #抬起时常
    moveDown(0.2, 0.2)#下移

    #右移
    # print('开始发送0')
    # client_socket.send('6'.encode())   #J4按下（右移）
    # print('6发送完成')
    # time.sleep(2.5)   #按下时长
    # print('开始发送1')
    # client_socket.send('7'.encode())   #J4抬起（右移）
    # print('7发送完成')
    # time.sleep(0.5)   #抬起时常
    moveRight(2.5, 0.5)#右移

    #技能A按下4次
    touchA(4)


    #图二

    #技能A按下12次
    touchA(12)



    #右移
    # print('开始发送0')
    # client_socket.send('6'.encode())   #J4按下（右移）
    # print('6发送完成')
    # time.sleep(4.0)   #按下时长
    # print('开始发送1')
    # client_socket.send('7'.encode())   #J4抬起（右移）
    # print('7发送完成')
    # time.sleep(0.5)   #抬起时常
    moveRight(4.0, 0.4)#右移


    #左移
    # print('开始发送0')
    # client_socket.send('4'.encode())   #J3按下（左移）
    # print('4发送完成')
    # time.sleep(0.2)   #按下时长
    # print('开始发送1')
    # client_socket.send('5'.encode())   #J3抬起（左移）
    # print('5发送完成')
    # time.sleep(0.2)   #抬起时常
    moveLeft(0.2, 0.2)#左移






    #技能A按5次
    touchA(5)
    

    #左移
    # print('开始发送0')
    # client_socket.send('4'.encode())   #J3按下（左移）
    # print('4发送完成')
    # time.sleep(0.1)   #按下时长
    # print('开始发送1')
    # client_socket.send('5'.encode())   #J3抬起（左移）
    # print('5发送完成')
    # time.sleep(0.1)   #抬起时常
    moveLeft(0.1, 0.1)#左移

    #技能A按7次
    touchA(7)
    

    #左移
    # print('开始发送0')
    # client_socket.send('4'.encode())   #J3按下（左移）
    # print('4发送完成')
    # time.sleep(0.2)   #按下时长
    # print('开始发送1')
    # client_socket.send('5'.encode())   #J3抬起（左移）
    # print('5发送完成')
    # time.sleep(0.2)   #抬起时常
    moveLeft(0.2, 0.2)


    #上移
    # print('开始发送0')
    # client_socket.send('0'.encode())   #J1按下（上移）
    # print('0发送完成')
    # time.sleep(3.0)   #按下时长
    # print('开始发送1')
    # client_socket.send('1'.encode())   #J1抬起（上移）
    # print('1发送完成')
    # time.sleep(0.5)   #抬起时常
    moveUp(3.0, 0.5)





    #技能A按6次
    touchA(6)


