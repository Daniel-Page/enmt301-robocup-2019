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
#define SERVO_LEFT_PIN                  13 // The pins can be switched for different orientations of the robot
#define SERVO_RIGHT_PIN                 12
#define STEPPER_MOTOR_LEFT_DIR_PIN      33
#define STEPPER_MOTOR_LEFT_STEP_PIN     32
#define STEPPER_MOTOR_RIGHT_DIR_PIN     31
#define STEPPER_MOTOR_RIGHT_STEP_PIN    30


//**********************************************************************************
// Variables
//**********************************************************************************
int stepper_motor_count = 0;
int step_state = 0;
Servo myservoLeft, myservoRight; // Creates servo objects to control the left and right motors

// Initialises the pins for both of the DC motors
void initMotors(void)
{
    myservoLeft.attach(SERVO_LEFT_PIN);   // Attaches the servo pin 3 to the servo object
    myservoRight.attach(SERVO_RIGHT_PIN); // Attaches the servo pin 2 to the servo object  

    // Setup steppermotors
    pinMode(STEPPER_MOTOR_LEFT_DIR_PIN, OUTPUT);
    pinMode(STEPPER_MOTOR_LEFT_STEP_PIN, OUTPUT);
    pinMode(STEPPER_MOTOR_RIGHT_DIR_PIN, OUTPUT);
    pinMode(STEPPER_MOTOR_RIGHT_STEP_PIN, OUTPUT);
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


void stepper_motor_write(int dir_pin, int step_pin, int motor_direction) 
{
    // 0.9 degrees/step
    // 400 steps/revolution

    static int stage = 1;
    static int previous_direction;

    if (motor_direction != previous_direction) {
      stage = 1;
    }

    if ((motor_direction == CLOCKWISE && stage == 1))
    {
        digitalWrite(dir_pin, LOW); // Set direction
        digitalWrite(step_pin, LOW);
        stage = 2;
    } else if (motor_direction == CLOCKWISE && stage == 2) {
        digitalWrite(step_pin, HIGH);
        stage = 1;
    } else if (motor_direction == ANTICLOCKWISE && stage == 1) {
        digitalWrite(dir_pin, HIGH); // Set direction
        digitalWrite(step_pin, LOW);
        stage = 2;
    } else if (motor_direction == ANTICLOCKWISE && stage == 2) {
        digitalWrite(step_pin, HIGH);
        stage = 1;
    }
    previous_direction = motor_direction;
}


void stepper_motor_step(int stepper_motor_side, int stepper_motor_direction, int steps) 
{
  switch(stepper_motor_side)
  {
      case LEFT:
          stepper_motor_step(STEPPER_MOTOR_LEFT_DIR_PIN, STEPPER_MOTOR_LEFT_STEP_PIN, stepper_motor_direction);
      break;
      case RIGHT:
          stepper_motor_step(STEPPER_MOTOR_RIGHT_DIR_PIN, STEPPER_MOTOR_RIGHT_STEP_PIN, stepper_motor_direction);
      break;
  }
}
