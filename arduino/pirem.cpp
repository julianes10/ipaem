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
    if (_status)  {   rt=PIR_INFO_HIGH2HIGH;}
    else          {   
                      rt=PIR_INFO_LOW2HIGH;   
                      Serial.println("PIR_INFO_LOW2HIGH");
                      _status=true;
                  }
  }
  else { // (aux==LOW){
    if (_status)  {   
                      rt=PIR_INFO_HIGH2LOW;   
                      Serial.println("PIR_INFO_HIGH2LOW");
                      _status=false;
                  }
    else          {   rt=PIR_INFO_LOW2LOW;}
  }
  return rt;
}


