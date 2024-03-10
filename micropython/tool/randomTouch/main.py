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


#主函数,点击器程序从这里开始运行
def main():
    setAllPinStates(0b1111111111111111)
     
    count = 120
    ispause = False
    while count > 0:
        tkey = tobj.key.value()
        if not tkey:
            ispause = not ispause
            time.sleep_ms(100)
        if ispause:
            time.sleep_ms(100)
            continue
        setAllPinStates(0b0000000000000000) #所有点击头按下
        dtmp = randint(80,200)      #随机数80-200之间
        print(dtmp)
        time.sleep_ms(dtmp)
        setAllPinStates(0b1111111111111111) #所有点击头抬起1为抬起0为按下，J16-J1
        dtmp = randint(400,800)
        print(dtmp)
        time.sleep_ms(dtmp)
        count = count - 1

if __name__ == '__main__':  
    main()
    