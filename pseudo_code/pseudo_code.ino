void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

// Pseudo code
if (inductive_prox_sensor_left_horiz == 1) {
    //##Change to pickup state##
    //##Stop robot##
    //##Start lowering the arm with the stepper motor (anticlockwise)##
    //##Limit switch is activated##
    //##Wait a short period for the electromagnet to make contact(anticlockwise)##
    //##Start raising the arm with the stepper motor (clockwise)##
      //##Optional load cell check##
      //##Actuate moving guide##
    //##Change to searching state##
}

if (inductive_prox_sensor_right_horiz == 1) {
    //##Change to pickup state##
    //##Stop robot##
    //##Start lowering the arm with the stepper motor (anticlockwise)##
    //##Limit switch is activated##
    //##Wait a short period for the electromagnet to make contact(anticlockwise)##
    //##Start raising the arm with the stepper motor (clockwise)##
      //##Optional load cell check##
      //##Actuate moving guide##
    //##Change to searching state## 
}



}
