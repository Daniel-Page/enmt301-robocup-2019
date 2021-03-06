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
#define IR_SENSOR_MIDDLE_TOP_LEFT_PIN            A8
#define IR_SENSOR_MIDDLE_TOP_RIGHT_PIN           A9
#define IR_SENSOR_LEFT_BOTTOM_PIN                A2
#define IR_SENSOR_RIGHT_BOTTOM_PIN               A0
#define IR_SENSOR_MIDDLE_BOTTOM_PIN              A4
#define LOAD_CELL_LEFT_1_PIN                     44
#define LOAD_CELL_LEFT_2_PIN                     45
#define LOAD_CELL_RIGHT_1_PIN                    46
#define LOAD_CELL_RIGHT_2_PIN                    47
#define INDUCTIVE_PROX_SENSOR_LEFT_PIN           20
#define INDUCTIVE_PROX_SENSOR_RIGHT_PIN          21
#define PROX_SENSOR_LEFT_PIN                     43
#define PROX_SENSOR_RIGHT_PIN                    42
#define WEIGHT_SENSOR_LEFT_DOUT                  16       
#define WEIGHT_SENSOR_LEFT_CLK                   17
#define WEIGHT_SENSOR_RIGHT_DOUT                 26
#define WEIGHT_SENSOR_RIGHT_CLK                  27
#define CALIBRATION_FACTOR_LEFT                 -7050.0
#define CALIBRATION_FACTOR_RIGHT                -7050.0
#define STEPPER_MOTOR_LOWERING_STEPS             3250
#define STEPPER_MOTOR_RAISING_STEPS              2850

// Task scheduler tasks
#define MS_READ_IR_TASK_PERIOD                   2 // In ms. Also note 0 is the equivalent to the main loop
#define MS_LED_TASK_PERIOD                       2000
#define MS_STATE_CONTROLLER_TASK_PERIOD          1
#define MS_READ_PROXIMITY_LEFT_TASK_PERIOD       3
#define MS_READ_PROXIMITY_RIGHT_TASK_PERIOD      3
#define MS_WATCHDOG_TASK_PERIOD                  100
#define MS_PLAY_TUNE_PERIOD                      0.01

#define MS_READ_IR_TASK_NUM_EXECUTE             -1 // -1 means infinite
#define MS_LED_TASK_NUM_EXECUTE                 -1
#define MS_STATE_CONTROLLER_TASK_NUM_EXECUTE    -1
#define MS_READ_PROXIMITY_LEFT_NUM_EXECUTE      -1
#define MS_READ_PROXIMITY_RIGHT_NUM_EXECUTE     -1
#define MS_WATCHDOG_NUM_EXECUTE                 -1
#define MS_PLAY_TUNE_NUM_EXECUTE                -1


//**********************************************************************************
// Variables
//**********************************************************************************
int IR_sensor_left_top = 0;
int IR_sensor_right_top = 0;
int IR_sensor_middle_top_left = 0;
int IR_sensor_middle_top_right = 0;
int IR_sensor_rear = 0;
int IR_sensor_left_bottom = 0;
int IR_sensor_right_bottom = 0;
int IR_sensor_middle_bottom = 0;
int inductive_prox_sensor_left = 0;
int limit_switch_left = 0;
int inductive_prox_sensor_right = 0;
int limit_switch_right = 0;
int prox_sensor_left = 0;
int prox_sensor_right = 0;
int is_prox_counting_right = 0;
int prox_counter_right = 0;
int pick_up_right_status = 0;
int is_prox_counting_left = 0;
int prox_counter_left = 0;
int pick_up_left_status = 0;

enum modes {SEARCHING, PICKUP, FAKE, FINISHED, FAKE2};
enum modes program_state = SEARCHING;

enum pickup_modes {INACTIVE, LOWERING_LEFT, RAISING_LEFT, LOWERING_RIGHT, RAISING_RIGHT};
enum pickup_modes pickup_state = INACTIVE;

HX711 scale_1; // Setup pins for digital communications with weight IC
HX711 scale_2; // Setup pins for digital communications with weight IC

// Initialise circular buffers for IR signals
circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;
circBuffer sensor5;
circBuffer sensor6;
circBuffer sensor7;


void read_IR_sensors();
void read_proximity_sensors_left();
void read_proximity_sensors_right();
void watchdog();
void state_controller_task();
void taskInit();


