#include <TaskScheduler.h>


int blue_led = 20; // Blue





#define US_READ_TASK_PERIOD                 400

#define US_READ_TASK_NUM_EXECUTE           10


void flash_led(void) {
      static int led_state = LOW;
      if (led_state == LOW) {
            led_state = HIGH;
        } else if (led_state == HIGH) {
            led_state = LOW;
        }
        digitalWrite(blue_led,led_state); 
}


Task tflash_led(US_READ_TASK_PERIOD, US_READ_TASK_NUM_EXECUTE, &flash_led);

Scheduler taskManager;





void setup() {
 // Setup blue led
  pinMode(blue_led, OUTPUT);
  digitalWrite(blue_led,LOW);
  
  task_init();

}


void task_init() {  
  
  // This is a class/library function. Initialise the task scheduler
  taskManager.init();     
 
  // Add tasks to the scheduler
  taskManager.addTask(tflash_led);   //reading ultrasonic 

  //enable the tasks
  tflash_led.enable();


 Serial.println("Tasks have been initialised \n");
}



void loop() {
   taskManager.execute();    //execute the scheduler
}
