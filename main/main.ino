// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 10/7/19

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.


#include "motors.h"
#include "sensors.h"


// Definitions for IR sensor
int analogInPin = A0;  
int sensorValue = 0;


void setup()
{ 
    pinMode(49, OUTPUT);                 // Pin 49 is used to enable IO power
    digitalWrite(49, 1);                 // Enable IO power on main CPU board
    
    Serial.begin(9600); // 9600 bit/s
    
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
} 
