/*
This sample code is for testing the 4 stepper motors
All motors will rotate the same direction at the same time
*/
int M1dirpin = 39;
int M1steppin = 38;
int M2dirpin = 41;
int M2steppin = 40;

int M3dirpin = 45;
int M3steppin = 44;
int M4dirpin = 43;
int M4steppin = 42;

void setup()
{
  pinMode(49, OUTPUT);                 //Pin 49 is used to enable IO power
  digitalWrite(49, 1);                 //Enable IO power on main CPU board
  
  //Setup step and direction pins for output
  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  
  pinMode(M2dirpin,OUTPUT);
  pinMode(M2steppin,OUTPUT);
  
  pinMode(M3dirpin,OUTPUT);
  pinMode(M3steppin,OUTPUT);
  
  pinMode(M4dirpin,OUTPUT);
  pinMode(M4steppin,OUTPUT);
}

void loop()
{
  int j;
  
  //Set direction for all channels
  digitalWrite(M1dirpin,LOW);
  digitalWrite(M2dirpin,LOW);
  digitalWrite(M3dirpin,LOW);
  digitalWrite(M4dirpin,LOW);
  
  for(j=0;j<=1000;j++)            //Move 1000 steps
  {
    digitalWrite(M1steppin,LOW);
    digitalWrite(M2steppin,LOW);
    digitalWrite(M3steppin,LOW);
    digitalWrite(M4steppin,LOW);
    delayMicroseconds(2);
    digitalWrite(M1steppin,HIGH);
    digitalWrite(M2steppin,HIGH);
    digitalWrite(M3steppin,HIGH);
    digitalWrite(M4steppin,HIGH);
    delay(1);
  }
}
