

#include "circular_buffer.h"

int analogInPin1 = A4;  
int sensorValue1 = 0;
int analogInPin2 = A6;  
int sensorValue2 = 0;
int analogInPin3 = A2;  
int sensorValue3 = 0;
int analogInPin4 = A8;  
int sensorValue4 = 0;
int analogInPin5 = A10;  
int sensorValue5 = 0;
int analogInPin6 = A0;  
int sensorValue6 = 0;

circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;
circBuffer sensor5;
circBuffer sensor6;


void setup() {
  Serial.begin(9600); // 9600 bps
  initCircBuff(&sensor1);
  initCircBuff(&sensor2);
  initCircBuff(&sensor3);
  initCircBuff(&sensor4);
  initCircBuff(&sensor5);
  initCircBuff(&sensor6);
}


void loop() {
  sensorValue1 = analogRead(analogInPin1);
  sensorValue2 = analogRead(analogInPin2);
  sensorValue3 = analogRead(analogInPin3);
  sensorValue4 = analogRead(analogInPin4);
  sensorValue5 = analogRead(analogInPin5);
  sensorValue6 = analogRead(analogInPin6);


  Serial.println(updateCircBuff(&sensor6, sensorValue6));
  Serial.print(" ");

  
  delay(2); // 2 ms ADC settling time


   

  
}
