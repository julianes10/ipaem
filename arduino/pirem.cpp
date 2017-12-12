#include <Arduino.h>

#include "pirem.h"


class PIREM PIREM;


#define DATA_PIN 8




//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
PIREM::PIREM()
{
  _status=false;
  pinMode(DATA_PIN, INPUT);     // declare sensor as input
}
//------------------------------------------------
int PIREM::refresh(void)
{
  int rt=PIR_INFO_LOW2LOW;
  int aux=0;
  aux = digitalRead(DATA_PIN);   
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


