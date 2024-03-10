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

#单数
def touch1():
    print('danshu down')
    setAllPinStates(danshu)     #单数按下
    dtmp = randint(50,100)      #取随机数50-100之间
    time.sleep_ms(dtmp)         #按下时间随机在50-100毫秒间
    setAllPinStates(allUntouch) #单数按下抬起，完成一次点击
    print('danshu up')

#双数
def touch2():
    print('shuangshu down')
    setAllPinStates(shuanshu) #双数按下
    dtmp = randint(50,100)      #取随机数50-100之间
    time.sleep_ms(dtmp)         #按下时间随机在50-100毫秒间
    setAllPinStates(allUntouch) #双数按下抬起，完成一次点击
    print('shuangshu up')

def delayRandom(tmin,t2max):
    dtmp = randint(tmin,t2max)      #取随机数1200-1500之间
    print(dtmp)
    time.sleep_ms(dtmp)  #延时等会上边随机数的毫秒时间  

#主函数,点击器程序从这里开始运行
def main():
    setAllPinStates(allUntouch)
    time.sleep(0.3)
    kst =False
    while True:  #无限循环
        if not tobj.key.value():
            kst = not kst
            if kst:
                setAllPinStates(danshu)
            else:
                setAllPinStates(shuanshu)
            time.sleep_ms(300)
        time.sleep_ms(50)
        # for n in range(3):
        #     for i in range(16):
        #         touchPin(i)
        #         time.sleep(0.1) 
        #         unTouchPin(i)
        #         time.sleep(0.1)
        #         if not tobj.key.value():
        #             return
        # time.sleep(0.5)
        # for n in range(3):
        #     setAllPinStates(danshu)
        #     time.sleep(0.1)
        #     setAllPinStates(allUntouch)
        #     time.sleep(0.1)
        #     if not tobj.key.value():
        #         return
        #     setAllPinStates(shuanshu)
        #     time.sleep(0.2)
        #     setAllPinStates(allUntouch)
        #     time.sleep(0.2)
        #     if not tobj.key.value():
        #         return

if __name__ == '__main__':  
    main()
    