

class Stepper{
  public:
    //
    void SetHardWare(int rotation,int mode_tr,int nb_micropas_driver,int mm_rev);
    Stepper(int enable_pin,int step_pin,int diraction_pin);
    void setup_step();
    void write(char mode,int parametre);
    void writeAbs(char mode,int parametre);
    int getposition(char mode);
    void setposition(char mode,int parametre);
    void setSpeed(char mode,int parametre);
    int getSpeed(char mode);
    void interrupt();
    void SetDefaultPosition();

    volatile int stepPosition = 0;

   private:

    int enable_pin;
    int step_pin;
    int diraction_pin;

    int rotation=400;
 
    int pos_init=0;
    int angle_init=0;
    int mm_init=0;

    int pos=0;
    
    int step=0;
    int angle=0;
    int n_rotation=0;
    int mm=0;

    int mode_tr;
    int nb_micropas_driver;
    int mm_rev;

    unsigned int c0;

    volatile int dir = 0;
    volatile unsigned int maxSpeed = 6000;
    volatile unsigned long n = 0;
    volatile float d;
    volatile unsigned long stepCount = 0;
    volatile unsigned long rampUpStepCount = 0;
    volatile unsigned long totalSteps = 0;
    

    volatile bool movementDone = false;

    void move(int S);

};
