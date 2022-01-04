#include "Stepio.h"

Stepper S(4,3,2);

void setup() {
  S.setup_step();
  S.setSpeed(0,10);
  S.SetHardWare(400,10);
}

void loop() {
  S.write(0, 400 );
  delay(100);
  S.write(0, -400 );
}

ISR(TIMER1_COMPA_vect){
  S.interrupt();
}
