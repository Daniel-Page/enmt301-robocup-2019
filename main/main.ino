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
#include <math.h>



//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE                             9600 // Bits/s
#define IR_SENSOR_RIGHT_TOP_PIN               A6
#define IR_SENSOR_LEFT_TOP_PIN                A10
#define IR_SENSOR_MIDDLE_TOP_PIN              A8
#define IR_SENSOR_REAR_PIN                    A4
#define IR_SENSOR_LEFT_BOTTOM_PIN             A2
#define IR_SENSOR_RIGHT_BOTTOM_PIN            A0
#define LOAD_CELL_LEFT_1_PIN                  44
#define LOAD_CELL_LEFT_2_PIN                  45
#define LOAD_CELL_RIGHT_1_PIN                 46
#define LOAD_CELL_RIGHT_2_PIN                 47
#define SERVO_FRONT_PIN                       34
#define INDUCTIVE_PROX_SENSOR_LEFT_PIN        20
#define LIMIT_SWITCH_LEFT_PIN                 39
#define INDUCTIVE_PROX_SENSOR_RIGHT_PIN       21
#define LIMIT_SWITCH_RIGHT_PIN                39
#define PROX_SENSOR_LEFT_PIN                  43
#define PROX_SENSOR_RIGHT_PIN                 42

// Task scheduler tasks
#define MS_READ_IR_TASK_PERIOD                2 // In ms. Also note 0 is the equivalent to the main loop
#define MS_LED_TASK_PERIOD                    2000
#define MS_STEPPER_MOTOR_TASK_PERIOD          1
#define MS_STATE_CONTROLLER_TASK_PERIOD       2
#define MS_READ_PROXIMITY_TASK_PERIOD         3
#define MS_WEIGHT_DETECT_TASK_PERIOD          2


#define MS_READ_IR_TASK_NUM_EXECUTE          -1 // -1 means infinite
#define MS_LED_TASK_NUM_EXECUTE              -1
#define MS_STEPPER_MOTOR_TASK_NUM_EXECUTE     20
#define MS_STATE_CONTROLLER_TASK_NUM_EXECUTE -1
#define MS_READ_PROXIMITY_NUM_EXECUTE        -1
#define MS_WEIGHT_DETECT_NUM_EXECUTE         -1



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
enum modes {SEARCHING, GOTO, PICKUP, FAKE, FINISHED};
enum modes program_state = SEARCHING;

enum pickup_modes {INACTIVE, LOWERING_LEFT, RAISING_LEFT, LOWERING_RIGHT, RAISING_RIGHT};
enum pickup_modes pickup_state = LOWERING_LEFT;

Hx711 scale(LOAD_CELL_LEFT_1_PIN,LOAD_CELL_LEFT_1_PIN);   // Setup pins for digital communications with weight IC
// Hx711 scale(LOAD_CELL_RIGHT_2_PIN,LOAD_CELL_RIGHT_2_PIN); // Setup pins for digital communications with weight IC

// Initialise circular buffers for IR signals
circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;
circBuffer sensor5;
circBuffer sensor6;


void read_IR_sensors(void);
void stepper_motor_task(void);
void state_controller_task(void);
void state_controller_task(void);
void read_proximity_sensors(void);
void weight_detect(void);


Task t_read_IR_sensors(MS_READ_IR_TASK_PERIOD, MS_READ_IR_TASK_NUM_EXECUTE, &read_IR_sensors);
Task t_stepper_motor(MS_STEPPER_MOTOR_TASK_PERIOD, MS_STEPPER_MOTOR_TASK_NUM_EXECUTE, &stepper_motor_task);
Task t_state_controller(MS_STATE_CONTROLLER_TASK_PERIOD, MS_STATE_CONTROLLER_TASK_NUM_EXECUTE, &state_controller_task);
Task t_read_proximity_sensors(MS_READ_PROXIMITY_TASK_PERIOD, MS_READ_PROXIMITY_NUM_EXECUTE, &read_proximity_sensors);
Task t_weight_detect(MS_WEIGHT_DETECT_TASK_PERIOD, MS_WEIGHT_DETECT_NUM_EXECUTE, &weight_detect);


Scheduler taskManager;


void taskInit() {  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(t_read_IR_sensors);
  taskManager.addTask(t_stepper_motor);
  taskManager.addTask(t_state_controller);
  taskManager.addTask(t_read_proximity_sensors);
  taskManager.addTask(t_weight_detect);


  // Enable the tasks
  t_read_IR_sensors.enable();
  //t_stepper_motor.enable();
  //t_state_controller.enable();
  t_read_proximity_sensors.enable(); // ##########################
  t_weight_detect.enable();


 //Serial.println("Tasks have been initialised \n");
}


void setup()
{ 
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    // wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset



    Serial.begin(BAUD_RATE); // Initialises serial

    Serial.println("setup....");

    pinMode(SERVO_FRONT_PIN, OUTPUT);

    // Proximity sensors
    pinMode(LIMIT_SWITCH_LEFT_PIN, INPUT);  
    pinMode(LIMIT_SWITCH_RIGHT_PIN, INPUT);  
    pinMode(PROX_SENSOR_LEFT_PIN, INPUT);  
    pinMode(PROX_SENSOR_RIGHT_PIN, INPUT);  

    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT);
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT);  

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
    Serial.println("setup....exit");
}


float compute_distance_green(int sensor_value)
{
    return 1506.6*pow(sensor_value,-1.0202);
}


