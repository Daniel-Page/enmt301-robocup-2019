

#include "circular_buffer.h"

int analogInPin = A8;  
int sensorValue = 0;
int analogInPin1 = A8;  
int sensorValue1 = 0;

circBuffer sensor1;

void setup() {
  Serial.begin(9600); // 9600 bps
  initCircBuff(&sensor1);
}


void loop() {
  sensorValue = analogRead(analogInPin);
  sensorValue1 = analogRead(analogInPin1);
  //Serial.print(sensorValue);
  //Serial.print(" ");
  Serial.println(updateCircBuff(&sensor1, sensorValue1));



  
  delay(2); // 2 ms ADC settling time


   

  
}
