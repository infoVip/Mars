
//----------------------------------------------------------------
//-- 本程序为oa25 Mars 设置和整定程序
//-- 在GPL许可下发布
//-- http://www.oa25.org
//-- Mars Dance smooth criminal
//-- Released under a GPL licencse
//-- Originally made for Zowi and Otto project remake for Otto
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//-----------------------------------------------------------------

#include<EEPROM.h>
#include <Servo.h>
#include <Oscillator.h>
#include <IRremote.h>

#define N_SERVOS 4
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     | 
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/

#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2

#define INTERVALTIME 10.0 

Oscillator servo[N_SERVOS];
int servoDate[N_SERVOS];


//设置舵机的中间位置  90度


String comData = ""; 
String comDataSub = "";     //
int inByte =0;
char inChar= 'A';
int n = 0;
int n1 =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);

  for(int i=0;i<4;i++) {
    runServo(i , 90);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
    {
       comData += char(Serial.read());
       delay(2);
       }
    comData.toUpperCase();   
    if (comData.length() > 1)
    {
      //取命令第一个字节
       inChar = comData[0];
       comDataSub = comData.substring(1);
       switch (inChar) {
        case 'L':  //LED 命令
          cmdLed(comDataSub);
          break;
        case 'S':  //舵机命令  
          cmdServo(comDataSub);
          break;
//        case 'I':
//          cmdIR(comDataSub);
//         break;  
        default: 
          inByte = inByte + 1;
     }
     Serial.println(comData);
     comData = "";
  }
}

//=====命令函数=======


//舵机命令
int cmdServo(String comData){
  Serial.println(comData);
  int cLne = comData.length();
  switch (cLne) {
    case 1:
      switch (comData[0]) {
        case 'C':
          for(int i=0;i<4;i++)  runServo(i , 90);
          break;
        case 'S':
          for(int i=0;i<4;i++) runServo(i , 20);
          break;   
        case 'E':
          for(int i=0;i<4;i++) runServo(i , 160);
          break;    
        case 'A':
          for(int i=0;i<4;i++){
            if (servoDate[i] < 160){
               runServo(i , (servoDate[i]+1));
            }
          }
          break;             
        case 'D': 
          for(int i=0;i<4;i++){
            if (servoDate[i] > 20){
               runServo(i , (servoDate[i]-1));
            }
          }
          break; 
        case 'T':
          for(int j=20;j<161;j++){
            for(int i=0;i<4;i++) runServo(i , j);
            delay(30);
          }
         for(int j=161;j>20;j--){
            for(int i=0;i<4;i++) runServo(i , j);
            delay(30);
          }   
          for(int j=20;j<90;j++){
            for(int i=0;i<4;i++) runServo(i , j);
            delay(10);
          }          
         for(int i=0;i<4;i++)  runServo(i , 90);        
         break;            
      }
      break;
    case 2:
      n = int(comData[1])-50;
      Serial.print("N = ");
      Serial.println(n+2);
      if (n >=0 and n <= 4) {
        switch (comData[0]) {
          case 'T':
            for(int j=20;j<161;j++) {
              runServo(n , j);
              delay(30);
            }
            for(int j=161;j>20;j--){
              runServo(n , j);
              delay(30);              
            }
            for(int j=20;j<90;j++){
              runServo(n , j);
              delay(30); 
            }
            runServo(n , 90);
            delay(30);
            break;
          case 'C':
            runServo(n , 90);
            break;
          case 'S':
            runServo(n , 20);
            break;   
          case 'E':
            runServo(n , 160);
            break;    
          case 'A':
            if (servoDate[n] < 160){
               runServo(n , (servoDate[n]+1));
            }
            break;             
          case 'D': 
            if (servoDate[n] > 20){
               runServo(n , (servoDate[n]-1));
            }
            break; 
        }
      }
      break;
    case 3:
      if (comData[0] == 'A' or comData[0] == 'D'){
        n = int(comData[1])-50;
        Serial.print("N = ");
        Serial.println(n+2);
        if (n >=0 and n <= 4) {
          n1 = int(comData[2])-50;
          if (n1 >=1 and n <= 9) { 
             switch (comData[0]) {
                case 'A':
                   servoDate[n]=servoDate[n]+n1;
                   if (servoDate[n] > 160){
                      servoDate[n] = 160;
                      }
                   runServo(n , servoDate[n]);
                break;             
                case 'D': 
                   servoDate[n]=servoDate[n]-n1;
                   if (servoDate[n] < 20){
                     servoDate[n]=20;
                     }
                   runServo(n , servoDate[n]);
                   break;                 
             }
          }
        }
      }
      break;  
  }
}

//舵机转动
int runServo(int servoNo , int servoD){
    servoDate[servoNo] = servoD;
    servo[servoNo].SetPosition(servoDate[servoNo]);
}

// LED命令
int cmdLed(String comData){
//  Serial.println(comData);
  if (comData.length() == 1)
  {
    switch (comData[0]) {
      case 'T':
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        break;
      case 'F':
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        break;
      default:
        Serial.println(comData + " -- Err");  
    }
  } else if  (comData.length() == 2) {
    switch (comData[0]) {
      case 'T':
          switch (comData[1]) {  
            case 'R':
               digitalWrite(10, HIGH);
               break;
            case 'L':
               digitalWrite(11, HIGH);
               break;                
          }
          break;
      case 'F':
          switch (comData[1]) {  
            case 'R':
               digitalWrite(10, LOW);
               break;
            case 'L':
               digitalWrite(11, LOW);
               break;                
          } 
          break;  
    }
  }
  
 }
