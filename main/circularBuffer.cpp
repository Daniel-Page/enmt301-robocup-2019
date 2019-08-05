/***********************************************************************************
circBuffer.h

Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/


#include <Arduino.h>


const int numReadings = 15;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


void initCircBuff(void) {
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {   // initialize all the readings to 0:
    readings[thisReading] = 0;
  }
}


int updateCircBuff(int value) {
  total = total - readings[readIndex];
  readings[readIndex] = value;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  return total / numReadings;
  delay(1);        // delay in between reads for stability
}
