/***********************************************************************************
main.ino

A program that controls the behaviour of a robot to compete in the
robocup competition.

Developed by Sasha Cox, Dervla Braem & Daniel Page

Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the
Arduino program.

Naming covention diagram:

       _*______*_
      |   FRONT  |
      |  o     o |
      |  LEVEL 1 |
 LEFT |__________| RIGHT
      |  LEVEL 2 |
      |          |
      |___BACK___|
           *
o = stepper motors
* = IR sensors

************************************************************************************/


//**********************************************************************************
// Modules and libraries
//**********************************************************************************
#include <avr/wdt.h>
#include "src/TaskScheduler/TaskScheduler.h" // https://github.com/arkhipenko/TaskScheduler
#include "motors.h"
#include "speaker.h"
#include "sensors.h"
#include "led.h"
#include "src/Hx711/Hx711.h" // Load cell library
#include "circular_buffer.h"
#include <math.h>


//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE                                9600 // Bits/s
#define IR_SENSOR_RIGHT_TOP_PIN                  A6
#define IR_SENSOR_LEFT_TOP_PIN                   A10
#define IR_SENSOR_MIDDLE_TOP_PIN                 A8
#define IR_SENSOR_REAR_PIN                       A4
#define IR_SENSOR_LEFT_BOTTOM_PIN                A2
#define IR_SENSOR_RIGHT_BOTTOM_PIN               A0
#define LOAD_CELL_LEFT_1_PIN                     44
#define LOAD_CELL_LEFT_2_PIN                     45
#define LOAD_CELL_RIGHT_1_PIN                    46
#define LOAD_CELL_RIGHT_2_PIN                    47
#define INDUCTIVE_PROX_SENSOR_LEFT_PIN           21
#define LIMIT_SWITCH_LEFT_PIN                    39
#define INDUCTIVE_PROX_SENSOR_RIGHT_PIN          20
#define LIMIT_SWITCH_RIGHT_PIN                   39
#define PROX_SENSOR_LEFT_PIN                     43
#define PROX_SENSOR_RIGHT_PIN                    42

#define STEPPER_MOTOR_LOWERING_STEPS             3300
#define STEPPER_MOTOR_RAISING_STEPS              2900

// Task scheduler tasks
#define MS_READ_IR_TASK_PERIOD                   2 // In ms. Also note 0 is the equivalent to the main loop
#define MS_LED_TASK_PERIOD                       2000
#define MS_STATE_CONTROLLER_TASK_PERIOD          1
#define MS_READ_PROXIMITY_TASK_PERIOD            3


#define MS_READ_IR_TASK_NUM_EXECUTE             -1 // -1 means infinite
#define MS_LED_TASK_NUM_EXECUTE                 -1
#define MS_STATE_CONTROLLER_TASK_NUM_EXECUTE    -1
#define MS_READ_PROXIMITY_NUM_EXECUTE           -1


//**********************************************************************************
// Variables
//**********************************************************************************
int IR_sensor_left_top = 0;
int IR_sensor_right_top = 0;
int IR_sensor_middle_top = 0;
int IR_sensor_rear = 0;
int IR_sensor_left_bottom = 0;
int IR_sensor_right_bottom = 0;

int inductive_prox_sensor_left = 0;
int limit_switch_left = 0;
int inductive_prox_sensor_right = 0;
int limit_switch_right = 0;
int prox_sensor_left = 0;
int prox_sensor_right = 0;

enum modes {SEARCHING, PICKUP, FAKE, STATIC};
enum modes program_state = SEARCHING;

enum pickup_modes {INACTIVE, LOWERING_LEFT, RAISING_LEFT, LOWERING_RIGHT, RAISING_RIGHT};
enum pickup_modes pickup_state = INACTIVE;

Hx711 scale(LOAD_CELL_LEFT_1_PIN,LOAD_CELL_LEFT_1_PIN);   // Setup pins for digital communications with weight IC
// Hx711 scale(LOAD_CELL_RIGHT_2_PIN,LOAD_CELL_RIGHT_2_PIN); // Setup pins for digital communications with weight IC

