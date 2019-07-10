// motors.cpp - Controls the motors of the robot.
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 10/7/19

// Sabretooth driver set to:
// 1 - Off
// 2 - On
// 3 - Off
// 4 - Off
// 5 - On
// 6 - Off


#include <Servo.h>
#include "motors.h"


#define LEFT_SERVO_PIN  2
#define RIGHT_SERVO_PIN 3


Servo myservoLeft;  // Creates a servo object to control the left motor
Servo myservoRight; // Creates a servo object to control the right motor


// Initialises the pins for both of the DC motors
void initMotors(void)
{
    // The pins can be switched for different orientations of the robot
    myservoLeft.attach(LEFT_SERVO_PIN);  // Attaches the servo pin 3 to the servo object
    myservoRight.attach(RIGHT_SERVO_PIN); // Attaches the servo pin 2 to the servo object  
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