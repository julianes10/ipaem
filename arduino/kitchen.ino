#include <SoftwareSerial.h>
#include <stdio.h>
#include "lsem.h"

String inputString = ""; // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete

void setup() { 

  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println("Setup... 'came on, be my baby, came on'");
  Serial.println("Setup... 'came on, be my baby, came on'");
  Serial.println("Setup... 'came on, be my baby, came on'");
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
     char inChar = (char)Serial.read();
     if (inChar < 0x20) {
       stringComplete = true;
       break;
     }
     inputString += inChar;
  }
}

//-------------------------------------------------
/*
EM SERIAL PROTOCOL, INEFFICIENT,UNSECURE, BUT EASY TO READ, QUICK TO IMPLEMENT AND TEST:
:<TYPE>[<SUBTYPE>|<VALUE><SUBTYPE>|<VALUE>...] ;[more commands....]...\n
TYPES:
*/
  #define TYPE_LED    'L' // (led strip)
  #define TYPE_STATUS 'S' // (ipaem status)
// For subtypes of leds see and protocol details go to  lsem.h

void readSerialCommand() {
  char m=0;
  uint8_t index=0;
  uint8_t len=0;
  uint32_t color=0;

  if (!stringComplete)   {goto exiting;}
  
  Serial.print("DEBUG:"); Serial.println(inputString);
  //TODO treat several commands in one line
  index=inputString.indexOf(':');
  len=inputString.length();
  if (index <0)         {Serial.println("DEBUG: void command");       goto cleanAndExit;}
  if ((len-index) <=0)  {Serial.println("DEBUG: incomplete command"); goto cleanAndExit;}
  index++; 
  switch(inputString.charAt(index)){
    case TYPE_STATUS:
      //TODO
      break;
    case TYPE_LED:
      index++;
      if ((len-index) <=0)  {Serial.println("DEBUG: incomplete led command"); goto cleanAndExit;}
      switch(inputString.charAt(index)){
        case LS_RESET:
          index++;
          LSEM.reset();
          break;
        case LS_COLOR:
          index++;
          if ((len-(index+6)) <0) {Serial.println("DEBUG: incomplete ls_color"); goto cleanAndExit;}
          sscanf(inputString.substring(index,index+6).c_str(),"%X",&color);
          LSEM.setColor(color);
          index=+6;
          break;
        case LS_TIMEOUT:
          index++;
          if ((len-(index+4)) <0) {Serial.println("DEBUG: incomplete ls_timeout"); goto cleanAndExit;}
          LSEM.setTimeout((uint16_t)inputString.substring(index,index+4).toInt());
          index=+4;
          break;
        case LS_PAUSE:
          index++;
          if ((len-(index+4)) <0) {Serial.println("DEBUG: incomplete ls_timeout"); goto cleanAndExit;}
          LSEM.setPause((uint16_t)inputString.substring(index,index+4).toInt());
          index=+4;
          break;
        case LS_MODE:
          index++;
          m=inputString.charAt(index);
          LSEM.setMode(m);
          //TODO backup M just in case
          switch(m){
              case LS_MODE_RAINBOW:
              case LS_MODE_COLOR:
              case LS_MODE_NOISE:
              case LS_MODE_ROLLING_TEST:
              case LS_MODE_ROLLING_COLOR:
                index++;
                break;
              case LS_MODE_ONE:
                index++;
                if ((len-(index+2)) <0) {Serial.println("DEBUG: incomplete ls_mode_one"); goto cleanAndExit;}
                LSEM.setLed((uint8_t)inputString.substring(index,index+2).toInt());
                index=+2;
                break;
              default: LSEM.setMode(0); Serial.println("DEBUG: LS unexpected mode"); goto cleanAndExit;
          }
          break;
        default: Serial.println("DEBUG:LS unexpected subtype"); goto cleanAndExit;
      };
      break;
    default: Serial.println("DEBUG: PROTOCOL unexpected type"); goto cleanAndExit;
  };
  Serial.println("DEBUG: Command processed successfully");

cleanAndExit:
  inputString = "";
  stringComplete = false;
exiting:;
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
  LSEM.refresh();

  // Write motor 
  //TODO



}
