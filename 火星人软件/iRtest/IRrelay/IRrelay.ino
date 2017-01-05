/*
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
//long La[18] = {-2139121561L,-2139113401L,-2139153691L,-2139137371L,-2139121051L,2155839615,2155823295,2155855935,2155815135,2155847775,2155831455,2155831455,2155864095,2155811055,2155843695,2155827375,2155806975,2155862055};
long La[21] = {1086298215,1086306375,1086282405,1086266085,1086298725,1086292095,1086275775,1086308415,1086267615,1086300255,1086283935,1086316575,1086263535,1086296175,1086279855,1086259455,1086314535,1086268125,1086300765,1086284445,1086276285};
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
  
  if (La[0] == results->value ) Serial.println("<--UP-->");
  if (La[1] == results->value ) Serial.println("<--Dow-->");
  if (La[2] == results->value ) Serial.println("<--LeiL-->");
  if (La[3] == results->value ) Serial.println("<--LeiR-->");
  if (La[4] == results->value ) Serial.println("<--Stop-->");
  if (La[5] == results->value ) Serial.println("<--G01-->");
  if (La[6] == results->value ) Serial.println("<--G02-->");
  if (La[7] == results->value ) Serial.println("<--G03-->");
  if (La[8] == results->value ) Serial.println("<--G04-->");
  if (La[9] == results->value ) Serial.println("<--G05-->");  
  if (La[10] == results->value ) Serial.println("<--G06-->");
  if (La[11] == results->value ) Serial.println("<--G07-->");
  if (La[12] == results->value ) Serial.println("<--G08-->");
  if (La[13] == results->value ) Serial.println("<--G09-->");
  if (La[14] == results->value ) Serial.println("<--G0*-->");
  if (La[15] == results->value ) Serial.println("<--G00-->");
  if (La[16] == results->value ) Serial.println("<--G0#-->");
  if (La[17] == results->value ) Serial.println("<--EyeLoNoFF-->");
  if (La[18] == results->value ) Serial.println("<--EyeSing-->");
  if (La[19] == results->value ) Serial.println("<--Sing-->");
  if (La[20] == results->value ) Serial.println("<--EyeRoNoFF-->");
  
/*  if (results->decode_type == UNKNOWN) {
    Serial.println("Could not decode message");
  } 
  else {
    if (results->decode_type == NEC) {
      Serial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) {
      Serial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == RC5) {
      Serial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }   */
  Serial.println("");
}

void setup()
{
//  pinMode(RELAY_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  saveireeprom();
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

void saveireeprom(){
  int eeAddress = 0;
  irstart_addr;
  for(int i = 0 ; i <= 21 ;i++ ){
    eeAddress = irstart_addr+i*sizeof(long);  
    EEPROM.put(eeAddress,La[i]);  
    Serial.println("EEPROM.put-----");
  }
}

