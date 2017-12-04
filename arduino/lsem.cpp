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
  _timers.run();
  switch(_mode) {
    case LS_MODE_COLOR:         _doColor();         break;
    case LS_MODE_ROLLING_TEST:  _doRollingTest();   break;
    case LS_MODE_ROLLING_COLOR: _doRollingColor(_color); break;
    case LS_MODE_ONE:           _doOne();           break;
    case LS_MODE_RAINBOW:       _doRainbow();       break;
    case LS_MODE_NOISE:         _doNoise();         break;
    default: FastLED.clear();                       break;
  }
  FastLED.show();
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
void LSEM::reset(void)
{
  _mode=0;
  _one=0;
  _color=CRGB::Black; //C
  _setAllLeds(CRGB::Black);
  _rollingTurn=0;
  _rollingUnpaused=false;
  _rollingTestColor=0x0000FF;
  _timers.deleteTimer(_timerTimeout);
  _timerTimeout=-1;
  _timers.deleteTimer(_timerPause);
  _timerPause=-1;
}

//------------------------------------------------
void _callbackTimeout(void)
{
  Serial.println("DEBUG: callbackTimeout");

  LSEM.reset();
}
//------------------------------------------------
void _callbackPause(void)
{
  //Serial.println("DEBUG: callbackPause");
  LSEM._rollingUnpaused=true;
}


//------------------------------------------------
void LSEM::setTimeout(uint16_t t)
{
  if (_timerTimeout >= 0) _timers.deleteTimer(_timerTimeout);

  _timerTimeout=-1;
  if (t>0){
    _timerTimeout=_timers.setTimeout((long int)t*100,_callbackTimeout);
    Serial.print("DEBUG: setTimeout to ");
    Serial.print(t*100);
    Serial.print(" ms. Timer number: ");
    Serial.println(_timerTimeout);
  }
}
//------------------------------------------------
void LSEM::setPause(uint16_t t)
{
  if (_timerPause >= 0) _timers.deleteTimer(_timerPause);
  _timerPause=-1;
  if (t>0){
    _timerPause=_timers.setInterval((long int)t,_callbackPause);
    Serial.print("DEBUG: setPause to ");
    Serial.print(t);
    Serial.print(" ms. Timer number: ");
    Serial.println(_timerPause);
  }
}
//------------------------------------------------
void LSEM::setMode(char m)
{
  Serial.print("DEBUG: setMode from: ");
  Serial.print(_mode);
  Serial.print(" to: ");
  Serial.println(m);
  _mode=m;
}
//------------------------------------------------
void LSEM::setColor(uint32_t c)
{ 
  Serial.print("DEBUG: setColor from: ");
  Serial.print(_color,HEX);
  Serial.print(" to: ");
  Serial.println(c,HEX);
  _color=(CRGB)c;
}
//------------------------------------------------
void LSEM::setLed(uint8_t led)
{
  _one=led;
}
//------------------------------------------------
void LSEM::_doOne()
{
  _leds[_one] = CRGB::White;
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
 if (_rollingUnpaused)
 {
   if (_rollingTurn>=NUM_LEDS)  _rollingTurn=0;

   _setAllLeds(CRGB::Black);
   _leds[_rollingTurn++] = color;
   _rollingUnpaused=false;
 }
}

//------------------------------------------------
void LSEM::_doRollingTest()
{
  if ( _rollingTurn==NUM_LEDS)
  { 
    if      (_rollingTestColor == (CRGB)CRGB::Red)    _rollingTestColor=CRGB::Green;
    else if (_rollingTestColor == (CRGB)CRGB::Green)  _rollingTestColor=CRGB::Blue;
    else if (_rollingTestColor == (CRGB)CRGB::Blue)   _rollingTestColor=CRGB::Red;
    else                                              _rollingTestColor=CRGB::Red;
  }
  _doRollingColor(_rollingTestColor); 
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

