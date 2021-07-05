#include "lib.h"

//int m;
Stepper S(4,3,2);

void setup() {
  // put your setup code here, to run once:
  S.setup_step();
  S.setSpeed(0,10);
  S.SetHardWare(400,10);
}

void loop() {
  // put your main code here, to run repeatedly:
  S.setSpeed(0, 100);
  S.write(0,  1600 );
  S.write(0, -1600 );

  S.setSpeed(0, 200);
  S.write(0, 200 );
  S.write(0, 400 );

  S.setSpeed(0, 400);
  S.write(0, 600 );
  S.write(0, 800 );

  S.setSpeed(0, 600);
  S.write(0, 1000 );
  S.write(0, 1200 );
  
}

ISR(TIMER1_COMPA_vect){
  S.interrupt();
}
