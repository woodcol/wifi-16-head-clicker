# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import touchDriver as t
import time

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

def conventNumber2String(n):
    tmpstr = str(n)
    if len(tmpstr) == 1:
        tmpstr = '000' + tmpstr
    elif len(tmpstr) == 2:
        tmpstr = '00' + tmpstr
    elif len(tmpstr) == 3:
        tmpstr = '0' + tmpstr
    elif len(tmpstr) > 4:
        return None
    return tmpstr

def sendNumber(n):
    strn = conventNumber2String(n)
    print(strn)
    for i,v in enumerate(strn):
        tmp = int(v)
        touchOncePin(tmp)
        time.sleep(1)

isRUN = False

def wait(t):
    global isRUN
    for i in range(t):
        print(i)
        time.sleep(1)
        tkey = tobj.key.value()
        if not tkey:
            isRUN = not isRUN
            return isRUN
    return True
        

#主函数,点击器程序从这里开始运行
def main():
    global isRUN
    while True:
        tkey = tobj.key.value()
        if not tkey:
            isRUN = not isRUN
        if isRUN:
            for i in range(0,10000):
                # touchOncePin(10)
                time.sleep(1)
                sendNumber(i)
                if not wait(35):
                    return
        time.sleep_ms(100)

if __name__ == '__main__':  
    main()
    