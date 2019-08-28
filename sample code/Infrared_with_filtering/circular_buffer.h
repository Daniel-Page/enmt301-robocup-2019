/***********************************************************************************
circBuffer.h

Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/


#ifndef CIRCBUFFER_H_
#define CIRCBUFFER_H_

const int numReadings = 20;

typedef struct circBuffer_s circBuffer;
struct circBuffer_s {
    float readings[numReadings];      // the readings from the analog input
    int readIndex = 0;              // the index of the current reading
    float total = 0;                  // the running total
    float average = 0;                // the average
};


void initCircBuff(circBuffer *readings_obj);


float updateCircBuff(circBuffer *readings_obj, float value);


#endif /* CIRCBUFFER_H_ */
