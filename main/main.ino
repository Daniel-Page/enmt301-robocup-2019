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


//**********************************************************************************
// Definitions
//**********************************************************************************
#define BAUD_RATE          9600 // Bits/s
#define rightSensor        A2
#define leftSensor         A3
#define electromagnet      A1
#define limit_switch_left  27
#define limit_switch_right 28
#define LOAD_CELL_PIN_1    44
#define LOAD_CELL_PIN_2    45

#define US_READ_TASK_PERIOD                400
#define US_READ_TASK_NUM_EXECUTE           10


//**********************************************************************************
// Variables
//**********************************************************************************
int rightValue = 0;
int leftValue = 0;
int blocked = 0;
enum modes {SEARCHING, PICKUP, FINISHED};
enum modes program_state = SEARCHING;
Hx711 scale(LOAD_CELL_PIN_1,LOAD_CELL_PIN_2); // Setup pins for digital communications with weight IC
Task tInit_led(US_READ_TASK_PERIOD, US_READ_TASK_NUM_EXECUTE, &initLed);
Scheduler taskManager;


//**********************************************************************************
// Task scheduler initialisation
//**********************************************************************************
void task_init() {  
    // This is a class/library function. Initialise the task scheduler
    taskManager.init();     
   
    // Add tasks to the scheduler
    taskManager.addTask(tInit_led); // Reading ultrasonic 
  
    //enable the tasks
    tInit_led.enable();

    Serial.println("Tasks have been initialised \n");
}


void setup()
{ 
    setup_tune();
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    //wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial

    // Setup Electromagnet
    pinMode(electromagnet, OUTPUT);
    digitalWrite(electromagnet, HIGH);

    // Setup limit switches
    pinMode(limit_switch_left,INPUT);
    pinMode(limit_switch_right,INPUT);
    
    initLed();
    initMotors();
}


void loop() // Assumed to be running at approximately 16MHz
{ 
    if (program_state == SEARCHING) {
       
        static int sensor_timer_count = 0;
        if (sensor_timer_count >= 117) // 1ms*16MHz
        {
            rightValue = analogRead(rightSensor);
            //Serial.println(rightValue);
            leftValue = analogRead(leftSensor);
            //Serial.println(leftValue);
            sensor_timer_count = 0;
        } else {
          sensor_timer_count++;
        }
      
        // randomBit = rand() % 2;
       
        if (rightValue < 200 && leftValue < 200) { // When nothing blocks both sensors
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
        } else if (rightValue >= 200 && leftValue >= 200) { // When both sensors are blocked
                blocked = 1;
                Serial.println(blocked);

               // Runs when both sensors are block for 100 counts
        } else if (rightValue >= 200 && !blocked) { // When the right sensor is blocked
              turnRobot(ANTICLOCKWISE, 50);
        } else if (leftValue >= 200 && !blocked) { // When the left sensor is blocked
              turnRobot(CLOCKWISE, 50);
        } else if (blocked) { // While both sensors are blocked
            setMotor(RIGHT, ANTICLOCKWISE, 40);   
            setMotor(LEFT, ANTICLOCKWISE, 50);   
        }

        if (rightValue >= leftValue) {
        flash_led(rightValue); // Poll to flash at a rate based on the sensor input
        } else {
            flash_led(leftValue);
        }

        int left_switch = digitalRead(limit_switch_left);
        int right_switch = digitalRead(limit_switch_right);
        if (left_switch == 1 || right_switch == 1) {
            setMotor(RIGHT, STATIONARY, 0);  
            setMotor(LEFT, STATIONARY, 0);
            program_state = PICKUP;
        }
       // Serial.print(limit_switch_left);
        //Serial.print(limit_switch_right);
        //Serial.print("\n");
        
    }

    if (program_state == PICKUP) {
        // Stepper motor sequences and electromagnet activation
        stepper();
        
        
        if (is_step_state() == 2) {
            // Load cell checked every 200ms
            static int load_cell_timer_count = 0;
            if (load_cell_timer_count >= 23400) // 200ms*16MHz // Effectively delay(200)
            {
                if (scale.getGram() < 200) {
                    green_led_on();
               }
               // Serial.print(scale.getGram(), 1);    // Get force and print answer
                //Serial.println(" g");
                load_cell_timer_count = 0;
            } else {
              load_cell_timer_count++;
            }
        }
        
        
    }
    
    play_tune();  
    //wdt_reset(); // Resets watchdog timer
    //taskManager.execute(); // Execute the scheduler
}
