#include <stdio.h>
#include "lsem.h"
#include "pirem.h"


//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    serialInputString[100]; // a string to hold incoming data
int     serialIx=0;
bool serialInputStringReady = false; // whether the string is complete
const char inputBootString[] PROGMEM ={":LP0020:LT0050:LMK:LCFF,00,00:LQ:LP0015:LT0050:LMk:LCFF,00,00"}; // a string to welcome
bool inputBootStringDone = false; // whether the string is complete


const char inputPIRLOW2HIGHstring[] PROGMEM = {":LCAA,BB,CC:LMA"};
const char inputPIRHIGH2LOWstring[] PROGMEM = {":LX"};
bool  isOngoingPIR_LD=false;

//------------------------------------------------
//--- GLOBAL FUNCTIONS ---------------------------
//------------------------------------------------
void setup() { 

  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println(F("Setup... 'came on, be my baby, came on'"));
  serialInputString[0]=0;
  
}

//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
     char inChar = (char)Serial.read();
     if (inChar < 0x20) {
       serialInputStringReady = true;
       serialInputString[serialIx]=0;
       serialIx=0;
       return;
     }
     serialInputString[serialIx++]=inChar;
  }
}

//-------------------------------------------------
void processInputs(int pirInfo) {

  char inputString[100];
  if (!inputBootStringDone){
    //inputString=(char*)inputBootString;//
    Serial.println(F("DEBUG: inputBootString..."));
    strcpy_P(inputString,(char*)inputBootString);
    inputBootStringDone=true;
  }
  else if (serialInputStringReady){
    //inputString=(char*)serialInputString;//
    strcpy(inputString,serialInputString);
    serialInputString[0]=0;
    serialInputStringReady = false;
  }
  else if ( (LSEM.isIdle()) && (pirInfo==PIR_INFO_LOW2HIGH) ) {
    Serial.println(F("DEBUG: inputPIRLOW2HIGHstring..."));
    strcpy_P(inputString,(char*)inputPIRLOW2HIGHstring);
    isOngoingPIR_LD=true;
  }
  else if ( (pirInfo==PIR_INFO_HIGH2LOW) && isOngoingPIR_LD ) {
    Serial.println(F("DEBUG: inputPIRHIGH2LOWstring..."));
    strcpy_P(inputString,(char*)inputPIRHIGH2LOWstring);
    isOngoingPIR_LD=false;
  }
  else return;

  LSEM.processCommands(inputString);

}
//-------------------------------------------------
void loop() { 

  //------------- INPUT REFRESHING ----------------
  // Receive command && read PIR
  processInputs(PIREM.refresh()); 
 
  // Read light sensor
  //TODO
  // ------------- OUTPUT REFRESHING ---------------

  // Write led strip
  LSEM.refresh();

  // Write motor 
  //TODO



}
