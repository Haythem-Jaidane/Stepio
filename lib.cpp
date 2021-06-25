#include "lib.h"
#include "Arduino.h"

void Stepper::move(int S) {
  /*
   * this function turn the stepper motor S step if S>0
   * the motor trun in clockwise else the motor turn in
   * counter-clockwise
   */
  bool wait = true;
  
  digitalWrite(this->diraction_pin, S < 0 ? HIGH : LOW);
  this->dir = S > 0 ? 1 : -1;
  this->totalSteps = abs(S);
  this->d = this->c0;
  OCR1A = this->d;
  this->stepCount = 0;
  this->n = 0;
  this->rampUpStepCount = 0;
  this->movementDone = false;
  TIMSK1 |=  (1 << OCIE1A);
  
  while ( wait && ! this->movementDone );
}

void Stepper::SetHardWare(int rotation, int mm_rev) {
  /*
   * this function define drive parametre number of step in rotation
   * mm in one rotation
   */
  this->rotation = rotation;
  this->mm_rev=mm_rev;
}

Stepper::Stepper(int enable_pin, int step_pin, int diraction_pin) {
  /*
   * the constructor function of the object that define the pins of
   * the stepper
   */
  this->enable_pin = enable_pin;
  this->step_pin = step_pin;
  this->diraction_pin = diraction_pin;
  this->c0 = 1600;
}

void Stepper::setup_step(){
  /*
   * this fuction is called to setup motor in void setup
   */
  pinMode(this->step_pin, OUTPUT);
  pinMode(this->diraction_pin, OUTPUT);
  pinMode(this->enable_pin, OUTPUT);
  digitalWrite(this->enable_pin, 0);
  
  noInterrupts();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  OCR1A = 1000;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10)); 
  interrupts();
}

void Stepper::write(char mode, int parametre) {
  /*
   * this function is used to move stepper with muliple mode
   * 0: step
   * 1: °
   * 2: rotation
   * 3: mm
   */
  switch (mode) {

    case 0:

      this->move(parametre);
      this->step += parametre;
      break;

    case 1:

      this->move(this->rotation / (360 / parametre));
      this->step += this->rotation / (360 / (parametre));
      break;

    case 2:

      this->move(parametre*this->rotation);
      this->step += (parametre) * this->rotation;
      break;

    case 3:
      this->move((this->mm_rev*this->rotation)/parametre);
      this->step += ((this->mm_rev*this->rotation)/(parametre));
      break;

  }

  this->stepPosition = this->step % this->rotation ;
  this->n_rotation = this->step / this->rotation;
  this->angle = 360/((double)this->rotation/this->stepPosition) ;
  this->mm = ((double)this->step/this->rotation)*this->mm_rev;
}

void Stepper::writeAbs(char mode, int parametre) {
  /*
   * 0: step
   * 1: °
   * 2: rotation
   * 3: mm 
   */
  if (this->step / this->rotation < 1) {
    this->write(mode, parametre);
  }

}

int Stepper::getposition(char mode) {
  /*
   * this function return the initial position of the stepper
   */
  switch (mode) {

    case 0:
      return this->stepPosition;
      break;

    case 1:
      return this->angle;
      break;

    case 2:
      return this->n_rotation;
      break;

    case 3:
      return this->mm;
      break;

  }
}

void Stepper::setposition(char mode, int parametre) {
  /*
   * 
   */
  switch (mode) {

    case 0:
      this->pos_init = parametre;
      break;

    case 1:
      this->angle_init = parametre;
      break;

    case 3:
      this->mm_init = parametre;
      break;
  }
}

void Stepper::setSpeed(char mode, int parametre) {
  /*
   * this function is used to set the speed 
   * of the stepper motor
   * 
   * int parametre : new speed of motor
   * char mode : type of speed
   * 0: step/s
   * 1: °/s
   * 2: rotation/s
   * 3: mm/s
   */
  switch (mode) {

    case 0:
      this->maxSpeed = parametre;
      break;

    case 1:
      this->maxSpeed = parametre * (this->rotation / 360);
      break;

    case 2:
      this->maxSpeed = parametre * this->rotation;
      break;

    case 3:
      this->maxSpeed = parametre / this->mm_rev;
      break;

  }
}

int Stepper::getSpeed(char mode) {
  /*
   * this function return the speed of the motor
   * 
   * char mode: type of speed
   * 0: step/s
   * 1: °/s
   * 2: rotation/s
   * 3: mm/s
   */
  switch (mode) {

    case 0:
      return this->maxSpeed;
      break;

    case 1:
      return this->maxSpeed / (this->rotation / 360);
      break;

    case 2:
      return this->maxSpeed / this->rotation;
      break;

    case 3:
      return (this->maxSpeed / this->rotation) * this->mm_rev;
      break;
  }
}


void Stepper::interrupt(){
  /*
   *  this function is called in every timer interrution
   *  that make stepper motor turn smothly
   */  
  if ( this->stepCount < this->totalSteps ) {
    digitalWrite(this->step_pin, 1);
    digitalWrite(this->step_pin, 0);
    this->stepCount++;
  }
  else {
    this->movementDone = true;
    TIMSK1 &= ~(1 << OCIE1A);
  }

  if ( this->rampUpStepCount == 0 ) { // ramp up phase
    this->n++;
    this->d = this->d - (2 * this->d) / (4 * this->n + 1);
    if ( this->d <= this->maxSpeed ) { // reached max speed
      this->d = this->maxSpeed;
      this->rampUpStepCount = this->stepCount;
    }
    if ( this->stepCount >= this->totalSteps / 2 ) { // reached halfway point
      this->rampUpStepCount = this->stepCount;
    }
  }
  else if ( this->stepCount >= this->totalSteps - this->rampUpStepCount ) { // ramp down phase
    this->n--;
    this->d = (this->d * (4 * this->n + 1)) / (4 * this->n + 1 - 2);
  }

  OCR1A = this->d;
}
