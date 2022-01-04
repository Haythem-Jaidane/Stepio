#include "Stepio.h"
#include "Arduino.h"

/*
 * this function turn the stepper motor S step if S>0
 * the motor trun in clockwise else the motor turn in
 * counter-clockwise
 */

void Stepper::move(int S) {
  digitalWrite(this->diraction_pin, S < 0 ? HIGH : LOW);  //  if S<0 give diraction_pin 5V else 0V
  this->dir = S > 0 ? 1 : -1;                             //  if S<0 dir=-1 else dir=1
  this->totalSteps = abs(S);                              //  set totalSteps to absulte value of S
  this->d = this->c0;                                     //  set delay variable to first delay value
  OCR1A = this->d;                                        //  set timer1 Compare A to d value
  this->stepCount = 0;                                    //  set step counter to 0
  this->n = 0;                                            //  set n value to 0
  this->rampUpStepCount = 0;                              //  set ramp up step counter to 0
  this->movementDone = false;                             //  set movementDone flag to false
  TIMSK1 |=  (1 << OCIE1A);                               //  enbale Compare A interupt
  while (! this->movementDone );                          //  wait until the movement deserved is done
}

/*
 * this function define drive parametre number of step in rotation
 * mm in one rotation
 */

void Stepper::SetHardWare(int rotation, int mm_rev) {
  this->rotation = rotation;  // config number step by rotation
  this->mm_rev=mm_rev;        // config distance spend in mm in one motor evolution
}

/*
 * the constructor method of the object that define the pins of
 * the stepper
 */

Stepper::Stepper(int enable_pin, int step_pin, int diraction_pin) {
  this->enable_pin = enable_pin;        // config the enable pin
  this->step_pin = step_pin;            // config step pin
  this->diraction_pin = diraction_pin;  // config diraction pin
  this->c0 = 1600;                      // set value of to 1600
}

/*
 * this fuction is called to setup motor in void setup
 */

void Stepper::setup_step(){
  pinMode(this->step_pin, OUTPUT);        // set step pin to OUTPUT mode
  pinMode(this->diraction_pin, OUTPUT);   // set diraction pin to OUTPUT mode
  pinMode(this->enable_pin, OUTPUT);      // set enable pin to OUTPUT mode
  digitalWrite(this->enable_pin, 0);      // give enable 0V

  cli();                                  // disable interuption
  TCCR1A=0;                               // initial TCCR1A register to 0x00
  TCCR1B=0;                               // initial TCCR1B register to 0x00
  TCNT1=0;                                // set TIMER1 counter to 0
  OCR1A = 1000;                           // set the comparator to 1000
  TCCR1B |= (1 << WGM12);                 // set the clock to comparative mode
  TCCR1B |= ((1 << CS11) | (1 << CS10));  // set prescaler clk/64
  sei();                                  // enable interuption
}

/*
 * this method is used to move stepper with muliple mode
 *
 * 0: step
 * 1: °
 * 2: rotation
 * 3: mm
 */

void Stepper::write(char mode, int parametre) {
  switch (mode) {

    case 0:

      this->move(parametre);                                       // call move method
      this->step += parametre;                                     // increase number of steps
      break;

    case 1:

      this->move(this->rotation / (360 / parametre));              // call move method
      this->step += this->rotation / (360 / (parametre));          // increase number of steps
      break;

    case 2:

      this->move(parametre*this->rotation);                        // call move method
      this->step += (parametre) * this->rotation;                  // increase number of steps
      break;

    case 3:
      this->move((this->mm_rev*this->rotation)/parametre);         // call move method
      this->step += ((this->mm_rev*this->rotation)/(parametre));   // increase number of steps
      break;

  }

  this->stepPosition = this->step % this->rotation ;               // update the position of motor in step
  this->n_rotation = this->step / this->rotation;                  // update the number of rotation done by the motor
  this->angle = 360/((double)this->rotation/this->stepPosition);   // update the position of motor in °
  this->mm = ((double)this->step/this->rotation)*this->mm_rev;     // update the distance that motor parcoured
}

/*
 * like write() but on one rotation
 */

