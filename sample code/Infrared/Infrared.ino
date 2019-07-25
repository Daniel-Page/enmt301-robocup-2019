int analogInPin = A3;  
int sensorValue = 0;


void setup() {
  Serial.begin(9600); // 9600 bps
}


void loop() {
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  delay(2); // 2 ms ADC settling time
}
