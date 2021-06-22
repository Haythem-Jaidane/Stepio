#include "lib.h"

int m;
Stepper S;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  S.init(4,3,2);
  delay(6000);
  S.setSpeed(0,0.001);
  S.SetHardWard(400,0,1,2,16,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  S.write(3,10);
  m=S.getposition(1);
  //Serial.println(m);
}
