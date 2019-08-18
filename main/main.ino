/***********************************************************************************
main.ino

A program that controls the behaviour of a robot to compete in the
robocup competition.

Developed by Sasha Cox, Dervla Braem & Daniel Page

Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the
Arduino program.

Clock speed: 16MHz
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
#include "src/Hx711/Hx711.h" // Include needed library of functions to talk to hx711 IC
#include "circular_buffer.h"


//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE          9600 // Bits/s
#define rightSensor        A2
#define leftSensor         A3
#define rightSensorBottom  A4
#define leftSensorBottom   A5
#define electromagnet      A1
#define limit_switch_left  27
#define limit_switch_right 28
#define LOAD_CELL_PIN_1    44
#define LOAD_CELL_PIN_2    45

#define US_READ_IR_TASK_PERIOD                10 // in ms
#define US_LED_TASK_PERIOD                    2000

#define US_READ_IR_TASK_NUM_EXECUTE           -1
#define US_LED_TASK_NUM_EXECUTE               -1


//**********************************************************************************
// Variables
//**********************************************************************************
int rightValue = 0;
int leftValue = 0;
int rightValueBottom = 0;
int leftValueBottom = 0;
int blocked = 0;
enum modes {SEARCHING, PICKUP, FINISHED};
enum modes program_state = SEARCHING;
Hx711 scale(LOAD_CELL_PIN_1,LOAD_CELL_PIN_2); // Setup pins for digital communications with weight IC
circBuffer sensor1;
circBuffer sensor2;
circBuffer sensor3;
circBuffer sensor4;


void obstructionCheck(void) {
    if (leftValueBottom <= 300 && leftValue <=50) {
        Serial.println(0);
    } else if (leftValueBottom > 300 && leftValue <= 50) {
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
    pinMode(electromagnet, OUTPUT);
    digitalWrite(electromagnet, HIGH);

    // Setup limit switches
    pinMode(limit_switch_left,INPUT);
    pinMode(limit_switch_right,INPUT);

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
    leftValue = analogRead(leftSensor);
    rightValue = analogRead(rightSensor);
    leftValueBottom = analogRead(leftSensorBottom);
    rightValueBottom = analogRead(rightSensorBottom);
    
    //Serial.println(leftValue);
    Serial.print(updateCircBuff(&sensor1, leftValue));
    Serial.print(" ");
    Serial.println(leftValue);

    delay(1);
    
    //Serial.println(rightValue);
    
    }
}
