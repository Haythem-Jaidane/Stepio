class Stepper{

  private:

    int enable_pin;
    int step_pin;
    int diraction_pin;

    float speed = 0.1;
    int rotation=400;
 
    int pos_init=0;
    int angle_init=0;
    
    int step=0;
    int angle=0;
    int n_rotation=0;
    int mm=0;

    int delays[800];

    int mode_tr;
    int nb_micropas_driver;
    int step_courroie;
    int nombre_dents_poulie;
    int pas_filetage;
    

    void constantAccel();
    void move(int S);
    void SetDefaultPosition();


  public:

    void SetHardWard(int rotation,int mode_tr,int nb_micropas_driver,int step_courroie,int nombre_dents_poulie,int pas_filetage);
    void init(int enable_pin,int step_pin,int diraction_pin);
    void write(char mode,int parametre);
    void writeAbs(char mode,int parametre);
    int getposition(char mode);
    void setposition(char mode,int parametre);
    void setSpeed(char mode,int parametre);
    float getSpeed(char mode);
};
