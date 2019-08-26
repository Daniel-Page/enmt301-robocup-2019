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

int time1;


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
#define IR_SENSOR_RIGHT_TOP_PIN               A7
#define IR_SENSOR_LEFT_BOTTOM_PIN             A4
#define IR_SENSOR_RIGHT_BOTTOM_PIN            A5
#define ELECTROMAGNET_LEFT_PIN                A0
#define ELECTROMAGNET_RIGHT_PIN               A2
#define LOAD_CELL_LEFT_1_PIN                  44
#define LOAD_CELL_LEFT_2_PIN                  45
#define LOAD_CELL_RIGHT_1_PIN                 46
#define LOAD_CELL_RIGHT_2_PIN                 47
#define SERVO_FRONT_PIN                       34
#define INDUCTIVE_PROX_SENSOR_LEFT_HORIZ_PIN  38
#define INDUCTIVE_PROX_SENSOR_LEFT_VERT_PIN   39
#define INDUCTIVE_PROX_SENSOR_RIGHT_HORIZ_PIN 38
#define INDUCTIVE_PROX_SENSOR_RIGHT_VERT_PIN  39
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
int IR_sensor_left_bottom = 0;
int IR_sensor_right_bottom = 0;

int inductive_prox_sensor_left_horiz = 0;
int inductive_prox_sensor_left_vert = 0;
int inductive_prox_sensor_right_horiz = 0;
int inductive_prox_sensor_right_vert = 0;
int prox_sensor_left = 0;
int prox_sensor_right = 0;

int blocked = 0;
enum modes {SEARCHING, PICKUP, FINISHED};
enum modes program_state = SEARCHING;
Hx711 scale(LOAD_CELL_LEFT_1_PIN,LOAD_CELL_LEFT_1_PIN);   // Setup pins for digital communications with weight IC
// Hx711 scale(LOAD_CELL_RIGHT_2_PIN,LOAD_CELL_RIGHT_2_PIN); // Setup pins for digital communications with weight IC

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
  t_stepper_motor.enable();
  t_state_controller.enable();
  t_read_proximity_sensors.enable();

 //Serial.println("Tasks have been initialised \n");
}


void setup()
{ 
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    // wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial

    // Setup Electromagnet
    pinMode(ELECTROMAGNET_LEFT_PIN, OUTPUT);
    pinMode(ELECTROMAGNET_RIGHT_PIN, OUTPUT);
    digitalWrite(ELECTROMAGNET_LEFT_PIN, HIGH);
    digitalWrite(ELECTROMAGNET_RIGHT_PIN, HIGH);

    pinMode(SERVO_FRONT_PIN, OUTPUT);

    // Proximity sensors
    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_HORIZ_PIN, INPUT);
    pinMode(INDUCTIVE_PROX_SENSOR_LEFT_VERT_PIN, INPUT);  
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_HORIZ_PIN, INPUT);  
    pinMode(INDUCTIVE_PROX_SENSOR_RIGHT_VERT_PIN, INPUT);  
    pinMode(PROX_SENSOR_LEFT_PIN, INPUT);  
    pinMode(PROX_SENSOR_RIGHT_PIN, INPUT);  

    initCircBuff(&sensor1);
    initCircBuff(&sensor2);
    initCircBuff(&sensor2);
    initCircBuff(&sensor2);

    initTune();
    initLed();
    initMotors();
    taskInit();
}


void read_IR_sensors(void) {
    time1 = millis();
    Serial.print(time1);
    Serial.print("\n");
    IR_sensor_left_top = analogRead(IR_SENSOR_LEFT_TOP_PIN);
    IR_sensor_right_top = analogRead(IR_SENSOR_RIGHT_TOP_PIN);
    IR_sensor_left_bottom = analogRead(IR_SENSOR_LEFT_BOTTOM_PIN);
    IR_sensor_right_bottom = analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN);
}


void read_proximity_sensors(void) {
    inductive_prox_sensor_left_horiz = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_HORIZ_PIN);
    inductive_prox_sensor_left_vert = digitalRead(INDUCTIVE_PROX_SENSOR_LEFT_VERT_PIN);
    inductive_prox_sensor_right_horiz = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_HORIZ_PIN);
    inductive_prox_sensor_right_vert = digitalRead(INDUCTIVE_PROX_SENSOR_RIGHT_VERT_PIN);
    prox_sensor_left = digitalRead(PROX_SENSOR_LEFT_PIN);
    prox_sensor_right = digitalRead(PROX_SENSOR_RIGHT_PIN);
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
            
        case PICKUP:
            // Stepper motor sequences and electromagnet activation
            //stepper();
            play_tune(); 
            //wdt_reset(); // Resets watchdog timer
    }
}
     

void loop()
{ 
    taskManager.execute(); // Execute the scheduler
}
