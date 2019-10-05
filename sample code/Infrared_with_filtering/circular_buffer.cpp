/***********************************************************************************
circBuffer.h

Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/


#include <Arduino.h>
#include "circular_buffer.h"


void initCircBuff(circBuffer *readings_obj) {
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {   // initialize all the readings to 0:
    readings_obj->readings[thisReading] = 0;
  }
}


float updateCircBuff(circBuffer *readings_obj, float value) {
  readings_obj->total = readings_obj->total - readings_obj->readings[readings_obj->readIndex];
  readings_obj->readings[readings_obj->readIndex] = value;
  readings_obj->total = readings_obj->total + readings_obj->readings[readings_obj->readIndex];
  readings_obj->readIndex = readings_obj->readIndex + 1;

  if (readings_obj->readIndex >= numReadings) {
    readings_obj->readIndex = 0;
  }

  return readings_obj->total / numReadings;
}
