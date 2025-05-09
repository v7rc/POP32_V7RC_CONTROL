#include <POP32.h>

#define SERVO_DEFAULT_VALUE 1500
#define SERVO_DEFAULT_MAX_VALUE 2000
#define SERVO_DEFAULT_MIN_VALUE 1000
#define PACKET_LENGTH 20

#define SERIAL_BAUDRATE 115200
#define DEFAULT_PERIOD_TIME 30
#define LOST_SIGNAL_MAX_TIME 500 // 最大失去信號時間;
#define DEFAULT_LOOP_TIMES 33

const int led = 13;      //舊板子是A0
const int failSaveLED = 8; //舊板子是A1

long startProcessTime = 0;
long endProcessTime = 0;
int currentLostSignalTime = 0;

char readBuffer[PACKET_LENGTH];
int bufferIndex = 0;
long loopStart, loopEnd;
static boolean ledStatus = LOW;
static int loopProcessCheck = 0;
static int failSafeCheckTime = 0;   // 檢查是否有收到資料;
static bool ifLostSignal = true;

int leftWheel = 0;
int rightWheel = 0;

#define SERIAL2_RX PB7
#define SERIAL2_TX PB6
HardwareSerial Serial2(SERIAL2_RX, SERIAL2_TX);

void setup() 
{
  Serial.begin(SERIAL_BAUDRATE);
  Serial2.begin(SERIAL_BAUDRATE);

  oled.text(0,0, "V7RC BLE");
  oled.show();
}

// const int led = 7;      //舊板子是A0

void loop() 
{

  loopStart = millis();

  if (loopProcessCheck > DEFAULT_LOOP_TIMES) {

    // Serial.println("init loopProcessCheck:" + String(loopProcessCheck));
    // Serial.println("loopProcessCheck:" + String(loopProcessCheck));

    if (ledStatus == LOW) {

      ledStatus = HIGH;

    } else {

      ledStatus = LOW;

    }

    digitalWrite(led, ledStatus);
    loopProcessCheck = 0;

  }
  
  int bufferLength = Serial2.available();

  while(bufferLength > 0){

    char tempBuffer[bufferLength];
    Serial2.readBytes(tempBuffer, bufferLength);
    
    // Serial.print("getValue:");
    // Serial.println(bufferLength);

    for(int i = 0; i < bufferLength; i ++) {
      
      readBuffer[bufferIndex] = tempBuffer[i];
      bufferIndex ++;

      //Data length = 20
      if(bufferIndex >= PACKET_LENGTH) {

        //convert char[] to String
        if(readBuffer[19]=='#'){
          
          String command = String(readBuffer);
          //Command Process
          V7RCCommand(command);
   
          Serial.println("processCommand");
          ifLostSignal = false;
          failSafeCheckTime = 0;  
          digitalWrite(failSaveLED, HIGH);  
        
        }else{
        
          Serial.print("invaild Value :");
          Serial.println(readBuffer[19]);
        
        }
        bufferIndex = 0;
      }
      if(i == bufferLength - 1){
        bufferIndex = 0;
      }
    }
    
    bufferLength = Serial2.available();

  }

  loopEnd = millis();

  //if delay time less then 30 microsecond
  int needDelay = loopEnd - loopStart;

  needDelay = DEFAULT_PERIOD_TIME - needDelay;
  failSafeCheckTime += DEFAULT_PERIOD_TIME;

  // Serial.print("failSafeCheckTime:");
  // Serial.println(failSafeCheckTime);

  if(failSafeCheckTime > LOST_SIGNAL_MAX_TIME) {

      processFailSave();
      failSafeCheckTime = LOST_SIGNAL_MAX_TIME;
      digitalWrite(failSaveLED, LOW);
      ifLostSignal = true;

      
    
  } 
  
  if(needDelay > 0) {
    
    delay(needDelay);
    
  }

  loopProcessCheck +=1;

  // motor(1,30);
  // motor(2,30);
  // delay(3000);
  // motor(1,-30);
  // motor(2,-30);
  // delay(3000);

  // Serial.println("End Loop!");

  // delay(1000);
  
}

