// motors.cpp - Controls the motors of the robot.
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 9/7/19


#include <Servo.h>
#include "motors.h"


Servo myservoLeft; // Creates a servo object to control the left motor
Servo myservoRight; // Creates a servo object to control the right motor


// Initialises both of the DC motors
void initMotors(void)
{
  // The pins can be switched for different orientations of the robot
   myservoLeft.attach(2);  // attaches the servo pin 3 to the servo object
   myservoRight.attach(3);  // attaches the servo pin 2 to the servo object  
}


// Sets the speed of a motor
// e.g. setMotor(LEFT, CLOCKWISE, 50) sets the speed of the left motor to 50% clockwise
void setMotor(int motor, int motor_direction, float percentage_power)
{
    switch(motor)
    {
        case LEFT:
            switch(motor_direction)
            {
                case CLOCKWISE:
                    myservoLeft.writeMicroseconds(1500-percentage_power/100*500);
                    break;
                case ANTICLOCKWISE:
                    myservoLeft.writeMicroseconds(1500+percentage_power/100*500);
                    break;
            }
            break;
        case RIGHT:
            switch(motor_direction)
            {
                case CLOCKWISE:
                // 1000us is anticlockwise
                    myservoRight.writeMicroseconds(1500-percentage_power/100*500); 
                    break;
                case ANTICLOCKWISE:
                // 2000us is clockwise
                    myservoRight.writeMicroseconds(1500+percentage_power/100*500);
                    break;
            }
            break;
    }
}
