#include <Stepio.h>

Stepper S(4,3,2);

void setup() {
  S.setup_step();
  S.setSpeed(0,10);
  S.SetHardWare(400,10);
}

void loop() {

  S.write( 0,1600 );
  S.write( 0,-1600 );

  S.write( 0,200 );
  S.write( 0,400 );
  S.write( 0,600 );
  S.write( 0,800 ); 

  S.write( 0,400 );
  S.write( 0,600 );
  S.write( 0,200 );
  S.write( 0,400 );

  S.setSpeed(0, 600);
  S.write( 0,200 );
  S.write( 0,400 );

  S.setSpeed(0, 400);
  S.write( 0,600 );
  S.write( 0,800 );

  S.setSpeed(0, 200);
  S.write( 0,1000 );
  S.write( 0,1200 );

  S.setSpeed(0, 10);
  S.write( 0,-200 );

}

ISR(TIMER1_COMPA_vect){
  S.interrupt();
}