Task t_read_IR_sensors(MS_READ_IR_TASK_PERIOD, MS_READ_IR_TASK_NUM_EXECUTE, &read_IR_sensors);
Task t_state_controller(MS_STATE_CONTROLLER_TASK_PERIOD, MS_STATE_CONTROLLER_TASK_NUM_EXECUTE, &state_controller_task);
Task t_read_proximity_sensors_left(MS_READ_PROXIMITY_LEFT_TASK_PERIOD, MS_READ_PROXIMITY_LEFT_NUM_EXECUTE, &read_proximity_sensors_left);
Task t_read_proximity_sensors_right(MS_READ_PROXIMITY_RIGHT_TASK_PERIOD, MS_READ_PROXIMITY_RIGHT_NUM_EXECUTE, &read_proximity_sensors_right);
Task t_watchdog(MS_WATCHDOG_TASK_PERIOD, MS_WATCHDOG_NUM_EXECUTE, &watchdog);
Task t_play_tune(MS_PLAY_TUNE_PERIOD, MS_PLAY_TUNE_NUM_EXECUTE, &play_tune);


Scheduler taskManager;


void setup()
{
    //Serial.begin(BAUD_RATE); // Initialises serial
    //Serial.println("Setup Start");

    // Proximity sensors
    //pinMode(PROX_SENSOR_LEFT_PIN, INPUT);  
    //pinMode(PROX_SENSOR_RIGHT_PIN, INPUT);  

    //pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);
    //pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);  

    initCircBuff(&sensor1);
    initCircBuff(&sensor2);
    initCircBuff(&sensor3);
    initCircBuff(&sensor4);
    initCircBuff(&sensor5);
    initCircBuff(&sensor6);
    initCircBuff(&sensor7);

    scale_1.begin(WEIGHT_SENSOR_LEFT_DOUT, WEIGHT_SENSOR_LEFT_CLK);
    scale_1.set_scale(CALIBRATION_FACTOR_LEFT);
    scale_1.tare(); //Reset the scale to 0

    scale_2.begin(WEIGHT_SENSOR_RIGHT_DOUT, WEIGHT_SENSOR_RIGHT_CLK);
    scale_2.set_scale(CALIBRATION_FACTOR_RIGHT);
    scale_2.tare(); //Reset the scale to 0
 
    initTune();
    initLed();
    initMotors();
    taskInit();

    wdt_enable(WDTO_8S); // Watchdog timer set to trigger after 2 seconds if not reset
    
    //Serial.println("Setup Finish");
}


void read_IR_sensors() 
{
    IR_sensor_left_top = updateCircBuff(&sensor1, analogRead(IR_SENSOR_LEFT_TOP_PIN));
    IR_sensor_right_top = updateCircBuff(&sensor2, analogRead(IR_SENSOR_RIGHT_TOP_PIN));
    IR_sensor_middle_top_left = updateCircBuff(&sensor3, analogRead(IR_SENSOR_MIDDLE_TOP_LEFT_PIN));
    IR_sensor_middle_top_right = updateCircBuff(&sensor4, analogRead(IR_SENSOR_MIDDLE_TOP_RIGHT_PIN));
    IR_sensor_left_bottom = updateCircBuff(&sensor5, analogRead(IR_SENSOR_LEFT_BOTTOM_PIN));
    IR_sensor_right_bottom = updateCircBuff(&sensor6, analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN));
    IR_sensor_middle_bottom = updateCircBuff(&sensor7, analogRead(IR_SENSOR_MIDDLE_BOTTOM_PIN));
}


void read_proximity_sensors_left() 
{   
        pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);
        pinMode(PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);  
      
        inductive_prox_sensor_left = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_PIN);
        prox_sensor_left = digitalRead(PROX_SENSOR_LEFT_PIN);

            if (pick_up_left_status == 0 && inductive_prox_sensor_left == 1) {
                pick_up_left_status = 1;
                setMotor(LEFT, STATIONARY, 0);
                setMotor(RIGHT, STATIONARY, 0);
                program_state = PICKUP;
                pickup_state = LOWERING_LEFT;
                prox_counter_left = 0;
    
            } else if (pick_up_left_status == 0 && prox_counter_left > 2000) {
                    program_state = FAKE;
                    prox_counter_left = 0;
                    Serial.println(prox_counter_left);
                    Serial.println(pick_up_left_status);
                
            } else if (pick_up_left_status == 0 && prox_sensor_left == 0 && inductive_prox_sensor_left == 0) {
                prox_counter_left++;
            }    
}