// Initialise circular buffers for IR signals
circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;
circBuffer sensor5;
circBuffer sensor6;


void read_IR_sensors();
void state_controller_task();
void read_proximity_sensors();
void taskInit();


Task t_read_IR_sensors(MS_READ_IR_TASK_PERIOD, MS_READ_IR_TASK_NUM_EXECUTE, &read_IR_sensors);
Task t_state_controller(MS_STATE_CONTROLLER_TASK_PERIOD, MS_STATE_CONTROLLER_TASK_NUM_EXECUTE, &state_controller_task);
Task t_read_proximity_sensors(MS_READ_PROXIMITY_TASK_PERIOD, MS_READ_PROXIMITY_NUM_EXECUTE, &read_proximity_sensors);


Scheduler taskManager;


void setup()
{
    Serial.begin(BAUD_RATE); // Initialises serial
    Serial.println("Setup Start");

    // Proximity sensors
    pinMode(LIMIT_SWITCH_LEFT_PIN, INPUT);  
    pinMode(LIMIT_SWITCH_RIGHT_PIN, INPUT);  
    pinMode(PROX_SENSOR_LEFT_PIN, INPUT);  
    pinMode(PROX_SENSOR_RIGHT_PIN, INPUT);  

    //pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);
    //pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);  

    initCircBuff(&sensor1);
    initCircBuff(&sensor2);
    initCircBuff(&sensor3);
    initCircBuff(&sensor4);
    initCircBuff(&sensor5);
    initCircBuff(&sensor6);

    initTune();
    initLed();
    initMotors();
    taskInit();

    // wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset
    Serial.println("Setup Finish");
}


void read_IR_sensors() 
{
    IR_sensor_left_top = updateCircBuff(&sensor1, analogRead(IR_SENSOR_LEFT_TOP_PIN));
    IR_sensor_right_top = updateCircBuff(&sensor2, analogRead(IR_SENSOR_RIGHT_TOP_PIN));
    IR_sensor_middle_top = updateCircBuff(&sensor3, analogRead(IR_SENSOR_MIDDLE_TOP_PIN));
    IR_sensor_rear = updateCircBuff(&sensor4, analogRead(IR_SENSOR_REAR_PIN));
    IR_sensor_left_bottom = updateCircBuff(&sensor5, analogRead(IR_SENSOR_LEFT_BOTTOM_PIN));
    IR_sensor_right_bottom = updateCircBuff(&sensor6, analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN));
}


void read_proximity_sensors() 
{
    static int pick_up_left_status = 0;
    static int pick_up_right_status = 0;

    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);
  
    inductive_prox_sensor_left = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_PIN);
    limit_switch_left = digitalRead(LIMIT_SWITCH_LEFT_PIN);
    inductive_prox_sensor_right = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_PIN);
    limit_switch_right = digitalRead(LIMIT_SWITCH_RIGHT_PIN);
    prox_sensor_left = digitalRead(PROX_SENSOR_LEFT_PIN);
    prox_sensor_right = digitalRead(PROX_SENSOR_RIGHT_PIN);

    if (pick_up_left_status == 0 && inductive_prox_sensor_left == 1) {
        setMotor(LEFT, STATIONARY, 0);
        setMotor(RIGHT, STATIONARY, 0);
        program_state = PICKUP;
        pickup_state = LOWERING_LEFT;
        pick_up_left_status = 1;
    } else if (pick_up_left_status == 1 && inductive_prox_sensor_right == 1) {
        setMotor(LEFT, STATIONARY, 0);
        setMotor(RIGHT, STATIONARY, 0);        
        program_state = PICKUP;
        pickup_state = LOWERING_RIGHT;
        pick_up_right_status = 1;
    }// else if (prox_sensor_left == 0) {
       // program_state = FAKE;
    //} else if (prox_sensor_right == 0) {
      //  program_state = FAKE;
   // }
  
}


