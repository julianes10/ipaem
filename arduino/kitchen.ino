#include "FastLED.h"
#include <SoftwareSerial.h>
#include <stdio.h>

// How many leds in your strip?
#define NUM_LEDS 46

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 7

String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete


// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println("Setup... 'came on, be my baby, came on'");
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}


void testColorRollingLed(CRGB color)
{
 for (int turn=0;turn<NUM_LEDS;turn++)
 {
  // Turn the LED on, then pause
  for (int i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Black;
  }
  /*Serial.print("Next LED: ");
  Serial.print(turn);
  Serial.print(" with color:");
  Serial.println(color,HEX);*/
  leds[turn] = color;
  FastLED.show();
  delay(10);
 }
}
boolean doRolling=false;
//------------------------------------------------
void refreshLedStrip()
{
  if (doRolling)
  {
    testColorRollingLed(CRGB::Red); 
    testColorRollingLed(CRGB::Green);
    testColorRollingLed(CRGB::Blue);
  }
  doRolling=false;
}


//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
     // get the new byte:
     char inChar = (char)Serial.read();
     // add it to the inputString:

     // if the incoming character is a newline, set a flag
     // so the main loop can do something about it:
     if (inChar < 0x20) {
       stringComplete = true;
       break;
     }
     inputString += inChar;
  }
}


//-------------------------------------------------
void readSerialCommand() {
  // print the string when a newline arrives:
  if (stringComplete) {
    //Process string
    Serial.print("DEBUG:");
    Serial.println(inputString);
    
    //PROCESS COMMAND
    if (inputString == "ROLLING")
    {
      Serial.println("DO ROLLING");
      doRolling=true;       
    }
    else {
      Serial.println("NO ROLLING");
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}


void loop() { 

  //------------- INPUT REFRESHING ----------------
  // Receive command 
  readSerialCommand(); 
  // Read PIR
  //TODO
  // Read light sensor
  //TODO
  // ------------- OUTPUT REFRESHING ---------------

  // Write led strip
  refreshLedStrip();

  // Write motor 
  //TODO



}
