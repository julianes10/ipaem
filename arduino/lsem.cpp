#include <Arduino.h>

#include "FastLED.h"
#include "lsem.h"

class LSEM LSEM;


#define DATA_PIN 7
//------------------------------------------------
LSEM::LSEM()
{
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(_leds, NUM_LEDS);
  reset();
}
//------------------------------------------------
void LSEM::refresh(void)
{
  switch(_mode) {
    case LS_MODE_COLOR:         _doColor();         break;
    case LS_MODE_ROLLING_TEST:  _doRollingTest();   break;
    case LS_MODE_ROLLING_COLOR: _doRollingColor(_color); break;
    case LS_MODE_ONE:           _doOne();           break;
    case LS_MODE_RAINBOW:       _doRainbow();       break;
    case LS_MODE_NOISE:         _doNoise();         break;
    default: FastLED.clear(true);                   break;
  }
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
void LSEM::reset(void)
{
  _mode=0;
  _one=0;
  _dsecTimeout=0; //T
  _msPause=0; //P
  _color=CRGB::Black; //C
  _setAllLeds(CRGB::Black);
}


//------------------------------------------------
void LSEM::_doOne()
{
  _leds[_one] = CRGB::White; FastLED.show();
}

//------------------------------------------------
void LSEM::_setAllLeds(CRGB color)
{
  // Turn the LED on, then pause
  for (int i=0;i<NUM_LEDS;i++)
  {
    _leds[i] = color;
  }
}
//------------------------------------------------
void LSEM::_doRollingColor(CRGB color)
{
 for (int turn=0;turn<NUM_LEDS;turn++)
 {
  _setAllLeds(CRGB::Black);
  _leds[turn] = color;
  FastLED.show();
  delay(_msPause);
 }
}

//------------------------------------------------
void LSEM::_doRollingTest()
{
  _doRollingColor(CRGB::Red); 
  _doRollingColor(CRGB::Green);
  _doRollingColor(CRGB::Blue);
}


//------------------------------------------------
void LSEM::_doColor()
{ 
  _setAllLeds(_color);
}
//------------------------------------------------
void LSEM::_doRainbow()
{ //TODO
}
//------------------------------------------------
void LSEM::_doNoise()
{ //TODO
}

