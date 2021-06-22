#include "lib.h"
#include "Arduino.h"

void Stepper::constantAccel() {
  
  float angle = 1;
  float accel = 0.01;
  float c0 = 2000 * sqrt( 2 * angle / accel ) * 0.67703;
  float lastDelay = 0;
  int highSpeed = 100;
  for (int i = 0; i < 800; i++) {
    float d = c0;
    if ( i > 0 )
      d = lastDelay - (2 * lastDelay) / (4 * i + 1);
    if ( d < highSpeed )
      d = highSpeed;
    this->delays[i] = d;
    lastDelay = d;
  }
}

void Stepper::move(int S){
  for(int i=0;i<S;i++){
    digitalWrite(this->step_pin,1);
    delayMicroseconds(delays[i]);
    digitalWrite(this->step_pin,0);
  }
  delay(1/this->speed);
}

void Stepper::SetDefaultPosition(){

  Serial.println("----------  position setting  ----------");
  Serial.println();
  Serial.println("press on any button when you get the 0 position");
  while(!(Serial.available())){
    this->write(0,1);
    delay(10);
  }
  int a=Serial.read();
  Serial.println();
}

void Stepper::SetHardWard(int rotation,int mode_tr,int nb_micropas_driver,int step_courroie,int nombre_dents_poulie,int pas_filetage){
  this->rotation=rotation;
  this->mode_tr=mode_tr;
  this->nb_micropas_driver=nb_micropas_driver;
  this->step_courroie=step_courroie;
  this->nombre_dents_poulie=nombre_dents_poulie;
  this->pas_filetage=pas_filetage;
}

void Stepper::init(int enable_pin,int step_pin,int diraction_pin){
      
  this->enable_pin=enable_pin;
  this->step_pin=step_pin;
  this->diraction_pin=diraction_pin;
  
  this->constantAccel();

  pinMode(step_pin,OUTPUT);
  pinMode(diraction_pin,OUTPUT);
  pinMode(enable_pin,OUTPUT);
  digitalWrite(enable_pin,0);

  this->SetDefaultPosition();
}
  
void Stepper::write(char mode,int parametre){
  
  if(parametre<0){
    digitalWrite(this->diraction_pin,1);
  }
  else{
    digitalWrite(this->diraction_pin,0);
  }
  
  switch(mode){
    
    case 0:
      
      this->move(parametre);
      this->step = this->step + parametre;
      break;
      
    case 1:
      
      this->move(this->rotation/(360/parametre));
      this->step = this->step + this->rotation/(360/parametre);
      break;
      
    case 2:
      
      this->move(parametre*this->rotation);
      this->step = this->step + parametre*this->rotation;
      break;
      
    case 3:

      if(this->mode_tr==0){
        this->move(parametre*((this->rotation*this->nb_micropas_driver)/(step_courroie*nombre_dents_poulie)));
        this->step = this->step + parametre*((this->rotation*this->nb_micropas_driver)/(step_courroie*nombre_dents_poulie));
      }
      else if(this->mode_tr==1){
        this->move(parametre*((this->rotation*this->nb_micropas_driver)/pas_filetage));
        this->step = this->step + parametre*((this->rotation*this->nb_micropas_driver)/pas_filetage);
      }
      break;  
      
    }
    
    this->n_rotation= this->step / this->rotation;
    this->angle = this->step*(this->rotation/360);
    if(this->mode_tr==0){
      this->mm=this->step/((this->rotation*this->nb_micropas_driver)/(step_courroie*nombre_dents_poulie));
    }
    else if(this->mode_tr==1){
      this->mm=this->step/((this->rotation*this->nb_micropas_driver)/pas_filetage);
    }
    
}

void Stepper::writeAbs(char mode,int parametre){
  
      switch(mode){
        
        case 0:
          if(this->step<this->rotation){
            this->write(0,parametre);
          }
          break;
          
        case 1:
          break;
          
        case 2:
          break;
        case 3:
          break;  
      }
    }

int Stepper::getposition(char mode){
  
      switch(mode){
        
        case 0:
          return this->step;
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

void Stepper::setposition(char mode,int parametre){
      switch(mode){
        case 0:
          this->pos_init=parametre;
          break;
        case 1:
          this->angle_init=parametre;
          break;
        case 3:
          break;  
      }
    }

void Stepper::setSpeed(char mode,int parametre){
  switch(mode){
    
    case 0:
      this->speed = parametre;
      break;
      
    case 1:
      
      break;
      
    case 2:
      this->speed = parametre*this->rotation;
      break;
      
    case 3:
      break;  
   }
}

float Stepper::getSpeed(char mode){
  switch(mode){
    
    case 0:
      return this->speed;
      break;
      
    case 1:
      return this->speed/(this->rotation/360);
      break;
      
    case 2:
      return this->speed/this->rotation;
      break;
      
    case 3:
      
      break;  
  }
}
