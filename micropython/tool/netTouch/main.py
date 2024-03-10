# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import socket
import touchDriver as t
import time
from machine import Timer
import json
import network
#实始化一个点击器控制实例对象
tobj = t.TouchObj()
#pNumber按键按下,pNumber从0~15
def touchPin(pNumber):
    tobj.setPin(pNumber,False)
    tobj.updateData()
#pNumber按键抬起,pNumber从0~15
def unTouchPin(pNumber):
    tobj.setPin(pNumber,True)
    tobj.updateData()
#按下并抬起,即点按一次
def touchOncePin(pNumber):
    touchPin(pNumber)
    time.sleep_ms(100)  #按下时间为50毫秒
    unTouchPin(pNumber)
    time.sleep_ms(100)  #抬起后再等50毫秒

#使用16位的按键状态数一次控制所有点击头状态
def setAllPinStates(state):
    tobj.set16Pins(state)
    tobj.updateData()



def getMacAddr():
    sta_if = network.WLAN(network.STA_IF)
    sta_if.active(True)
    s = sta_if.config('mac')
    mymac = ('%02x%02x%02x%02x%02x%02x') %(s[0],s[1],s[2],s[3],s[4],s[5])
    return mymac

keynum = 0

runlist = []
            #0001001001001001
keyzus = {1:[1,4,7,10,13],2:[2,5,8,11,14],3:[3,6,9,12,15]}

def readCmdFromFile():
    global runlist
    f = open('f.txt','r')
    fstr = f.read()
    f.close()
    fstrs = fstr.split('\n')
    for i,v in enumerate(fstrs):
        if len(v) > 2:
            tmpv = v.replace('\r','')
            runlist.append(tmpv)

    print(str(runlist))
runtimes = 160

TIMEOBJ = None

def stopAll():
    global TIMEOBJ
    global runtimes
    global keynum
    global nowTouchState
    TIMEOBJ.deinit()          #循环次数到了，程序结束
    keynum = 0
    nowTouchState = 0
    runtimes = 160
    setAllPinStates(0b1111111111111111)

#定时器时间达到后的回调函数
def onTimerEnd(tim):
    global keynum
    global runtimes
    global nowTouchState
    print(keynum,len(runlist))
    datstr = runlist[keynum].split(',')
    k = int(datstr[0])
    t = int(datstr[1])
    s = int(datstr[2])
    if s == 0:
        for i,v in enumerate(keyzus[k]):
            touchPin(v-1)
    else:
        for i,v in enumerate(keyzus[k]):
            unTouchPin(v-1)
    keynum +=1
    if(keynum >= len(runlist)):
        keynum = 0
        runtimes -= 1
        print('run times:%d'%(runtimes))
    if runtimes < 0:
        #循环次数到了，程序结束
        stopAll()
        return
    tim.init(period=t,mode=Timer.ONE_SHOT ,callback=onTimerEnd)
    
def getNetAddr():
    f = open('net.txt','r')
    netstr = f.read()
    f.close()
    netstr = netstr.replace('\r','')
    nets = netstr.split('\n')
    addr_info = socket.getaddrinfo(nets[0], int(nets[1]))
    addrout = addr_info[0][-1]
    print(addrout)
    return addrout

#字符串转为hex十六进制数
# hex_string = "0xFF"

# an_integer = int(hex_string, 16)

# hex_value = hex(an_integer)
# print(hex_value)

def netLoop():
    addr = getNetAddr()
    s = socket.socket()
    s.connect(addr)
    while True:
        data = s.recv(500)
        print(str(data, 'utf8'), end='')
        print('%x'%(data))
        datdic = json.loads(data)
        # {"cmd":1,"ctype":1,"dat":"ff","mac":"mac地址"} 
        if datdic['mac'] 

nowTouchState = 0 #0:当前停止状态,1:当关前运行状态,2:当前为暂停状态,3:当前为复位重启状态
#短按函数
def shotDown(tim):
    global nowTouchState
    global keynum
    if nowTouchState == 0:      #在停止状态下按了按键 
        keynum = 0              #设置当前运行从头开始
        nowTouchState = 1       #设置当前状态为运行状态
        onTimerEnd(tim)           #启动工作延时定时器  
    elif nowTouchState == 1:    #当前工作状态为正常运行,则状态要改为暂停运行
        stopAll()

#主函数,点击器程序从这里开始运行
def main():
    global TIMEOBJ
    setAllPinStates(0b1111111111111111)
    readCmdFromFile()
    tim = Timer(-1)  #新建一个虚拟定时器
    TIMEOBJ = tim
    keydowntime = 0
    keyuptime = 0
    #按键检测循环
    while True:
        tkey = tobj.key.value()
        if not tkey:
            keydowntime = time.time()
        else:
            keyuptime = time.time()
        if keydowntime and keyuptime and keyuptime > keydowntime:
            dtime = keyuptime - keydowntime
            if dtime > 0:
                keydowntime = 0
                shotDown(tim)
        else:
            time.sleep_ms(10)

    
if __name__ == '__main__':  
    main()
    