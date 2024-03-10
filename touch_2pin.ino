#define KeyPin  11     //按键开关引脚定义
#define FkeyPin 10     //频率切换按键

#define MAXFQUNUMBER 2    //可改变频率数量
#define RANDOMMM 4       //点击随机时间幅度

float fquList[MAXFQUNUMBER] = {7.0,7.9};   //点击频率
int fquIndex = 1;                          //当前点击频率编号

const int tPin[6][2] = {{2,3},{4,5},{6,7},{8,9},{A2,A1},{A4,A3}};  //1号引脚定义

void cleanAllTouch(){
  for(int i = 0;i<6;i++){
    digitalWrite(tPin[i][1], LOW);
    digitalWrite(tPin[i][0], HIGH);
  }
}

void touchPin(int p){
  if(p >=0 && p < 6){
    digitalWrite(tPin[p][1], HIGH);
    digitalWrite(tPin[p][0], LOW);
  }
}

void unTouchPin(int p){
  if(p >=0 && p < 6){
    digitalWrite(tPin[p][0], HIGH);
    digitalWrite(tPin[p][1], LOW);
  }
}

const int blinkdelay = 50;

void ledBlink(){
  for(int i = 0;i<5;i++){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(blinkdelay);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(blinkdelay); 
  }
}

void setup() {
  //初始化串口
  Serial.begin(115200);

  //设置随机数种子
  randomSeed(analogRead(A0));
  pinMode(KeyPin, INPUT_PULLUP); //设置按键引脚为输入
  pinMode(FkeyPin, INPUT_PULLUP); //设置按键引脚为输入

  pinMode(LED_BUILTIN, OUTPUT);
  //设置点击头控制引脚为输出
  for(int i = 0;i<6;i++){
    pinMode(tPin[i][0], OUTPUT);
    pinMode(tPin[i][1], OUTPUT);
  }
  delay(1);
  
  //初始化点击头状态,清除所有当关点击状态
  cleanAllTouch();
  digitalWrite(LED_BUILTIN, LOW);
}






int minTime = int(500/fquList[fquIndex]) - RANDOMMM;  //最小随机间隔时间
int maxTime = int(500/fquList[fquIndex]) + RANDOMMM;  //最大随机间隔时间

int minTTime = 60;
int maxTTime = 120;

//通过频率更新随机间隔时间
void updateRandomTime(float f){
  minTime = int(500/f) - RANDOMMM;  //最小随机间隔时间
  maxTime = int(500/f) + RANDOMMM;  //最大随机间隔时间
}

int ttime = 125;
int utime = 125;
void loop() {
  int k = digitalRead(KeyPin);
  if(!k){
    int p = random(0,6);               //取随机按键
    ttime = random(minTTime,maxTTime);   //取随机时间
    touchPin(p);
    delay(ttime);
    utime = random(minTime,maxTime);   //取随机时间
    unTouchPin(p);
    delay(utime);
  }else{
    int fk = digitalRead(FkeyPin);
    if(!fk){
        fquIndex++;
        if(fquIndex >= MAXFQUNUMBER){
          fquIndex = 0;
          updateRandomTime(fquList[fquIndex]);
        }else{
          updateRandomTime(fquList[fquIndex]);
        }
        ledBlink();
    }
  }
}