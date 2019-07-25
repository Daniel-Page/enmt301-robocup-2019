/*
This sample code is for testing the 4 stepper motors
Each motor will iturn rotate forward then backward
*/

int M2dirpin   = 32;
int M2steppin  = 33;


void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  

 
  pinMode(M2dirpin,OUTPUT);
  pinMode(M2steppin,OUTPUT);
  

}

void loop()
{
  int j;

  //Set direction for all channels   
  digitalWrite(M2dirpin,LOW);

  
  //Channel 2
  for(j=0;j<=100;j++)
  {
    digitalWrite(M2steppin,LOW);
    delayMicroseconds(2);
    digitalWrite(M2steppin,HIGH);
    delay(1);
  }

  

}
