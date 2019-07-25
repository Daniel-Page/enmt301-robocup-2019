// main.ino - A program that controls the behaviour of a robot to compete in the robocup competition.
// Developed by Sasha Cox, Dervla Braem & Daniel Page

// Note the board has to be set to "Arduino Mega ADK" in Tools->Board in the Arduino program.
// Clock speed: 16MHz


#include <avr/wdt.h>
#include "motors.h"
#include "speaker.h"
#include "sensors.h"


#define BAUD_RATE 9600 // Bits/s


// Definitions for IR sensor
int rightSensor = A3; 
int rightValue = 0;

int leftSensor = A2; 
int leftValue = 0;

int blocked = 0;
int blink_rate = 0;

int electromagnet = A1;

int led = 20;

enum modes {SEARCHING, KNOCKOVER, PICKUP, BACKOFF, FINISHED}; // Implement states later


void setup()
{ 
    //pinMode(49, OUTPUT); // Pin 49 is used to enable IO power
    //digitalWrite(49, 1); // Enable IO power on main CPU board

    // Setup RGB led
    pinMode(led, OUTPUT);
    digitalWrite(led,LOW);

    // Setup Electromagnet
    pinMode(electromagnet, OUTPUT);
    digitalWrite(electromagnet, HIGH);

    //wdt_enable(WDTO_2S); // Watchdog timer set to trigger after 2 seconds if not reset

    Serial.begin(BAUD_RATE); // Initialises serial
    
    initMotors();
    midi(); // Start playing song
}


void loop() // Assumed to be running at approximately 16MHz
{ 
    rightValue = analogRead(rightSensor);
    Serial.println(rightValue);
    leftValue = analogRead(leftSensor);
    Serial.println(leftValue);
    //delay(1); // delay in ms between reads for stability
  
 
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
  
  
    static float n;
    // Max IR output assumed to be 1000 
    if (n >= 1600000) // 0.1s (0.1s*16MHz = 1600000)
    {
        digitalWrite(led,HIGH);
    } else if (n >= 3200000) { // 0.2s (0.2s*16MHz = 3200000)
        digitalWrite(led,LOW);
        n = 0;
    } else {
        n = n + rightValue/1000;
    }


    stepper_revolutions(2,CLOCKWISE); // Poll to step -- stops once revolutions completed

  
    //wdt_reset(); // Resets watchdog timer
}
