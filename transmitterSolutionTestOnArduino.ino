#include <Wire.h> 

#include <DS3231.h> 

#include <VirtualWire.h> 

#include "sha1.h" 

#define TxPin 12 

byte Year ; 

byte Month ; 

byte Date ; 

byte DoW ; 

byte Hour ; 

byte Minute ; 

byte Second ; 

bool Century  = false; 

bool h12 ; 

bool PM ; 

DS3231 Clock; 

  uint8_t *hash; 

  Sha1.init(); 

  int code = 35240; 

void setup() 

{ 

 

  pinMode(12, INPUT); 

  Serial.begin(9600);    

  Serial.println("Setup Transmitter"); 

  vw_set_ptt_inverted(true); 

  vw_setup(2000);  

   Wire.begin(); 

} 

void readRTC( ) {  

  

 Serial.print(Clock.getYear(), DEC); 

 Serial.print("-"); 

 Serial.print(Clock.getMonth(Century), DEC); 

 Serial.print("-"); 

 Serial.print(Clock.getDate(), DEC); 

 Serial.print(" "); 

 Serial.print(Clock.getHour(h12, PM), DEC); //24-hr 

 Serial.print(":"); 

 Serial.print(Clock.getMinute(), DEC); 

 Serial.print(":"); 

 Serial.println(Clock.getSecond(), DEC); 

 delay(1000); 

} 

void setDate( ) {  

 ////Set Real Time Clock 

 if (Serial.available()) { 

   //int _start = millis(); 

   GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second); 

   Clock.setClockMode(false);  // set to 24h 

   Clock.setSecond(Second); 

   Clock.setMinute(Minute); 

   Clock.setHour(Hour); 

   Clock.setDate(Date); 

   Clock.setMonth(Month); 

   Clock.setYear(Year); 

   Clock.setDoW(DoW); 

 } 

} 

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) {  

 boolean GotString = false; 

 char InChar; 

 byte Temp1, Temp2; 

 char InString[20]; 

 byte j = 0; 

 while (!GotString) { 

   if (Serial.available()) { 

     InChar = Serial.read(); 

     InString[j] = InChar; 

     j += 1; 

     if (InChar == 'x') { 

       GotString = true; 

     } 

   } 

 } 

 Serial.println(InString); 

 

 Temp1 = (byte)InString[0] - 48; 

 Temp2 = (byte)InString[1] - 48; 

 Year = Temp1 * 10 + Temp2; 

 

 Temp1 = (byte)InString[2] - 48; 

 Temp2 = (byte)InString[3] - 48; 

 Month = Temp1 * 10 + Temp2; 

 

 Temp1 = (byte)InString[4] - 48; 

 Temp2 = (byte)InString[5] - 48; 

 Day = Temp1 * 10 + Temp2; 

  

 DoW = (byte)InString[6] - 48; 

  

 Temp1 = (byte)InString[7] - 48; 

 Temp2 = (byte)InString[8] - 48; 

 Hour = Temp1 * 10 + Temp2; 

  

 Temp1 = (byte)InString[9] - 48; 

 Temp2 = (byte)InString[10] - 48; 

 Minute = Temp1 * 10 + Temp2; 

  

 Temp1 = (byte)InString[11] - 48; 

 Temp2 = (byte)InString[12] - 48; 

 Second = Temp1 * 10 + Temp2; 

} 

void loop() 

{  

  setDate(); 

 readRTC();   

  Sha1.print((Second+Minute+Hour+Day+Month)*code*Year); 

  hash = Sha1.result(); 

  vw_send((uint8_t *)hash, sizeof(hash)); 

  vw_wait_tx();  

  digitalWrite(13, false); 

  delay(500);                              

} 