void state_controller_task()
{
    static int turn_count;
    switch(program_state) 
    {   
        case SEARCHING:
            static int blocked;
            static int blocked2;
            if (IR_sensor_right_top < 200 && IR_sensor_left_top < 200 && IR_sensor_right_bottom < 500 && IR_sensor_left_bottom < 500 && IR_sensor_middle_top < 50) { // When nothing blocks both sensors
                blocked = 0;
                blocked2 = 0;
                setMotor(RIGHT, CLOCKWISE, 100);
                setMotor(LEFT, CLOCKWISE, 100);
            } else if (blocked) { // While all front sensors are blocked
                turnRobot(ANTICLOCKWISE, 100);
            } else if (blocked2) {
                turnRobot(CLOCKWISE, 100);
            } else if (IR_sensor_middle_top >= 50 && !blocked) { // When the middle top sensor is blocked
                  if (random(0,2)) {
                     blocked2 = 1;
                  } else {
                     blocked = 1;
                  }
            } else if (IR_sensor_right_top >= 200 && IR_sensor_left_top >= 200 && IR_sensor_right_bottom >= 500 && IR_sensor_left_bottom >= 500 && IR_sensor_middle_top >= 50) { // When both sensors are blocked
                  if (random(0,2)) {
                     blocked2 = 1;
                  } else {
                     blocked = 1;
                  }
            } else if (IR_sensor_right_top >= 200 && !blocked) { // When the right top sensor is blocked
                  turnRobot(ANTICLOCKWISE, 100);
            } else if (IR_sensor_left_top >= 200 && !blocked) { // When the left top sensor is blocked
                  turnRobot(CLOCKWISE, 100);
            } else if (IR_sensor_right_bottom >= 200 && !blocked) { // When the right bottom sensor is blocked
                  turnRobot(CLOCKWISE, 100);
            } else if (IR_sensor_left_bottom >= 200 && !blocked) { // When the left bottom sensor is blocked
                  turnRobot(ANTICLOCKWISE, 100);                       
            }
            break;
            
        case PICKUP:
            static int step_count_left = 0;
            static int step_count_right = 0;
            switch(pickup_state)
            {
                case LOWERING_LEFT:
                    if (step_count_left < STEPPER_MOTOR_LOWERING_STEPS) {
                        stepper_motor_step(LEFT, CLOCKWISE);
                        step_count_left++;
                    } else {
                        step_count_left = STEPPER_MOTOR_LOWERING_STEPS - STEPPER_MOTOR_RAISING_STEPS;
                        pickup_state = RAISING_LEFT;
                    }
                    break;
                case RAISING_LEFT:
                    if (step_count_left < STEPPER_MOTOR_LOWERING_STEPS) {
                        stepper_motor_step(LEFT, ANTICLOCKWISE);
                        step_count_left++;
                    } else {
                        program_state = SEARCHING;
                    }
                    break;
                case LOWERING_RIGHT:
                    if (step_count_right < STEPPER_MOTOR_LOWERING_STEPS) {
                        stepper_motor_step(RIGHT, CLOCKWISE);
                        step_count_right++;
                    } else {
                        step_count_right = STEPPER_MOTOR_LOWERING_STEPS - STEPPER_MOTOR_RAISING_STEPS;
                        pickup_state = RAISING_RIGHT;
                    }
                    break;
                case RAISING_RIGHT:
                    if (step_count_right < STEPPER_MOTOR_LOWERING_STEPS) {
                        stepper_motor_step(RIGHT, ANTICLOCKWISE);
                        step_count_right++;
                    } else {
                        program_state = SEARCHING;
                    }
                    break;
            }
            break;
        case FAKE:
            Serial.println("Fake weight detected");
            break;
               
    }
}
    

void taskInit() 
{  
    // This is a class/library function. Initialise the task scheduler
    taskManager.init();     
    
    // Add tasks to the scheduler
    taskManager.addTask(t_read_IR_sensors);
    taskManager.addTask(t_state_controller);
    taskManager.addTask(t_read_proximity_sensors);
    
    // Enable the tasks
    t_read_IR_sensors.enable();
    t_state_controller.enable();
    t_read_proximity_sensors.enable();
    
    Serial.println("Tasks initialised");
}


void loop()
{ 
    taskManager.execute(); // Execute the scheduler
    //play_tune(); 
    //wdt_reset(); // Resets watchdog timer
}
