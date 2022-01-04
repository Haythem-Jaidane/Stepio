#include "Stepio.h"

#define POT 5

Stepper S(4,3,2);
int pred=0,val=0;

void setup() {
  S.setup_step();
  S.setSpeed(0,10);
  S.SetHardWare(400,10);
}

void loop() {
  val=analogRead(POT)-pred;
  pred=analogRead(POT);
}

ISR(TIMER1_COMPA_vect){
  S.interrupt();
}