void V7RCCommand(String bufferString){

  // Serial.println(bufferString);
  
  int P0Value = 1500;
  int P1Value = 1500;
  int P2Value = 1500;
  int P3Value = 1500;
  int P4Value = 1500;
  int P5Value = 1500;
  
  if(bufferString.substring(0,3) == "SRV"){
    
    P0Value = (bufferString.substring(3,7)).toInt();  
    P1Value = (bufferString.substring(7,11)).toInt();
    P2Value = (bufferString.substring(11,15)).toInt();
    P3Value = (bufferString.substring(15,19)).toInt();

    // servo_channel[0].write(P0Value);
    // servo_channel[1].write(P1Value);

    leftWheel = map(P0Value, 1000, 2000, -100, 100);
    rightWheel = map(P1Value, 1000, 2000, -100, 100);
  
    POP32CarDriving(leftWheel, rightWheel);
    
  }else if(bufferString.substring(0,3) == "SS8"){
    
    P0Value = (bufferString.substring(3,5)).toInt();
    P1Value = (bufferString.substring(5,7)).toInt();
    P2Value = (bufferString.substring(7,9)).toInt();
    P3Value = (bufferString.substring(9,11)).toInt();
    P4Value = (bufferString.substring(11,13)).toInt();
    P5Value = (bufferString.substring(13,15)).toInt();

    int Tank0 = 1500;
    int Tank1 = 1500;

    if (P1Value >= 1500) {
        Tank1 = Tank1 + (P1Value - 1500);
        Tank0 = Tank0 - (P1Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P1Value);
        Tank0 = Tank0 + (1500 - P1Value);
    }
    if (P0Value >= 1500) {
       Tank1 = Tank1 + (P0Value - 1500);
       Tank0 = Tank0 + (P0Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P0Value);
        Tank0 = Tank0 - (1500 - P0Value);
    }
    if (Tank0 < SERVO_DEFAULT_MIN_VALUE) {
        Tank0 = SERVO_DEFAULT_MIN_VALUE;
    }
    if (Tank0 > SERVO_DEFAULT_MAX_VALUE) {
        Tank0 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 > SERVO_DEFAULT_MAX_VALUE) {
        Tank1 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 < SERVO_DEFAULT_MIN_VALUE) {
        Tank1 = SERVO_DEFAULT_MIN_VALUE;
    }

    leftWheel = map(Tank0, 0, 99, -100, 100);
    rightWheel = map(Tank1, 0, 99, -100, 100);

     POP32CarDriving(leftWheel, rightWheel);
    
  }else if(bufferString.substring(0,3) == "SRT"){
    
    int Tank0 = 1500;
    int Tank1 = 1500;
    
    P0Value = (bufferString.substring(3,7)).toInt();
    P1Value = (bufferString.substring(7,11)).toInt();
    P2Value = (bufferString.substring(11,15)).toInt();
    P3Value = (bufferString.substring(15,19)).toInt();

    if (P1Value >= 1500) {
        Tank1 = Tank1 + (P1Value - 1500);
        Tank0 = Tank0 - (P1Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P1Value);
        Tank0 = Tank0 + (1500 - P1Value);
    }
    if (P0Value >= 1500) {
       Tank1 = Tank1 + (P0Value - 1500);
       Tank0 = Tank0 + (P0Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P0Value);
        Tank0 = Tank0 - (1500 - P0Value);
    }
    if (Tank0 < SERVO_DEFAULT_MIN_VALUE) {
        Tank0 = SERVO_DEFAULT_MIN_VALUE;
    }
    if (Tank0 > SERVO_DEFAULT_MAX_VALUE) {
        Tank0 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 > SERVO_DEFAULT_MAX_VALUE) {
        Tank1 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 < SERVO_DEFAULT_MIN_VALUE) {
        Tank1 = SERVO_DEFAULT_MIN_VALUE;
    }

    leftWheel = map(Tank0, 1000, 2000, -100, 100);
    rightWheel = map(Tank1, 1000, 2000, -100, 100);

    POP32CarDriving(leftWheel, rightWheel);

  }else{
    // Serial.write("ERROR");
  }
  
}

void POP32CarDriving(int leftWheel,int rightWheel) {

  motor(1, leftWheel);
  motor(2, leftWheel);

  motor(3, rightWheel);
  motor(4, rightWheel);

}


// //convert PWM Value
// int mapValue(int Value){
//   int mapvalue = 0;
//   mapvalue = map(Value,0,99,544,2400);
//   return mapvalue;
// }


// 沒有收到數據時，要回到預設值;
void processFailSave() {

  // Stop all motors;
  AO();
  
}
