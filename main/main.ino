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
#include "src/TaskScheduler/TaskScheduler.h"
#include "motors.h"
#include "speaker.h"
#include "sensors.h"
#include "led.h"
#include "src/Hx711/Hx711.h" // Load cell library
#include "circular_buffer.h"


//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE                    9600 // Bits/s
#define IR_SENSOR_LEFT_TOP_PIN       A6
#define IR_SENSOR_RIGHT_TOP_PIN      A7
#define IR_SENSOR_LEFT_BOTTOM_PIN    A4
#define IR_SENSOR_RIGHT_BOTTOM_PIN   A5
#define ELECTROMAGNET_LEFT           A0
#define ELECTROMAGNET_RIGHT          A2
#define LIMIT_SWITCH_LEFT            27 // Not in use
#define LIMIT_SWITCH_RIGHT           28
#define LOAD_CELL_LEFT_1_PIN         44
#define LOAD_CELL_LEFT_1_PIN         45
#define LOAD_CELL_RIGHT_2_PIN        46
#define LOAD_CELL_RIGHT_2_PIN        47
#define SERVO_FRONT_PIN              34

#define US_READ_IR_TASK_PERIOD       10 // in ms
#define US_LED_TASK_PERIOD           2000

#define US_READ_IR_TASK_NUM_EXECUTE  -1
#define US_LED_TASK_NUM_EXECUTE      -1


//**********************************************************************************
// Variables
//**********************************************************************************

int IR_sensor_left_top = 0;
int IR_sensor_right_top = 0;
int IR_sensor_left_bottom = 0;
int IR_sensor_right_bottom = 0;
int blocked = 0;
enum modes {SEARCHING, PICKUP, FINISHED};
enum modes program_state = SEARCHING;
Hx711 scale(LOAD_CELL_LEFT_1_PIN,LOAD_CELL_LEFT_1_PIN);   // Setup pins for digital communications with weight IC
// Hx711 scale(LOAD_CELL_RIGHT_2_PIN,LOAD_CELL_RIGHT_2_PIN); // Setup pins for digital communications with weight IC

circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;


void obstructionCheck(void) {
    if (IR_sensor_left_bottom <= 300 && IR_sensor_left_top <= 50) {
        Serial.println(0);
    } else if (IR_sensor_left_bottom > 300 && IR_sensor_left_top <= 50) {
        Serial.println(1);
    }
}


void readIR(void) {
    

}


Task tReadIR(US_READ_IR_TASK_PERIOD, US_READ_IR_TASK_NUM_EXECUTE, &readIR);
Task tFlashLed(US_LED_TASK_PERIOD, US_LED_TASK_NUM_EXECUTE, &flash_led);

Scheduler taskManager;


void taskInit() {  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(tReadIR);   //reading ultrasonic 
  taskManager.addTask(tFlashLed);

  //enable the tasks
  tReadIR.enable();
  tFlashLed.enable();


 //Serial.println("Tasks have been initialised \n");
}


void setup()
{ 
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    // wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial

    // Setup Electromagnet
    pinMode(ELECTROMAGNET_LEFT, OUTPUT);
    pinMode(ELECTROMAGNET_RIGHT, OUTPUT);
    digitalWrite(ELECTROMAGNET_LEFT, HIGH);
    digitalWrite(ELECTROMAGNET_RIGHT, HIGH);

    // Setup limit switches
    pinMode(LIMIT_SWITCH_LEFT, INPUT);
    pinMode(LIMIT_SWITCH_RIGHT, INPUT);


    pinMode(SERVO_FRONT_PIN, OUTPUT);



    initCircBuff(&sensor1);
    initCircBuff(&sensor2);
    initCircBuff(&sensor2);
    initCircBuff(&sensor2);

    initTune();
    initLed();
    initMotors();
    taskInit();
}


void loop()
{ 
    if (program_state == SEARCHING) {

    //taskManager.execute(); // Execute the scheduler
    //Serial.print(rightValue);
    // Serial.print(" ");
    //Serial.println(leftValue);
    //Serial.print(" ");
    IR_sensor_left_top = analogRead(IR_SENSOR_LEFT_TOP_PIN);
    IR_sensor_right_top = analogRead(IR_SENSOR_RIGHT_TOP_PIN);
    IR_sensor_left_bottom = analogRead(IR_SENSOR_LEFT_BOTTOM_PIN);
    IR_sensor_right_bottom = analogRead(IR_SENSOR_RIGHT_BOTTOM_PIN);
    
    //Serial.println(leftValue);
    Serial.print(updateCircBuff(&sensor1, IR_SENSOR_LEFT_TOP_PIN));
    Serial.print(" ");
    Serial.println(IR_SENSOR_LEFT_TOP_PIN);

    delay(1);
    
    //Serial.println(rightValue);
    
    }
}
