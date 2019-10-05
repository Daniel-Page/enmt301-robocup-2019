/***********************************************************************************
motors.h

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


#ifndef MOTORS_H_
#define MOTORS_H_


// Global variables
enum motor_direction {CLOCKWISE, ANTICLOCKWISE, STATIONARY};
enum side {LEFT, RIGHT};


void initMotors(void);


void setMotor(int motor, int motor_direction, float percentage_power);


void turnRobot(int turn_direction, float percentage_speed);


void stepper_motor_write(int dir_pin, int step_pin, int motor_direction);


void stepper_motor_step(int stepper_motor_side, int motor_direction);


#endif /* MOTORS_H_ */
