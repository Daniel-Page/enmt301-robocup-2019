/***********************************************************************************
motors.cpp

Controls the motors of the robot.

Developed by Sasha Cox, Dervla Braem & Daniel Page

// Sabretooth driver connected to 13V and the DIP switches set to:
// 1 - Off
// 2 - On
// 3 - Off
// 4 - Off
// 5 - On
// 6 - Off
************************************************************************************/


//**********************************************************************************
// Modules and libraries
//**********************************************************************************
#include <Servo.h>
#include <Arduino.h>
#include "motors.h"


//**********************************************************************************
// Definitions
//**********************************************************************************
#define LEFT_SERVO_PIN  2 // The pins can be switched for different orientations of the robot
#define RIGHT_SERVO_PIN 3

//**********************************************************************************
// Definitions
//**********************************************************************************
int stepper_motor_dir = 32;
int stepper_motor_step = 33;
int stepper_motor_count = 0;
int step_state = 0;

Servo myservoLeft, myservoRight; // Creates servo objects to control the left and right motors


// Initialises the pins for both of the DC motors
void initMotors(void)
{
    myservoLeft.attach(LEFT_SERVO_PIN);   // Attaches the servo pin 3 to the servo object
    myservoRight.attach(RIGHT_SERVO_PIN); // Attaches the servo pin 2 to the servo object  

    // Setup steppermotor
    pinMode(stepper_motor_dir,OUTPUT);
    pinMode(stepper_motor_step,OUTPUT);
}


// Sets the speed and direction of a motor based on the input paramters
// setMotor(Motor: LEFT/RIGHT Motor, Direction: CLOCKWISE/ANTICLOCKWISE/STATIONARY, % Speed 0-100)
void setMotor(int motor, int motor_direction, float percentage_speed)
{
    switch(motor)
    {
        case LEFT:
            switch(motor_direction)
            {
                case STATIONARY:
                    // 1500us is stationary
                    myservoLeft.writeMicroseconds(1500);
                    break;
                case CLOCKWISE:
                    // 1000us is anticlockwise
                    myservoLeft.writeMicroseconds(1500-percentage_speed/100*500);
                    break;
                case ANTICLOCKWISE:
                    // 2000us is clockwise
                    myservoLeft.writeMicroseconds(1500+percentage_speed/100*500);
                    break;
            }
            break;
        case RIGHT:
            switch(motor_direction)
            {
                case STATIONARY:
                    // 1500us is stationary
                    myservoRight.writeMicroseconds(1500);
                    break;
                case CLOCKWISE:
                    // 1000us is anticlockwise
                    myservoRight.writeMicroseconds(1500-percentage_speed/100*500); 
                    break;
                case ANTICLOCKWISE:
                    // 2000us is clockwise
                    myservoRight.writeMicroseconds(1500+percentage_speed/100*500);
                    break;
            }
            break;
    }
}


void turnRobot(int turn_direction, float percentage_speed)
{
  switch(turn_direction)
    {
        case CLOCKWISE:
            setMotor(RIGHT, ANTICLOCKWISE, percentage_speed);   
            setMotor(LEFT, CLOCKWISE, percentage_speed);   
            break;
        case ANTICLOCKWISE:
            setMotor(RIGHT, CLOCKWISE, percentage_speed);   
            setMotor(LEFT, ANTICLOCKWISE, percentage_speed);   
            break;
    }
}


void steps(int steps, int motor_direction) 
{
    // 0.9 degrees/step
    // 400 steps/revolution
   
    static int count_timer = 0;

    switch(motor_direction)
    {
        case CLOCKWISE:
            if (stepper_motor_count <= steps && count_timer >= 117) // Count timer: 1ms*16MHz = 16000 cycles           
            {
                digitalWrite(stepper_motor_dir,LOW); // Set direction
                digitalWrite(stepper_motor_step,LOW);
                delayMicroseconds(2);
                digitalWrite(stepper_motor_step,HIGH);
                stepper_motor_count++;
                count_timer = 0;
            } else if (stepper_motor_count > steps) {
                step_state++;
            } else {
                count_timer++;
            }
            break;
        case ANTICLOCKWISE:
            if (stepper_motor_count <= steps && count_timer >= 117) // Count timer: 1ms*16MHz = 16000 cycles
            {
                digitalWrite(stepper_motor_dir,HIGH); // Set direction
                digitalWrite(stepper_motor_step,LOW);
                delayMicroseconds(2);
                digitalWrite(stepper_motor_step,HIGH);
                stepper_motor_count++;
                count_timer = 0; 
            } else if (stepper_motor_count > steps) {
                step_state++;
            } else {
                count_timer++;
            }
            break;
    }
}


void stepper(void) {
  if (step_state == 0) {
      steps(9500,ANTICLOCKWISE);
  } else if (step_state == 1) {
      static int lock = 0;
      if (lock == 0) {
          stepper_motor_count = 0 ;
          lock = 1;
      }
      steps(8000,CLOCKWISE); // Raises the arm to almost the top
  }
}


int is_step_state(void) {
    return step_state;
}