void read_proximity_sensors_right() 
{
        pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);
        pinMode(PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);
      
        inductive_prox_sensor_right = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_PIN);
        prox_sensor_right = digitalRead(PROX_SENSOR_RIGHT_PIN);
            
        if (pick_up_right_status == 0 && inductive_prox_sensor_right == 1) {
            setMotor(LEFT, STATIONARY, 0);
            setMotor(RIGHT, STATIONARY, 0);        
            program_state = PICKUP;
            pickup_state = LOWERING_RIGHT;
            prox_counter_right = 0;
            pick_up_right_status = 1;
        } else if (pick_up_right_status == 0 && prox_counter_right == 2000) {
            program_state = FAKE;
            prox_counter_right = 0;  
        } else if (pick_up_right_status == 0 && prox_sensor_right == 0 && inductive_prox_sensor_right == 0) {
            prox_counter_right++;
        } 
}


void watchdog() 
{
    wdt_reset(); // Resets watchdog timer
}


void state_controller_task()
{
    static int weight_count = 0;
    if (weight_count == 2) {
        program_state = FINISHED;
    }
    switch(program_state) 
    {
        case SEARCHING:
            static int blocked;
            static int blocked2;
            static int suspend_turn;
            static int weight_collection_timeout = 0;
            static int turn_towards_weight_left = 0;
            static int turn_towards_weight_right = 0;
            static int turn_towards_weight_block = 0;
            static int turn_towards_weight_block_2 = 0;


            if (turn_towards_weight_left > 0 && turn_towards_weight_left < 400) {
                turn_towards_weight_left++;
            } else if (turn_towards_weight_right > 0 && turn_towards_weight_right < 400) {
                turn_towards_weight_right++;
            // All of the sensors are clear
            } else if (IR_sensor_right_top < 260 && IR_sensor_left_top < 260 && IR_sensor_right_bottom < 260 && IR_sensor_left_bottom < 260 && IR_sensor_middle_top_left < 350 && IR_sensor_middle_top_right < 350) { // When nothing blocks both sensors
                if (suspend_turn < 1 || suspend_turn > 600) { 
                    blocked = 0;
                    blocked2 = 0;
                    turn_towards_weight_block = 0;
                    turn_towards_weight_block_2 = 0;
                    turn_towards_weight_right = 0;
                    turn_towards_weight_left = 0;
                    weight_collection_timeout = 0;
                    suspend_turn = 0;
                    setMotor(RIGHT, CLOCKWISE, 90);
                    setMotor(LEFT, CLOCKWISE, 90);
                } else {
                    suspend_turn++;
                }
            } else if (blocked) { // While all front sensors are blocked
                turnRobot(ANTICLOCKWISE, 100);
            } else if (blocked2) {
                turnRobot(CLOCKWISE, 100);
            } else if ((IR_sensor_middle_top_left >= 360 || IR_sensor_middle_top_right >= 360) && !blocked) { // When the middle top sensor is blocked
                  program_state = FAKE;
            } else if (IR_sensor_right_top >= 250 && IR_sensor_left_top >= 250 && IR_sensor_right_bottom >= 300 && IR_sensor_left_bottom >= 300 && IR_sensor_middle_top_left >= 100 && IR_sensor_middle_top_right >= 100) { // When both sensors are blocked
                  // Everything is blocked
                  if (random(0,2)) {
                     blocked2 = 1;
                  } else {
                     blocked = 1;
                  }
                  suspend_turn = 1;
            } else if (IR_sensor_right_top >= 300  && !blocked) { // When the right top sensor is blocked
                  turn_towards_weight_block = 0;
                  turnRobot(ANTICLOCKWISE, 100);
            } else if (IR_sensor_left_top >= 300 && !blocked) { // When the left top sensor is blocked
                  turn_towards_weight_block = 0;
                  turnRobot(CLOCKWISE, 100);
            } else if (IR_sensor_middle_bottom >= 250 && IR_sensor_middle_top_left < 300 && IR_sensor_middle_top_right < 300) {
                  program_state = FAKE;
            } else if (weight_collection_timeout == 1000) {
                  program_state = FAKE;
                  weight_collection_timeout = 0;
            } else if (!turn_towards_weight_block && IR_sensor_right_bottom >= 80 && (IR_sensor_right_bottom - IR_sensor_right_top) > (IR_sensor_right_bottom/2) && IR_sensor_right_bottom > IR_sensor_right_top && !blocked) { // When the right bottom sensor is blocked turns towards weight
                  turnRobot(CLOCKWISE, 75);
                  turn_towards_weight_right = 1;
                  weight_collection_timeout++;
            } else if (!turn_towards_weight_block && IR_sensor_left_bottom >= 80 && (IR_sensor_left_bottom - IR_sensor_left_top) > (IR_sensor_left_bottom/2) && IR_sensor_left_bottom > IR_sensor_left_top && !blocked) { // When the left bottom sensor is blocked turn towards weight
                  turnRobot(ANTICLOCKWISE, 75);
                  turn_towards_weight_left = 1;
                  weight_collection_timeout++;
            }
            if (turn_towards_weight_block && IR_sensor_left_bottom > 300 || IR_sensor_right_bottom > 300) {
                  turn_towards_weight_block = 1;
            }
            if (turn_towards_weight_block && IR_sensor_left_top > 300 || IR_sensor_right_top > 300) {
                  turn_towards_weight_block_2 = 1;
            }
            if (turn_towards_weight_block_2 < 2000 && turn_towards_weight_block_2 > 0) {
                turn_towards_weight_block_2++;
            } else if (turn_towards_weight_block_2 == 2000) {
                turn_towards_weight_block_2++;
                program_state = FAKE2;
                turn_towards_weight_block_2 = 0;
            }
            
            if (turn_towards_weight_block < 2000 && turn_towards_weight_block > 0) {
                turn_towards_weight_block++;
            } else if (turn_towards_weight_block == 2000) {
                turn_towards_weight_block++;
                program_state = FAKE;
                turn_towards_weight_block = 0;
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
                        if (scale_1.get_units() >= 25) {
                            weight_count++;
                            program_state = SEARCHING;
                        } else {
                            is_prox_counting_left = 0;
                            prox_counter_left = 0;
                            pick_up_left_status = 0;
                            step_count_left = 0;
                            program_state = FAKE;
                        }
                        //Serial.print(scale_1.get_units(), 1);
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
                        if (scale_2.get_units() >= 25) {
                            weight_count++;
                            program_state = SEARCHING;

                        } else {
                            is_prox_counting_right = 0;
                            prox_counter_right = 0;
                            pick_up_right_status = 0;                       
                            step_count_right = 0;
                            program_state = FAKE;
                        }
                        //Serial.print(scale_2.get_units(), 1); 
                    }
                    break;
                   
            }
            break;
        case FAKE:
            static int reverse_count = 0;
            static int turning_direction = 0;
            static int lock_flag = 0;
            
            if (reverse_count > 700) {
                reverse_count = 0;
                lock_flag = 0;
                program_state = SEARCHING;
            } else if (reverse_count > 500) {
                // Rotate the robot
                if (lock_flag == 0) {
                    if (random(0,2)) {
                        turning_direction = 0;
                    } else {
                        turning_direction = 1;
                    }
                    lock_flag = 1;
                }
                if (turning_direction == 0) {
                    turnRobot(ANTICLOCKWISE, 100);                       
                } else if (turning_direction == 1) {
                    turnRobot(CLOCKWISE, 100);
                }
                reverse_count++;
            } else {
                // Reverse the robot
                setMotor(RIGHT, ANTICLOCKWISE, 60);
                setMotor(LEFT, ANTICLOCKWISE, 60);
                reverse_count++;
            }
     
            break;
        case FINISHED:
            setMotor(LEFT, STATIONARY, 0);
            setMotor(RIGHT, STATIONARY, 0);
            //Serial.println("Two weights collected");
            break;
        case FAKE2:
            static int reverse_count_2 = 0;
            if (reverse_count_2 > 700) {
                reverse_count_2 = 0;
                program_state = SEARCHING;
            } else {
                // Reverse the robot
                setMotor(RIGHT, CLOCKWISE, 60);
                setMotor(LEFT, CLOCKWISE, 60);
                reverse_count_2++;
            }
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
    taskManager.addTask(t_read_proximity_sensors_left);
    taskManager.addTask(t_read_proximity_sensors_right);
    taskManager.addTask(t_watchdog);
    taskManager.addTask(t_play_tune);


    // Enable the tasks
    t_read_IR_sensors.enable();
    t_state_controller.enable();
    t_read_proximity_sensors_left.enable();
    t_read_proximity_sensors_right.enable();
    t_watchdog.enable();
    t_play_tune.enable();

    //Serial.println("Tasks initialised");
}


void loop()
{ 
    taskManager.execute(); // Execute the scheduler
}
