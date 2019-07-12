// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.


#include <avr/wdt.h>
#include "motors.h"
#include "sensors.h"

#define BAUD_RATE 9600 // Bits/s

enum modes {SEARCHING, KNOCKOVER, PICKUP, BACKOFF, FINISHED}; // Implement later


// Definitions for IR sensor
int analogInPin = A0;  
int sensorValue = 0;


void setup()
{ 
    pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    digitalWrite(49, 1); // Enable IO power on main CPU board

    wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset
    
    Serial.begin(BAUD_RATE); // Initialises serial
    
    initMotors();
    setMotor(RIGHT, CLOCKWISE, 100);    // Sets the right motor to 100% speed clockwise
    setMotor(LEFT, ANTICLOCKWISE, 100); // Sets the left motor to 100% speed anticlockwise
    // setMotor(LEFT, STATIONARY, 0);   // Stops left motor
}


void loop() 
{ 
    // Find the equation for calculating the distance based on the sensor ouput. Test ultrasonic sensor?
    // Implement feedback control? PD or PID
    // Implement state machine for robot modes
    
    sensorValue = analogRead(analogInPin);
    Serial.println(sensorValue);
    delay(2); // 2 ms ADC settling time (500Hz)


    wdt_reset(); // Reset watchdog timer
}
