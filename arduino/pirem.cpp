#include <Arduino.h>

#include "pirem.h"


class PIREM PIREM;


#define DATA_PIN_PIR 8




//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
PIREM::PIREM()
{
  _status=false;
  pinMode(DATA_PIN_PIR, INPUT);     // declare sensor as input
}
//------------------------------------------------
int PIREM::refresh(void)
{
  int rt=PIR_INFO_LOW2LOW;
  int aux=0;
  aux = digitalRead(DATA_PIN_PIR);   
  //Serial.print(aux);
  if (aux==HIGH){
    //new Fresh value is HIGH
    if (_status)  {   rt=PIR_INFO_HIGH2HIGH;}
    else          {   
                      rt=PIR_INFO_LOW2HIGH;   
                      Serial.println("PIR_INFO_LOW2HIGH");
                  }
  }
  else { 
    //new Fresh value is LOW
    if (_status)  {   
                      rt=PIR_INFO_HIGH2LOW;   
                      Serial.println("PIR_INFO_HIGH2LOW");
                  }
    else          {   rt=PIR_INFO_LOW2LOW;}
  }
  _status=aux;
  return rt;
}


