/***********************************************************************************
led.cpp
  
Developed by Sasha Cox, Dervla Braem & Daniel Page
************************************************************************************/


//**********************************************************************************
// Modules and libraries
//**********************************************************************************
#include "led.h"
#include <Arduino.h>


//**********************************************************************************
// Definitions
//**********************************************************************************
#define blue_led  20 // Blue
#define green_led 21 // Green


void initLed(void)
{
    // Setup blue led
    pinMode(blue_led, OUTPUT);
    digitalWrite(blue_led,LOW);  

    // Setup green led
    pinMode(green_led, OUTPUT);
    digitalWrite(green_led,LOW);  
}


void flash_led(void)
{
    static int led_state = LOW;

        if (led_state == LOW) {
            led_state = HIGH;
        } else if (led_state == HIGH) {
            led_state = LOW;
        }
        digitalWrite(blue_led,led_state); 
}


void green_led_on(void) {
    digitalWrite(blue_led,LOW); 
    digitalWrite(green_led,HIGH); 
}
