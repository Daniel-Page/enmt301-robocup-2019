// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.


#include <avr/wdt.h>
#include "motors.h"
//#include "sensors.h"


#define BAUD_RATE 9600 // Bits/s

// Definitions for IR sensor
int rightSensor = A0; 
int rightValue = 0;
int leftSensor = A2; 
int leftValue = 0;
int blocked = (rightValue < 200 && leftValue < 200);

enum modes {SEARCHING, KNOCKOVER, PICKUP, BACKOFF, FINISHED}; // Implement states later


void setup()
{ 
    pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    digitalWrite(49, 1); // Enable IO power on main CPU board

    //wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    // Initialise serial input and buffer
    Serial.begin(BAUD_RATE); // Initialises serial
    
    initMotors();
    // setMotor(RIGHT, CLOCKWISE, 50);   // Sets the right motor to 100% speed clockwise
    // setMotor(LEFT, CLOCKWISE, 50);    // Sets the left motor to 100% speed anticlockwise
    // turnRobot(CLOCKWISE, 50);
    // setMotor(LEFT, STATIONARY, 0); // Stops left motor
}


void loop() 
{ 
  rightValue = analogRead(rightSensor);
  Serial.println(rightValue);
  leftValue = analogRead(leftSensor);
  Serial.println(rightValue);
  delay(1);        // delay in between reads for stability

  if (rightValue < 200 && leftValue < 200) {
    blocked = 0;
    setMotor(RIGHT, CLOCKWISE, 75);   // Sets the right motor to 100% speed clockwise
    setMotor(LEFT, CLOCKWISE, 75); 
  } else if (blocked) {
      turnRobot(ANTICLOCKWISE, 50);
  } else if (rightValue > 200 && !blocked) {
    turnRobot(ANTICLOCKWISE, 50);
  } else if (leftValue > 200 && !blocked) {
    turnRobot(CLOCKWISE, 50);
  }
    //wdt_reset(); // Resets watchdog timer
}
