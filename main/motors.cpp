// motors.cpp    
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 9/7/19


#include <Servo.h>
#include "motors.h"


Servo myservoLeft; // create servo object to control a servo A
Servo myservoRight; // create servo object to control a servo B


void initMotors(void)
{
   myservoLeft.attach(3);  // attaches the servo pin 3 to the servo object
   myservoRight.attach(2);  // attaches the servo pin 2 to the servo object  
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
