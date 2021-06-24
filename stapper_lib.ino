#include "lib.h"

//int m;
Stepper S(4,3,2);

void setup() {
  // put your setup code here, to run once:
  S.setup_step();
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  S.write(0,400);
  Serial.print("step : ");
  Serial.println(S.getposition(0));
  Serial.print("angle : ");
  Serial.println(S.getposition(1));
  Serial.print("rotation : ");
  Serial.println(S.getposition(2));
  Serial.print("mm : ");
  Serial.println(S.getposition(3));
  S.write(0,-400);
}

ISR(TIMER1_COMPA_vect){
  S.interrupt();
}
