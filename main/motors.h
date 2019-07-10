// motors.h    
// Developed by Sasha Cox, Dervla Braem & Daniel Page
// Last updated 9/7/19


#ifndef MOTORS_H_
#define MOTORS_H_


enum motor {LEFT, RIGHT};
enum motor_direction {CLOCKWISE, ANTICLOCKWISE, STATIONARY};


void initMotors(void);


void setMotor(int motor, int motor_direction, float percentage_power);


#endif /* MOTORS_H_ */