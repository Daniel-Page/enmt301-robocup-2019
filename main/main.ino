// main.ino
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 9/7/19

/*
Useing port S1 and channels S1 and S2 to drive the two motors on the robot.
The motors must be connected through the Sabertooth driver.
The sabertooth must be in mode Microcontroller pulses, independent linear control mode.
1000us to 2000us control the speed and direction. 1500us is stop.
Any speed outside this range will be ignored.
*/

#include <Servo.h>
#include "motors.h"


//int analogInPin = A0;  
//int sensorValue = 0;


void setup()
{ 
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  //Serial.begin(9600); // 9600 bit/s
  initMotors();
}


void loop() 
{ 
  setMotor(RIGHT, CLOCKWISE, 100);    // Sets motor to 100% speed clockwise
  setMotor(LEFT, ANTICLOCKWISE, 100); // Sets motor to 100% speed clockwise
  delay(1500); // 1.5s ADC settling time
  
  //sensorValue = analogRead(analogInPin);
  //Serial.println(sensorValue);
} 
