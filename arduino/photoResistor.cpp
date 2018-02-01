#include <Arduino.h>

#include "photoResistor.h"


class PR PR;


#define DATA_PIN A0
#define MIN_LIGHT_TH_VALUE  70
#define MAX_LIGHT_TH_VALUE  150
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
PR::PR()
{
  _status=false;
  for (int i=0;i<8;_windowValues[i]=0,i++);
  _index=0;
}
//------------------------------------------------
bool PR::refresh(void)
{
  int  sensorValue =  0;
  sensorValue =  analogRead(DATA_PIN);

  /*Serial.print("Last Light value:");
  Serial.println(sensorValue,  DEC); */

  _windowValues[_index++]=sensorValue;
  if (_index>8) _index=0;

  for (int i=0;i<8;sensorValue+=_windowValues[i],i++);
  sensorValue=(sensorValue<<3);  //Divides by 8
  if (sensorValue > MAX_LIGHT_TH_VALUE){
    if (!_status)  {   
      Serial.println("LIGHT NOT ENOUGH");
    }   

   _status=true;
  }
  else if (sensorValue < MIN_LIGHT_TH_VALUE){
    if (_status)  {   
      Serial.println("LIGHT ENOUGH");
    }   
    _status=false;
  }  
  /*Serial.print("Integrate value:");
  Serial.println(sensorValue,  DEC);*/
  return _status;
}

