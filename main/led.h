/***********************************************************************************
led.h
  
Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/
#include "src/FastLED/FastLED.h"

#ifndef LED_H_
#define LED_H_

void setup_led_strip();

void led_update();

void nextPattern();

void rainbow();

void rainbowWithGlitter();

void addGlitter( fract8 chanceOfGlitter);

void confetti();

void sinelon();

void bpm();

void juggle();


#endif /* LED_H_ */
