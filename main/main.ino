// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.
// Clock speed: 16MHz

// To fix:
// load cell threshold -- can detect when a weight is attached but takes a few seconds
// reliability of stepper motor movement?


#include <avr/wdt.h>
#include "TaskScheduler.h"
#include "motors.h"
#include "speaker.h"
#include "sensors.h"
#include "led.h"
#include "Hx711.h"  //Include needed library of functions to talk to hx711 IC
Hx711 scale(44,45);  //Setup pins for digital communications with weight IC


#define BAUD_RATE 9600 // Bits/s


// Definitions for IR sensor
int rightSensor = A2; 
int rightValue = 0;

int leftSensor = A3; 
int leftValue = 0;

int blocked = 0;
int blink_rate = 0;

int electromagnet = A1;

int limit_switch_left = 27;
int limit_switch_right = 28;

enum modes {SEARCHING, PICKUP, FINISHED};
enum modes program_state = SEARCHING;


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
}
