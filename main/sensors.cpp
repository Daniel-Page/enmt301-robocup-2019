// sensors.cpp    
// Developed by Sasha Cox, Dervla Braem & Daniel Page


#include "sensors.h"
#include "Arduino.h"


// Definitions for IR sensor
int analogInPin = A0; 
int sensorValue = 0;


void irSense(void)
{
    sensorValue = analogRead(analogInPin);
    Serial.println(sensorValue);
    delay(2); // 2 ms ADC settling time (500Hz)
}
