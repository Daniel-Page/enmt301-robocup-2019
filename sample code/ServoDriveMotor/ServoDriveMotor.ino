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

void setup()
{ 
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  myservoA.attach(13);  // attaches the servo pin 3 to the servo object
  myservoB.attach(12);  // attaches the servo pin 2 to the servo object
}

void loop() 
{ 
  myservoA.writeMicroseconds(1500);      //Stop 
  myservoB.writeMicroseconds(1500);      // 
  delay(1500);                           //
  myservoA.writeMicroseconds(1800);      //Forward Slow  
  myservoB.writeMicroseconds(1800);      // 
  delay(1500);                           // 
  myservoA.writeMicroseconds(2000);      //Forward Fast   
  myservoB.writeMicroseconds(2000);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1800);      //Forward Slow  
  myservoB.writeMicroseconds(1800);      // 
  delay(1500);                           // 
  myservoA.writeMicroseconds(1500);      //Stop 
  myservoB.writeMicroseconds(1500);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1200);      //Backward Slow  
  myservoB.writeMicroseconds(1200);      // 
  delay(1500);                           // 
  myservoA.writeMicroseconds(1000);      //Backward Fast 
  myservoB.writeMicroseconds(1000);      // 
  delay(1500);                           // 
  myservoA.writeMicroseconds(1200);      //Backward Slow   
  myservoB.writeMicroseconds(1200);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1500);      //Stop  
  myservoB.writeMicroseconds(1500);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1200);      //Spin Slow clockwise 
  myservoB.writeMicroseconds(1800);      //  
  delay(1500);                           //  
  myservoA.writeMicroseconds(1000);      //Spin Fast clockwise  
  myservoB.writeMicroseconds(2000);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1200);      //Spin Slow clockwise  
  myservoB.writeMicroseconds(1800);      //  
  delay(1500);                           //
  myservoA.writeMicroseconds(1500);      //Stop  
  myservoB.writeMicroseconds(1500);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1800);      //Spin Slow anticlockwise 
  myservoB.writeMicroseconds(1200);      //  
  delay(1500);                           //  
  myservoA.writeMicroseconds(2000);      //Spin Fast anticlockwise  
  myservoB.writeMicroseconds(1000);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1800);      //Spin Slow anticlockwise  
  myservoB.writeMicroseconds(1200);      //  
  delay(1500);                           // 
  myservoA.writeMicroseconds(1500);      //Stop  
  myservoB.writeMicroseconds(1500);      //  
  delay(1500);                           // 
} 
