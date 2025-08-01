
#include "AeroShield.h"      

#include "TimerOne.h"

static void timer_callback();

bool AeroClass::begin(float Ts){    
  this->Ts=Ts;                            
  bool res;
  Wire.begin();  
  Wire.setClock(400000);                                                                                                 
  res=as5600.begin();
  if (!res) return res;
  res= as5600.detectMagnet();
  if (!res) return res;
  as5600.setOffset(-200);
  calibrate();
  delay(200); 
  if (Ts>0.0)
  {
    Timer1.initialize(Ts*1e6);
    Timer1.attachInterrupt(timer_callback);
  }
  return res;
      
} 

void AeroClass::calibrate(void){                          
 _zero = as5600.readAngle();                                          
}

float AeroClass::referenceRead(void) {                                                
  return mapFloat(analogRead(AERO_RPIN), 0.0, 1024.0, 0.0, 100.0);  
}

void AeroClass::actuatorWrite(float percentValue) {    

  percentValue=max(percentValue,0);
  percentValue=min(percentValue,100);
  analogWrite(AERO_UPIN, percToPwm(percentValue));        
}

float AeroClass::sensorReadDegree(){
  uint16_t angle = as5600.readAngle();
  return ((int)(angle - _zero)) * AS5600_RAW_TO_DEGREES; 
}

float AeroClass::sensorReadDegreeSampled(){
 return y_sampler.read();
}



float AeroClass::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) 
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; 
}

byte AeroClass::percToPwm(float perc){
	return byte(mapFloat(perc, 0.0, 100, 0.0, 255.0));
}


class AeroClass_ : public AeroClass{	

  public: 
  void timer_tick()
  {
    y_sampler.sample(sensorReadDegree());
    tick++;
  }

};


 AeroClass_ AeroShield_; 
 AeroClass& AeroShield=AeroShield_;

static void timer_callback()
{
  interrupts();
  AeroShield_.timer_tick();
}