void Stepper::writeAbs(char mode, int parametre) {
  if (this->step / this->rotation < 1) {        // check if one evolution is done
    this->write(mode, parametre);               // write the diserved move
  }
}

/*
 * this method return the initial position of the stepper
 */

int Stepper::getposition(char mode) {
  switch (mode) {

    case 0:
      return this->stepPosition;  // return the current position of motor in step
      break;

    case 1:
      return this->angle;         // return the current position of motor in °
      break;

    case 2:
      return this->n_rotation;    // return the number of evolution done by the motor
      break;

    case 3:
      return this->mm;            // return the distance spend by the motor in mm
      break;

  }
}

/*
 * the method set the default position of the
 * stepper
 *
 * char mode : type of speed
 * 0: step/s
 * 1: °/s
 * 2: rotation/s
 * 3: mm/s
 * int parametre : new position of motor
 */

void Stepper::setposition(char mode, int parametre) {
  switch (mode) {

    case 0:
      this->pos_init = parametre;         // set position in step
      break;

    case 1:
      this->angle_init = parametre;       // set position in °
      break;

    case 3:
      this->mm_init = parametre;          // set position in mm
      break;
  }
}

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

void Stepper::setSpeed(char mode, int parametre) {
  switch (mode) {

    case 0:
      this->maxSpeed = parametre;                            // set speed in step/s
      break;

    case 1:
      this->maxSpeed = parametre * (this->rotation / 360);   // set speed in °/s
      break;

    case 2:
      this->maxSpeed = parametre * this->rotation;           // set speed in rotation/s
      break;

    case 3:
      this->maxSpeed = parametre / this->mm_rev;             // set speed in mm/s
      break;

  }
}

/*
 * this function return the speed of the motor
 *
 * char mode: type of speed
 * 0: step/s
 * 1: °/s
 * 2: rotation/s
 * 3: mm/s
 */

int Stepper::getSpeed(char mode) {

  switch (mode) {

    case 0:
      return this->maxSpeed;                                    //  return speed in step/s
      break;

    case 1:
      return this->maxSpeed / (this->rotation / 360);           //  return speed in °/s
      break;

    case 2:
      return this->maxSpeed / this->rotation;                   //  return speed in rotition/s
      break;

    case 3:
      return (this->maxSpeed / this->rotation) * this->mm_rev;  //  return speed in mm/s
      break;
  }
}

/*
 *  this function is called in every timer interrution
 *  that make stepper motor turn smothly
 */

void Stepper::interrupt(){
  if ( this->stepCount < this->totalSteps ) {
    // pulse the step pin to turn one steppe
    digitalWrite(this->step_pin, 1);    // set step pin to high
    digitalWrite(this->step_pin, 0);    // set step pin to low
    // increase the step counter
    this->stepCount++;                  // increase step counter
  }
  else {
    this->movementDone = true;          // set movementDone flag to True
    TIMSK1 &= ~(1 << OCIE1A);           // disable Compare A Match Interrupt
    this->d = this->maxSpeed;           // set delay value to maxspeed
  }
  // --------------------  ramp up phase  --------------------
  if ( this->rampUpStepCount == 0 ) {
    this->n++;
    this->d = this->d - (2 * this->d) / (4 * this->n + 1);
    if ( this->d <= this->maxSpeed ) {  // reached max speed
      this->d = this->maxSpeed;         // set delay value to maxspeed
      this->rampUpStepCount = this->stepCount; // set rampup phase counter to step counter value
    }
    // ----------------  reached halfway point  ----------------
    if ( this->stepCount >= this->totalSteps / 2 ) {
      this->rampUpStepCount = this->stepCount; // set rampup phase counter to step counter value
    }
  }
  // --------------------  ramp down phase  --------------------
  else if ( this->stepCount >= this->totalSteps - this->rampUpStepCount ) {
    this->n--;                                                        //
    this->d = (this->d * (4 * this->n + 1)) / (4 * this->n + 1 - 2);  // set delay value to
  }
  OCR1A = this->maxSpeed + this->d;   //  set the TIMER1 Comparator to maxspeed + delay value calculated
}
