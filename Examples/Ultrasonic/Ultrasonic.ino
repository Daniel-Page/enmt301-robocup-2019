// For use with HC-SR04 US sensor

const unsigned int echo_pin = 28;
const unsigned int trig_pin = 26;
const unsigned int repetition_period_ms = 60;
const unsigned int echo_wait_us = 10000;
const unsigned int long serial_baud_rate = 115200;

void setup()
{
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    Serial.begin(serial_baud_rate);
}

void loop()
{
    static unsigned long time_ms_previous = 0;
    unsigned long time_ms;
    unsigned long duration_us;
    
    time_ms = millis();
    if ((time_ms - time_ms_previous) < repetition_period_ms)
        return;
    time_ms_previous = time_ms;
    
    // Trigger ping
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    
    // Wait for detected echo or timeout if too long
    duration_us = pulseIn(echo_pin, HIGH, echo_wait_us);
    
    Serial.print(duration_us);
    Serial.print("\n");
}
