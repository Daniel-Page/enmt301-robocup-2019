// Credit:
// Midi to Arduino Converter
//     - Andy Tran (extramaster), 2015
// https://www.extramaster.net/tools/midiToArduino/
//
// Process:
// Midi -> Midi tracks -> Note mappings -> Frequency
//
// CC0

// Set this to be the pin that your buzzer resides in. (Note that you can only have one buzzer actively using the PWM signal at a time).
#include <arduino.h>
int tonePin = A0;

// Ice Cream Man -- Greensleeves

void midi(void) {
    tone(tonePin, 783, 321.42825);
    delay(357.1425);
    tone(tonePin, 932, 642.8565);
    delay(714.285);
    tone(tonePin, 1046, 321.42825);
    delay(357.1425);
    tone(tonePin, 1174, 482.142375);
    delay(535.71375);
    tone(tonePin, 1244, 160.714125);
    delay(178.57125);
    tone(tonePin, 1174, 321.42825);
    delay(357.1425);
    tone(tonePin, 1046, 642.8565);
    delay(714.285);
    tone(tonePin, 880, 321.42825);
    delay(357.1425);
    tone(tonePin, 698, 482.142375);
    delay(535.71375);
    tone(tonePin, 783, 160.714125);
    delay(178.57125);
    tone(tonePin, 880, 321.42825);
    delay(357.1425);
    tone(tonePin, 932, 642.8565);
    delay(714.285);
    tone(tonePin, 783, 321.42825);
    delay(357.1425);
    tone(tonePin, 783, 482.142375);
    delay(535.71375);
    tone(tonePin, 698, 160.714125);
    delay(178.57125);
    tone(tonePin, 783, 321.42825);
    delay(357.1425);
    tone(tonePin, 880, 642.8565);
    delay(714.285);
    tone(tonePin, 698, 321.42825);
    delay(357.1425);
    tone(tonePin, 587, 642.8565);
    delay(714.285);
    tone(tonePin, 783, 321.42825);
    delay(357.1425);
    tone(tonePin, 932, 642.8565);
    delay(714.285);
    tone(tonePin, 1046, 321.42825);
    delay(357.1425);
    tone(tonePin, 1174, 482.142375);
    delay(535.71375);
    tone(tonePin, 1318, 160.714125);
    delay(178.57125);
    tone(tonePin, 1174, 321.42825);
    delay(357.1425);
    tone(tonePin, 1046, 642.8565);
    delay(714.285);
    tone(tonePin, 880, 321.42825);
    delay(357.1425);
    tone(tonePin, 698, 482.142375);
    delay(535.71375);
    tone(tonePin, 783, 160.714125);
    delay(178.57125);
    tone(tonePin, 880, 321.42825);
    delay(357.1425);
    tone(tonePin, 932, 482.142375);
    delay(535.71375);
    tone(tonePin, 880, 160.714125);
    delay(178.57125);
    tone(tonePin, 783, 321.42825);
    delay(357.1425);
    tone(tonePin, 739, 482.142375);
    delay(535.71375);
    tone(tonePin, 659, 160.714125);
    delay(178.57125);
    tone(tonePin, 739, 321.42825);
    delay(357.1425);
    tone(tonePin, 783, 642.8565);
    delay(714.285);
}
