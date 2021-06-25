class Stepper{
  public:
    //
    void SetHardWare(int rotation ,int mm_rev);
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

   private:

    unsigned int enable_pin;
    unsigned int step_pin;
    unsigned int diraction_pin;
 
    int pos_init=0;
    int angle_init=0;
    int mm_init=0;
    
    volatile int step=0;
    volatile int angle=0;
    volatile int mm=0;

    volatile int stepPosition = 0;
    volatile int n_rotation=0;

    unsigned int rotation=400;
    volatile unsigned int mm_rev;

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
