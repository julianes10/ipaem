#ifndef LSEM_h
#define LSEM_h

#include "Arduino.h"
#include "FastLED.h"
#include "SimpleTimer.h"

#define NUM_LEDS 46

//-------------
//SUBTYPES led strip PROTOCOL
  #define LS_RESET   'X'  // off led strip and reset any variable to default
  #define LS_COLOR   'C'  // (color) with VALUE:RGB in 6 ascii hexadecimal 
                          //    e.g 66AB4F. All led set to this color. Default 000000
  #define LS_TIMEOUT 'T'  // (timeout) with VALUE:DS in 4 ascii in decs of seconds filled with zeros 
                          //    e.g 0300, 30 seconds. Default 0 NO timeout
  #define LS_PAUSE   'P'  // (pause)   with VALUE:MS in 4 ascii in decs of seconds filled with zeors 
                          //    e.g 4000, 4  seconds. Default 0 no pause
  #define LS_MODE    'M'  // (mode) with <SUBTYPE>
    #define LS_MODE_COLOR         'A'  // (all) setup all leds with general settings: C,T,P.
    #define LS_MODE_ROLLING_TEST  'T'  // rolling 3 test colors. With general settings: T,P.
    #define LS_MODE_ROLLING_COLOR 'C'  // rolling bit With general settings: C,T,P.
    #define LS_MODE_ONE           '1'  //(1 led bit change) with VALUE:LED_POSITION 2 ascii decimal 
                                       //  e.g 00 or 40. With general settings: C,T,P.
    #define LS_MODE_RAINBOW       'W'  //rainbow. With general settings: T.
    #define LS_MODE_NOISE         'N'  //rainbow. With general settings: T,P

class LSEM
{
 public:

  LSEM();

  void refresh();

  void setMode(char m);
  void setColor(uint32_t c);
  void setLed(uint8_t led);
  void setTimeout(uint16_t t);
  void setPause(uint16_t p);
  void reset();

  bool _rollingUnpaused;


 private:
  CRGB _leds[NUM_LEDS];
  char _mode;
  uint8_t _one;
  int  _timerTimeout; //T
  int  _timerPause; //P
  CRGB _color; //C
  SimpleTimer _timers;
  uint8_t _rollingTurn;
  CRGB _rollingTestColor;



  void _doRollingTest();
  void _doRollingColor(CRGB color);
  void _doOne();
  void _doColor();
  void _doRainbow();
  void _doNoise();
  void _setAllLeds(CRGB color);

 };

extern class LSEM LSEM;
#endif


