#ifndef PIREM_h
#define PIREM_h

#include "Arduino.h"


#define PIR_INFO_LOW2LOW   0
#define PIR_INFO_LOW2HIGH  1
#define PIR_INFO_HIGH2HIGH 2
#define PIR_INFO_HIGH2LOW  3
class PIREM
{
 public:

  PIREM();

  int refresh();
  bool getStatus(){return _status;}

 private:
  bool _status;

 };

extern class PIREM PIREM;
#endif


