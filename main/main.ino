/***********************************************************************************
main.ino

A program that controls the behaviour of a robot to compete in the
robocup competition.

Developed by Sasha Cox, Dervla Braem & Daniel Page

Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the
Arduino program.

Clock speed: 16MHz

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


//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE                             9600 // Bits/s
#define IR_SENSOR_LEFT_TOP_PIN                A6
#define IR_SENSOR_RIGHT_TOP_PIN               A10
#define IR_SENSOR_MIDDLE_TOP_PIN              A8
#define IR_SENSOR_REAR_PIN                    A4
#define IR_SENSOR_LEFT_BOTTOM_PIN             A2
#define IR_SENSOR_RIGHT_BOTTOM_PIN            A0
#define LOAD_CELL_LEFT_1_PIN                  44
#define LOAD_CELL_LEFT_2_PIN                  45
#define LOAD_CELL_RIGHT_1_PIN                 46
#define LOAD_CELL_RIGHT_2_PIN                 47
#define SERVO_FRONT_PIN                       34
#define INDUCTIVE_PROX_SENSOR_LEFT_PIN        38
#define LIMIT_SWITCH_LEFT_PIN                 39
#define INDUCTIVE_PROX_SENSOR_RIGHT_PIN       38
#define LIMIT_SWITCH_RIGHT_PIN                39
#define PROX_SENSOR_LEFT_PIN                  38
#define PROX_SENSOR_RIGHT_PIN                 39

// Task scheduler tasks
#define MS_READ_IR_TASK_PERIOD                2 // In ms. Also note 0 is the equivalent to the main loop
#define MS_LED_TASK_PERIOD                    2000
#define MS_STEPPER_MOTOR_TASK_PERIOD          1
#define MS_STATE_CONTROLLER_TASK_PERIOD       2
#define MS_READ_PROXIMITY_TASK_PERIOD         2

#define MS_READ_IR_TASK_NUM_EXECUTE          -1 // -1 means infinite
#define MS_LED_TASK_NUM_EXECUTE              -1
#define MS_STEPPER_MOTOR_TASK_NUM_EXECUTE    -1
#define MS_STATE_CONTROLLER_TASK_NUM_EXECUTE -1
#define MS_READ_PROXIMITY_NUM_EXECUTE        -1


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

int blocked = 0;
enum modes {SEARCHING, GOTO, PICKUP_LEFT, PICKUP_RIGHT, FINISHED};
enum modes program_state = SEARCHING;
Hx711 scale(LOAD_CELL_LEFT_1_PIN,LOAD_CELL_LEFT_1_PIN);   // Setup pins for digital communications with weight IC
// Hx711 scale(LOAD_CELL_RIGHT_2_PIN,LOAD_CELL_RIGHT_2_PIN); // Setup pins for digital communications with weight IC

// Initialise circular buffers for IR signals
circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;


void read_IR_sensors(void);
void stepper_motor_task(void);
void state_controller_task(void);
void state_controller_task(void);
void read_proximity_sensors(void);


Task t_read_IR_sensors(MS_READ_IR_TASK_PERIOD, MS_READ_IR_TASK_NUM_EXECUTE, &read_IR_sensors);
Task t_stepper_motor(MS_STEPPER_MOTOR_TASK_PERIOD, MS_STEPPER_MOTOR_TASK_NUM_EXECUTE, &stepper_motor_task);
Task t_state_controller(MS_STATE_CONTROLLER_TASK_PERIOD, MS_STATE_CONTROLLER_TASK_NUM_EXECUTE, &state_controller_task);
Task t_read_proximity_sensors(MS_READ_PROXIMITY_TASK_PERIOD, MS_READ_PROXIMITY_NUM_EXECUTE, &read_proximity_sensors);

Scheduler taskManager;


void taskInit() {  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(t_read_IR_sensors);
  taskManager.addTask(t_stepper_motor);
  taskManager.addTask(t_state_controller);
  taskManager.addTask(t_read_proximity_sensors);

  // Enable the tasks
  t_read_IR_sensors.enable();
  //t_stepper_motor.enable();
  t_state_controller.enable();
  t_read_proximity_sensors.enable(); // ##########################

 //Serial.println("Tasks have been initialised \n");
}


void setup()
{ 
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    // wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial

    pinMode(SERVO_FRONT_PIN, OUTPUT);

    // Proximity sensors
    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT);
    pinMode(LIMIT_SWITCH_LEFT_PIN, INPUT);  
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT);  
    pinMode(LIMIT_SWITCH_RIGHT_PIN, INPUT);  
    pinMode(PROX_SENSOR_LEFT_PIN, INPUT);  
    pinMode(PROX_SENSOR_RIGHT_PIN, INPUT);  

    initCircBuff(&sensor1);
    initCircBuff(&sensor2);
    initCircBuff(&sensor3);
    initCircBuff(&sensor4);

    initTune();
    initLed();
    initMotors();
    taskInit();
}


void read_IR_sensors(void) {
    IR_sensor_left_top = analogRead(IR_SENSOR_LEFT_TOP_PIN);
    IR_sensor_right_top = analogRead(IR_SENSOR_RIGHT_TOP_PIN);
    IR_sensor_middle_top = analogRead(IR_SENSOR_MIDDLE_TOP_PIN);
    IR_sensor_rear = analogRead(IR_SENSOR_REAR_PIN);
    IR_sensor_left_bottom = analogRead(IR_SENSOR_LEFT_BOTTOM_PIN);
    IR_sensor_right_bottom = analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN);
}


void read_proximity_sensors(void) {
    inductive_prox_sensor_left = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_PIN);
    limit_switch_left = digitalRead(LIMIT_SWITCH_LEFT_PIN);
    inductive_prox_sensor_right = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_PIN);
    limit_switch_right = digitalRead(LIMIT_SWITCH_RIGHT_PIN);
    prox_sensor_left = digitalRead(PROX_SENSOR_LEFT_PIN);
    prox_sensor_right = digitalRead(PROX_SENSOR_RIGHT_PIN);

    if (inductive_prox_sensor_left == 1) {
        //##Stop robot##
        setMotor(LEFT, STATIONARY, 0);
        setMotor(RIGHT, STATIONARY, 0);
        //##Change to pickup state##
        program_state = PICKUP_LEFT;
    }
    
    if (inductive_prox_sensor_right == 1) {
        //##Stop robot##
        setMotor(LEFT, STATIONARY, 0);
        setMotor(RIGHT, STATIONARY, 0);
        //##Change to pickup state##
        program_state = PICKUP_RIGHT;
    }
}


void stepper_motor_task(void)
{
    stepper_motor_step(RIGHT, ANTICLOCKWISE, 2000);
}


void state_controller_task(void) 
{
    switch(program_state) 
    {
        case SEARCHING:
            if (IR_sensor_right_top < 200 && IR_sensor_left_top < 200) { // When nothing blocks both sensors
            blocked = 0;   
            static int variable_speed_count = 0;
            static int variable_speed = 40;
            if (variable_speed_count >= 1500 && variable_speed <= 75) {
                variable_speed++;
                variable_speed_count = 0;
            } else {
                variable_speed_count++;
            }
            setMotor(RIGHT, CLOCKWISE, variable_speed);  
            setMotor(LEFT, CLOCKWISE, variable_speed);
            } else if (IR_sensor_right_top >= 200 && IR_sensor_left_top >= 200) { // When both sensors are blocked
                blocked = 1;
                //Serial.println(blocked);

               // Runs when both sensors are block for 100 counts
            } else if (IR_sensor_right_top >= 200 && !blocked) { // When the right sensor is blocked
                turnRobot(ANTICLOCKWISE, 50);
            } else if (IR_sensor_left_top >= 200 && !blocked) { // When the left sensor is blocked
                turnRobot(CLOCKWISE, 50);
            } else if (blocked) { // While both sensors are blocked
                setMotor(RIGHT, ANTICLOCKWISE, 40);   
                setMotor(LEFT, ANTICLOCKWISE, 50);   
            }

            if (IR_sensor_right_top >= IR_sensor_left_top) {
            //flash_led(IR_sensor_right_top); // Poll to flash at a rate based on the sensor input
            } else {
            //flash_led(IR_sensor_left_top);
            }
            
        case PICKUP_LEFT:
            if (limit_switch_left == 1) {
                // State change
            }
            
            //##Start lowering the arm with the stepper motor (anticlockwise)##
            //##Start raising the arm with the stepper motor (clockwise)##
            //##Optional load cell check##
            //##Change to searching state##
            // Stepper motor sequences and electromagnet activation
            //stepper();
            //play_tune(); 
            //wdt_reset(); // Resets watchdog timer
            
        case PICKUP_RIGHT:
            // Stepper motor sequences and electromagnet activation
            //stepper();
            //play_tune(); 
            //wdt_reset(); // Resets watchdog timer
            //##Start raising the arm with the stepper motor (clockwise)##
            //##Optional load cell check##
            //##Change to searching state##
            //##Start lowering the arm with the stepper motor (anticlockwise)##
            //##Limit switch is activated##
            if (limit_switch_right == 1) {
            // State change
            }
    }
}
    

void loop()
{ 
    taskManager.execute(); // Execute the scheduler
}
