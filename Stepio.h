#ifndef STEP_
#define STEP_

/*
 * Stepio.h - Interrupt driven Stepper library for Arduino. 
 * 
 * the stpper is used by activating creating Stepper Class passing 
 * the desired 3 pin ( enable pin , step pin , direction pin), call
 * setup_step() method in void setup to inisialize your stepper
 * 
 * if you want to turn your motor call write(char mode,int parametre)
 * method using the mode that define unit and parametre that define
 * number and direction of steps/rotation/degree/mm
 *  
 */

 
class Stepper{
  public:

    Stepper(int enable_pin,int step_pin,int diraction_pin);         //  contractor method of the class
  
    void SetHardWare(int rotation ,int mm_rev);                     //  setup the hardware config
    void setup_step();                                              //  method to call in void setup()
    
    void write(char mode,int parametre);                            //  turn motor parametre in deserved mode 
    void writeAbs(char mode,int parametre);                         //  like write(char mode,int parametre) but on one revolution
    
    int getposition(char mode);                                     //  get the current position of the motor
    void setposition(char mode,int parametre);                      //  set the zero point of the motor
    
    void setSpeed(char mode,int parametre);                         //  set the speed motor 
    int getSpeed(char mode);                                        //  get the current speed of motor
    
    void interrupt();                                               //  call in ISR function


   private:

    // -------------------    pin of the motor    -------------------
    unsigned int enable_pin;                                        // the pin attached to enable pin of the driver
    unsigned int step_pin;                                          // the pin attached to step pin of the driver
    unsigned int diraction_pin;                                     // the pin attached to diraction pin of the driver
    // -------------------    initial position    -------------------
    int pos_init=0;
    int angle_init=0;
    int mm_init=0;
    // ------ number of step and evolution done by the motor  -------
    volatile int step=0;                                            // number of step done by the stepper
    volatile int n_rotation=0;                                      // number of rotation done by the stepper
    // ------------------- position of the motor  -------------------
    volatile int stepPosition = 0;                                  // initial position in step
    volatile int angle=0;                                           // initial position in °
    volatile int mm=0;                                              // initial position in mm
    // -------------------    hardware settings   -------------------
    unsigned int rotation=400;                                      // number of step needed in one rotation
    volatile unsigned int mm_rev;                                   // distance in mm done in one rotation
    // -------------------   speed and direction  -------------------
    volatile unsigned int maxSpeed = 10;                            // the max speed of the stepper
    volatile int dir = 0;                                           // direction of the stepper (1: direct ,0: stop ,-1: indirect)
    // ------------------- the calculation -------------------
    unsigned int c0;                                                // first delay value
    volatile unsigned long n = 0;                                   // 
    volatile float d;                                               // delay value
    volatile unsigned long stepCount = 0;                           // step counter value
    volatile unsigned long rampUpStepCount = 0;                     //
    volatile unsigned long totalSteps = 0;                          // number of steps must be done by stepper
    volatile bool movementDone = false;                             // movementDone flag that define the stepper

    
    void move(int S);            // this function display the timer and turn the stepper smothly

};

#endif
