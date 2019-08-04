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


enum motor {LEFT, RIGHT};
enum motor_direction {CLOCKWISE, ANTICLOCKWISE, STATIONARY};


void initMotors(void);


void setMotor(int motor, int motor_direction, float percentage_power);


void turnRobot(int turn_direction, float percentage_speed);


void steps(int steps, int motor_direction);


void stepper(void);


int is_step_state(void);


#endif /* MOTORS_H_ */
