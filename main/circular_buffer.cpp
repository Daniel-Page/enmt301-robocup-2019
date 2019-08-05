/***********************************************************************************
circBuffer.h

Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/


#include <Arduino.h>
#include "circular_buffer.h"


int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


void initCircBuff(circBuffer *readings_obj) {
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {   // initialize all the readings to 0:
    readings_obj->readings[thisReading] = 0;
  }
}


int updateCircBuff(circBuffer *readings_obj, int value) {
  readings_obj->total = readings_obj->total - readings_obj->readings[readIndex];
  readings_obj->readings[readIndex] = value;
  readings_obj->total = readings_obj->total + readings_obj->readings[readIndex];
  readings_obj->readIndex = readings_obj->readIndex + 1;

  if (readings_obj->readIndex >= numReadings) {
    readings_obj->readIndex = 0;
  }

  return readings_obj->total / numReadings;
}
