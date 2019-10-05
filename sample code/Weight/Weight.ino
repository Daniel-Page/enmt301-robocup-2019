/*
   Description:
   The sample code for digital weight scale of hx711. The weight will be printed in the serial.
   Connection:   
   Hx711.DOUT - pin 24
   Hx711.SCK  - pin 25
*/
#include "Hx711.h"                      //Include needed library of functions to talk to hx711 IC
Hx711 scale1(16, 17);                    //Setup pins for digital communications with weight IC
Hx711 scale2(26, 27);
void setup() 
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  Serial.begin(9600);                  //Setup serial port
}
 
void loop() 
{
 Serial.print(scale1.getGram(), 1);    //Get force and print answer
 Serial.print(" ");
 
 delay(200); 
 Serial.print(scale2.getGram(), 1);    //Get force and print answer
 Serial.print("\n");
 delay(200);
                         //Wait a while
}
