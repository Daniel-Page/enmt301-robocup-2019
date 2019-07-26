// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.
// Clock speed: 16MHz


#include <avr/wdt.h>
#include "motors.h"
#include "speaker.h"
#include "sensors.h"
#include "led.h"


#define BAUD_RATE 9600 // Bits/s


// Definitions for IR sensor
int rightSensor = A3; 
int rightValue = 0;

int leftSensor = A2; 
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
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial

    // Setup Electromagnet
    pinMode(electromagnet, OUTPUT);
    digitalWrite(electromagnet, LOW);
    
    pinMode(limit_switch_left,INPUT);
    pinMode(limit_switch_right,INPUT);
    
    initLed();
    initMotors();
    //midi(); // Start playing song
}


void loop() // Assumed to be running at approximately 16MHz
{ 
    if (program_state == SEARCHING) {
       
        static int sensor_timer_count = 0;
        if (sensor_timer_count >= 16000) // 1ms*16MHz
        {
            rightValue = analogRead(rightSensor);
            Serial.println(rightValue);
            leftValue = analogRead(leftSensor);
            Serial.println(leftValue);
            sensor_timer_count = 0;
        } else {
          sensor_timer_count++;
        }
      
        // randomBit = rand() % 2;
       
        if (rightValue < 200 && leftValue < 200) { // When nothing blocks both sensors
            blocked = 0;   
            setMotor(RIGHT, CLOCKWISE, 75);  
            setMotor(LEFT, CLOCKWISE, 75);
        } else if (rightValue >= 200 && leftValue >= 200) { // When both sensors are blocked
                blocked = 1;
               // Runs when both sensors are block for 100 counts
        } else if (rightValue >= 200 && !blocked) { // When the right sensor is blocked
              turnRobot(ANTICLOCKWISE, 50);
        } else if (leftValue >= 200 && !blocked) { // When the left sensor is blocked
              turnRobot(CLOCKWISE, 50);
        } else if (blocked) { // While both sensors are blocked
              turnRobot(ANTICLOCKWISE, 50);
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
        Serial.print(limit_switch_left);
        Serial.print(limit_switch_right);
        Serial.print("\n");
        
    }

    if (program_state == PICKUP) {
        
        static int arm;
        
        if (arm = 0 && steps(10000,ANTICLOCKWISE) == 1) { // Lowers the arm to the level of the weight
            arm = 1;
            digitalWrite(electromagnet, HIGH);
        } 
        
        if (arm == 1) {
            steps(9000,CLOCKWISE); // Raises the arm to almost the top
        }
    }
    
  
    wdt_reset(); // Resets watchdog timer
}
