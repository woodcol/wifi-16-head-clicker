# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import touchDriver as t
import time
import random

# import urandom

def randint(min, max):
    span = max - min + 1
    div = 0x3fffffff // span
    
    offset = random.getrandbits(30) // div
    val = min + offset
    return val

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

allUntouch = 0b1111111111111111    #所有点击头抬起
danshu     = 0b1010101010101010    #单数按下
shuanshu   = 0b0101010101010101    #双数按下


def delayt():
    dtmp = randint(80,100)      #取随机数50-100之间
    time.sleep_ms(dtmp)         #按下时间随机在50-100毫秒间
#单数
def touch1():
    setAllPinStates(danshu)     #单数按下
    delayt()
    setAllPinStates(allUntouch) #单数按下抬起，完成一次点击
    delayt()
    setAllPinStates(danshu)     #单数按下
    delayt()
    setAllPinStates(allUntouch) #单数按下抬起，完成一次点击
    delayt()

#双数
def touch2():
    setAllPinStates(shuanshu) #双数按下
    delayt()
    setAllPinStates(allUntouch) #双数按下抬起，完成一次点击
    delayt()


def delayRandom(tmin,t2max):
    dtmp = randint(tmin,t2max)      #取随机数1200-1500之间
    print(dtmp)
    time.sleep_ms(dtmp)  #延时等会上边随机数的毫秒时间  

#主函数,点击器程序从这里开始运行
def main():
    setAllPinStates(allUntouch)
    isrun = False                     #按键状态，是否启动
    while True:  #无限循环
        tkey = tobj.key.value()
        if not tkey:
            isrun = not isrun
            time.sleep_ms(100)
            if not isrun:
                time.sleep_ms(5000)
        if isrun:
            touch1()                     #单数点击一次
            delayRandom(1400,1500)         #随机等待
            touch2()                     #双数点击一次
            delayRandom(300,350)         #一次循环完成，等300-350毫秒进行下次循环      
        else:
            time.sleep_ms(100)

if __name__ == '__main__':  
    main()
    