float compute_distance_brown(int sensor_value)
{
    return 1438*pow(sensor_value,-0.8095);
}



void read_IR_sensors(void) 
{
    IR_sensor_left_top = updateCircBuff(&sensor1, analogRead(IR_SENSOR_LEFT_TOP_PIN));
    IR_sensor_right_top = updateCircBuff(&sensor2, analogRead(IR_SENSOR_RIGHT_TOP_PIN));
    IR_sensor_middle_top = updateCircBuff(&sensor3, analogRead(IR_SENSOR_MIDDLE_TOP_PIN));
    IR_sensor_rear = updateCircBuff(&sensor4, analogRead(IR_SENSOR_REAR_PIN));
    IR_sensor_left_bottom = updateCircBuff(&sensor5, analogRead(IR_SENSOR_LEFT_BOTTOM_PIN));
    IR_sensor_right_bottom = updateCircBuff(&sensor6, analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN));
}


void weight_detect(void) 
{
float top_left_corrected;
float bottom_left_corrected;
float difference;

top_left_corrected = compute_distance_green(IR_sensor_left_top);
bottom_left_corrected = compute_distance_brown(IR_sensor_left_bottom) + 7;
difference = top_left_corrected - bottom_left_corrected;

//Serial.println(top_left_corrected - bottom_left_corrected);


//Serial.print(compute_distance_green(IR_sensor_left_top));
//Serial.print(" ");
//Serial.print(compute_distance_brown(IR_sensor_left_bottom));
//Serial.print("\n");

if (difference > 5 && difference < 15) {
    //Serial.println("Weight");
}

}


void read_proximity_sensors(void) 
{
    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_PIN, INPUT_PULLUP);
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_PIN, INPUT_PULLUP);
  
    inductive_prox_sensor_left = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_PIN);
    limit_switch_left = digitalRead(LIMIT_SWITCH_LEFT_PIN);
    inductive_prox_sensor_right = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_PIN);
    limit_switch_right = digitalRead(LIMIT_SWITCH_RIGHT_PIN);
    prox_sensor_left = digitalRead(PROX_SENSOR_LEFT_PIN);
    prox_sensor_right = digitalRead(PROX_SENSOR_RIGHT_PIN);
    
    Serial.print(inductive_prox_sensor_left);
    Serial.print(" ");
    Serial.print(inductive_prox_sensor_right);
    Serial.print(" ");
    Serial.print(prox_sensor_left);
    Serial.print(" ");
    Serial.print(prox_sensor_right);
    Serial.print("\n");
}


void stepper_motor_task(void)
{
    stepper_motor_step(RIGHT, CLOCKWISE);
    //stepper_motor_step(LEFT, ANTICLOCKWISE);
}


void state_controller_task(void)
{
    switch(program_state) 
    {
        case SEARCHING:
            static int rand_bit = random(0,1);
            if (IR_sensor_right_top < 200 && IR_sensor_left_top < 200 && IR_sensor_right_bottom < 500 && IR_sensor_left_bottom < 500 && IR_sensor_middle_top < 50) { // When nothing blocks both sensors
                //blocked = 0;   
                setMotor(RIGHT, CLOCKWISE, 100);
                setMotor(LEFT, CLOCKWISE, 100);
            //} else if (IR_sensor_right_top >= 200 && IR_sensor_left_top >= 200 && IR_sensor_right_bottom >= 500 && IR_sensor_left_bottom >= 500 && IR_sensor_middle_top >= 50) { // When both sensors are blocked
              //      blocked = 1;
            } else if (IR_sensor_right_bottom >= 500 && !blocked) { // When the right bottom sensor is blocked
                  turnRobot(ANTICLOCKWISE, 100);  
            } else if (IR_sensor_left_bottom >= 500 && !blocked) { // When the left bottom sensor is blocked
                  turnRobot(ANTICLOCKWISE, 100);                        
            } else if (IR_sensor_right_top >= 200 && !blocked) { // When the right top sensor is blocked
                  turnRobot(ANTICLOCKWISE, 100);
            } else if (IR_sensor_left_top >= 200 && !blocked) { // When the left top sensor is blocked
                  turnRobot(CLOCKWISE, 100);
            } else if (IR_sensor_middle_top >= 50 && !blocked) { // When the middle top sensor is blocked
                  turnRobot(CLOCKWISE, 100);
            //} else if (blocked) { // While all front sensors are blocked
                //  turnRobot(ANTICLOCKWISE, 100);
            }
            break;
        case PICKUP:
            switch(pickup_state)
            {
                case LOWERING_LEFT:
                    //##Start lowering the arm with the stepper motor (anticlockwise)##
                    //stepper();
                    t_stepper_motor.enable();
               
                    break;
                case RAISING_LEFT:
                    //stepper(); clockwise
                    t_stepper_motor.enable();
                    // Reaches step count or load cells activated???
                    break;
                case LOWERING_RIGHT:
  //t_stepper_motor.enable();
                    if (limit_switch_right == 1) {
                        pickup_state = RAISING_RIGHT;
                    }
                    break;
                case RAISING_RIGHT:
                  //stepper(); clockwise
                  // Reaches step count or load cells activated???
                  break;
            }
            break;            
            //play_tune(); 
            //wdt_reset(); // Resets watchdog timer
    }
}
    

void loop()
{ 
    taskManager.execute(); // Execute the scheduler
}
