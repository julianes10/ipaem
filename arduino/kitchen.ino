#include <stdio.h>
#include "lsem.h"
#include "pirem.h"
#include "photoResistor.h"


//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    GLBserialInputString[100]; // a string to hold incoming data
char    GLBauxString[100];
int     GLBserialIx=0;
bool    GLBserialInputStringReady = false; // whether the string is complete

const char inputBootString[] PROGMEM ={":LP0020:LT0005:LMK:LCFF,00,00:LQ:LMk:LQ:LMN"};

const char inputPIRLOW2HIGHstring[]  PROGMEM = {":LT0020:LP0200:LMN:LQ:LCFF,FF,FF:LT0000:LMA"};
const char inputPIRHIGH2LOWstring[]  PROGMEM = {":LCFF,FF,FF:LT0030:LMA:LQ:LT0050:LP0500:LMN"};
const char inputPIRBack2HIGHstring[] PROGMEM = {":LCFF,FF,FF:LT0000:LMA"};

int GLBpirInfo=0;
bool GLBisDark=true;


//------------------------------------------------
//--- GLOBAL FUNCTIONS ---------------------------
//------------------------------------------------

//-- SOME NEEDED PROTOTYPES ---------------------------

void STATE_init(void);
void STATE_welcome(void);
void STATE_idle(void);
void STATE_LDPIRon(void);
void STATE_LDPIRoff(void);
void STATE_LDcmd(void);

// State pointer function
void (*GLBptrStateFunc)();


//------------------------------------------------

void setup() { 

  // Serial to debug AND comunication protocolo with PI              
  Serial.begin(9600);
  Serial.println(F("Setup... 'came on, be my baby, came on'"));
  GLBserialInputString[0]=0;

  GLBptrStateFunc = STATE_init;
  Serial.println(F("STATE INIT"));
  
}

//------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
     char inChar = (char)Serial.read();
     if (inChar < 0x20) {
       GLBserialInputStringReady = true;
       GLBserialInputString[GLBserialIx]=0;
       GLBserialIx=0;
       return;
     }
     GLBserialInputString[GLBserialIx++]=inChar;
  }
}

//-------------------------------------------------
void processSerialInputString()
{
  strcpy(GLBauxString,GLBserialInputString);
  GLBserialInputString[0]=0;
  GLBserialInputStringReady = false;
  LSEM.processCommands(GLBauxString);
}
//-------------------------------------------------
void STATE_init(void)
{
  Serial.println(F("DEBUG: inputBootString..."));
  strcpy_P(GLBauxString,(char*)inputBootString);
  LSEM.processCommands(GLBauxString);

  GLBptrStateFunc=STATE_welcome;
  Serial.println(F("STATE INIT -> WELCOME"));
}
//-------------------------------------------------
void STATE_welcome(void)
{
  if (LSEM.isIdle()) {
    GLBptrStateFunc=STATE_idle;
    Serial.println(F("STATE WELCOME -> IDLE"));
  }
}
//-------------------------------------------------
void STATE_idle(void)
{
  if (GLBserialInputStringReady){
    processSerialInputString();
    GLBptrStateFunc=STATE_LDcmd;
    Serial.println(F("STATE IDLE -> LD CMD"));
  }
  else if ( GLBisDark &&
            ( (GLBpirInfo==PIR_INFO_LOW2HIGH) ||  
              (GLBpirInfo==PIR_INFO_HIGH2HIGH) )  ) {
    Serial.println(F("DEBUG: inputPIRLOW2HIGHstring..."));
    strcpy_P(GLBauxString,(char*)inputPIRLOW2HIGHstring);
    LSEM.processCommands(GLBauxString);
    GLBptrStateFunc=STATE_LDPIRon;
    Serial.println(F("STATE IDLE -> LD PIR ON"));

  }  
}
//-------------------------------------------------
void STATE_LDPIRon(void)
{
  if (GLBserialInputStringReady){
    processSerialInputString();
    GLBptrStateFunc=STATE_LDcmd;
    Serial.println(F("STATE LD PIR ON -> LD CMD"));
  }
  else if ( (GLBpirInfo==PIR_INFO_HIGH2LOW) ||  
            (GLBpirInfo==PIR_INFO_LOW2LOW) ){
    Serial.println(F("DEBUG: inputPIRHIGH2LOWstring..."));
    strcpy_P(GLBauxString,(char*)inputPIRHIGH2LOWstring);
    LSEM.processCommands(GLBauxString);
    GLBptrStateFunc=STATE_LDPIRoff;
    Serial.println(F("STATE LD PIR ON -> LD PIR OFF"));
  }
}
//-------------------------------------------------
void STATE_LDPIRoff(void)
{
  if (GLBserialInputStringReady){
    processSerialInputString();
    GLBptrStateFunc=STATE_LDcmd;
    Serial.println(F("STATE LD PIR OFF -> LD CMD"));
  }
  else if ( (GLBpirInfo==PIR_INFO_LOW2HIGH) ||  
            (GLBpirInfo==PIR_INFO_HIGH2HIGH) ){
    Serial.println(F("DEBUG: inputPIRBack2HIGHstring..."));
    strcpy_P(GLBauxString,(char*)inputPIRBack2HIGHstring);
    LSEM.processCommands(GLBauxString);
    GLBptrStateFunc=STATE_LDPIRon;
    Serial.println(F("STATE LD PIR OFF -> LD PIR ON"));
  }
}
//-------------------------------------------------
void STATE_LDcmd(void)
{
  if (GLBserialInputStringReady){
    processSerialInputString();
  }
  else if (LSEM.isIdle()) {
    GLBptrStateFunc=STATE_idle;
    Serial.println(F("STATE LD CMD -> IDLE"));
  }
}



//-------------------------------------------------
//-------------------------------------------------
//-------------------------------------------------
void loop() { 

  //------------- INPUT REFRESHING ----------------
  // Read PIR sensor
  GLBpirInfo=PIREM.refresh();

  // Read light sensor
  GLBisDark=PR.refresh();

  //--------- TIME TO THINK MY FRIEND -------------
  // State machine as main controller execution
  GLBptrStateFunc();


  // ------------- OUTPUT REFRESHING ---------------
  // Write led strip
  LSEM.refresh();

  // Write motor 
  //TODO

}
