// Credit:
// Midi to Arduino Converter
//     - Andy Tran (extramaster), 2015
// https://www.extramaster.net/tools/midiToArduino/
//
// Process:
// Midi -> Midi tracks -> Note mappings -> Frequency
//
// CC0
    
#include "src/protothreads/pt.h"   // include protothread library
#include "src/protothreads/timer.h"
#include <Arduino.h>
// Set this to be the pin that your buzzer resides in. (Note that you can only have one buzzer actively using the PWM signal at a time).
int tonePin = 38;
boolean enablePtreplaceMe594744 = true;

static struct pt ptreplaceMe594744;

void initTune(void) {
  PT_INIT(&ptreplaceMe594744);  // protothread variable
}

static PT_THREAD(protothreadreplaceMe594744(struct pt *pt)) {
  static struct timer treplaceMe594744;
  PT_BEGIN(pt);
  while (enablePtreplaceMe594744) {
    tone(tonePin, 783, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 932, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1046, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1174, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1244, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1174, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1046, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 698, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 932, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 698, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 698, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 587, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 932, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1046, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1174, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1318, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1174, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 1046, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 698, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 932, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 880, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 739, 482.142375);
    timer_set(&treplaceMe594744, 535.71375); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 659, 160.714125);
    timer_set(&treplaceMe594744, 178.57125); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 739, 321.42825);
    timer_set(&treplaceMe594744, 357.1425); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));
    tone(tonePin, 783, 642.8565);
    timer_set(&treplaceMe594744, 714.285); PT_WAIT_UNTIL(pt, timer_expired(&treplaceMe594744));

    }
  PT_END(pt);
}

void play_tune(void) {
  protothreadreplaceMe594744(&ptreplaceMe594744); // schedule the protothread
}
