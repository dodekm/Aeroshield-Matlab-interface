
#ifndef AEROSHIELD_H			      
#define AEROSHIELD_H	
            
#include <Arduino.h>			
#include "AS5600.h"

#define AERO_RPIN A3             
#define AERO_UPIN 5   


class Sampler
{

public:
void sample(float value_){value=value_; missed=available; available=true;}
bool isAvailable(){return available;}
bool isMissed(){return missed;}
float read(){ available=false; missed=false; return value;}
float getLastValue(){return value;}



private:
volatile float value = 0;
volatile bool available = false;
volatile bool missed = false;

};

class AeroClass{		    	                         
 
 public:
  bool begin(float);                                              
  void actuatorWrite(float);             
                   
  float referenceRead(void);                       
  float sensorReadDegree(void); 
  float sensorReadDegreeSampled(void);    
  bool isSampleAvailable(){return y_sampler.isAvailable();}               
  void calibrate(void);
  unsigned long getTick(){return tick;}
 
protected:

  Sampler y_sampler;
  unsigned long tick=0;
   
 private: 
                         
   float mapFloat(float , float , float , float , float );
   byte percToPwm(float);

   uint16_t _zero;
   
  float Ts=0.05; 
   AS5600 as5600;
   
};

extern AeroClass& AeroShield; 



#endif
