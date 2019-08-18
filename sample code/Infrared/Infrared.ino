int analogInPin = A7;  
int sensorValue = 0;
int analogInPin1 = A7;  
int sensorValue1 = 0;


void setup() {
  Serial.begin(9600); // 9600 bps
}


void loop() {
  sensorValue = analogRead(analogInPin);
  sensorValue1 = analogRead(analogInPin1);
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.println(sensorValue1);
  delay(2); // 2 ms ADC settling time


   

  
}
