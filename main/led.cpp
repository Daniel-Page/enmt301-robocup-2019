// led.cpp
// Developed by Sasha Cox, Dervla Braem & Daniel Page


#include "led.h"
#include <Arduino.h>


int blue_led = 20; // Blue
int green_led = 21; // Green


void initLed(void)
{
    // Setup blue led
    pinMode(blue_led, OUTPUT);
    digitalWrite(blue_led,LOW);  

    // Setup green led
    pinMode(green_led, OUTPUT);
    digitalWrite(green_led,LOW);  
}


void flash_led(float sensor_input)
{
    static long n = 0;
    static int led_state = LOW;
    // Max IR output assumed to be 1000 
    if (n >= (1-sensor_input/200)*11700)
    {
        if (led_state == LOW) {
            led_state = HIGH;
        } else if (led_state == HIGH) {
            led_state = LOW;
        }
        digitalWrite(blue_led,led_state); 
        n = 0;
    } else {
        n++;
    }
}


void green_led_on(void) {
    digitalWrite(blue_led,LOW); 
    digitalWrite(green_led,HIGH); 
}
