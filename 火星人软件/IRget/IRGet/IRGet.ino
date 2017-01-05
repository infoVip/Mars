/*
 *本程序 为Mars(火星人) 验证红外指令的程序 
 *      本程序的功能是 将 EEPROM中的红外码 读入内存
 *      
 *  http://www.oa25.org
 *  
 *  http://blog.oa25.org/?cat=15
 * 
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>
#include <EEPROM.h>

int RECV_PIN = 12;        // 红外接收管脚 

int eeAddress = 0;            //EEPROM 的当前地址
int irstart_addr = 48;   // 红外控制码 EEPROM 的起始地址

IRrecv irrecv(RECV_PIN);
decode_results results;



long testL;
long La[21] = {-2139121561L,-2139113401L,-2139153691L,-2139137371L,-2139121051L,2155839615,2155823295,2155855935,2155815135,2155847775,2155831455,2155831455,2155864095,2155811055,2155843695,2155827375,2155806975,2155862055,2155827375,2155806975,2155862055};
//long La[21] = {1086298215,1086306375,1086282405,1086266085,1086298725,1086292095,1086275775,1086308415,1086267615,1086300255,1086283935,1086316575,1086263535,1086296175,1086279855,1086259455,1086314535,1086268125,1086300765,1086284445,1086276285};
char* LaStr[]={"UP","Dow","LeiL","LeiR","Stop","G01","G02","G03","G04","G05","G06","G07","G08","G09","G0*","G00","G0#","EyeLoNoFF","EyeSing","Sing","EyeRoNoFF"};

//up,dow,Left,R,stop
// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dump(void *v) {
//  decode_results *results = (decode_results *)v

void dump(decode_results *results) {
  int count = results->rawlen;
  testL = long(results->value);
  Serial.println(results->value, HEX);
  Serial.println(results->value);
  Serial.println("-------Test long ");
  Serial.println(testL, HEX);
  Serial.println(testL);
  if (testL == results->value ){
    Serial.println("==");
  } else {
    Serial.println("<>");
  }
  
  for (int i = 0; i < 21; i++){
    if (La[i] == results->value ) {
      Serial.print("<--");
      Serial.print(LaStr[i]);
      Serial.println("-->");
    }
  }
  Serial.println("");
}

void setup()
{
//  pinMode(RELAY_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  getIReeprom();
}

int on = 0;
unsigned long last = millis();

void loop() {
  if (irrecv.decode(&results)) {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) {
      on = !on;
  //    digitalWrite(RELAY_PIN, on ? HIGH : LOW);
      digitalWrite(13, on ? HIGH : LOW);
      dump(&results);
    }
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}

void getIReeprom(){
  int eeAddress = 0;
  irstart_addr;
  for(int i = 0 ; i <= 21 ;i++ ){
    eeAddress = irstart_addr+i*sizeof(long);  
    EEPROM.get(eeAddress,La[i]);  
    Serial.println("EEPROM.get-----");
  }
}

