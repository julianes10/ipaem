#ifndef PHOTORESISTOR_h
#define PHOTORESISTOR_h

#include "Arduino.h"

class PR
{
 public:

  PR();

  bool refresh();
  bool isDark(){return _status;}

 private:
  bool _status;
  int _windowValues[8];
  int _index=0;

 };

extern class PR PR;
#endif


