/*
Useing port S1 and channels S1 and S2 to drive the two motors on the robot.
The motors must be connected through the Sabertooth driver.
The sabertooth must be in mode Microcontroller pulses, independent linear control mode.
1000us to 2000us control the speed and direction. 1500us is stop.
Any speed outside this range will be ignored.
*/
#include <Servo.h>

Servo myservoA;      // create servo object to control a servo
Servo myservoB;      // create servo object to control a servo

int analogInPin = A0;  
int sensorValue = 0;


void setup()
{ 
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board

  Serial.begin(9600); // 9600 bit/s
  
  myservoA.attach(3);  // attaches the servo pin 3 to the servo object
  myservoB.attach(2);  // attaches the servo pin 2 to the servo object
}


// Sets the speed motor A based on a percentage that can be positive for clockwise
// or negative for anticlockwise.
// e.g. setMotorA(-50) sets the speed to 50% anticlockwise
void setMotorA(int percentage_power)
{
  if (percentage_power <= 0) {
  myservoA.writeMicroseconds(1500 - (percentage_power/100)*500); 
  } else {
  myservoA.writeMicroseconds(1000 + (1 - (percentage_power/100))*500); 
  }
}


// Sets the speed of motor B based on a percentage that can be positive for clockwise
// or negative for anticlockwise.
// e.g. setMotorB(-50) sets the speed to 50% anticlockwise
void setMotorB(int percentage_power)
{
  if (percentage_power <= 0) {
  myservoB.writeMicroseconds(1500 - (percentage_power/100)*500); 
  } else {
  myservoB.writeMicroseconds(1000 + (1 - (percentage_power/100))*500); 
  }
}


void loop() 
{ 
  setMotorA(60); // Sets motor to 100% speed clockwise
  setMotorB(-60); // Sets motor to 50% speed anticlockwise
  delay(1500); // 1.5s ADC settling time
  
  //sensorValue = analogRead(analogInPin);
  //Serial.println(sensorValue);
  
} 
