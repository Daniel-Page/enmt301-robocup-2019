// led.cpp
// Developed by Sasha Cox, Dervla Braem & Daniel Page


#include "led.h"
#include <Arduino.h>


int led = 20;


void initLed(void)
{
    // Setup RGB led
    pinMode(led, OUTPUT);
    digitalWrite(led,LOW);  
}


void flash_led(float sensor_input)
{
    static long n = 0;
    static int led_state = LOW;
    // Max IR output assumed to be 1000 
    if (n >= (1-sensor_input/500)*10000)
    {
        if (led_state == LOW) {
            led_state = HIGH;
        } else if (led_state == HIGH) {
            led_state = LOW;
        }
        digitalWrite(led,led_state); 
        n = 0;
    } else {
        n++;
    }
}
