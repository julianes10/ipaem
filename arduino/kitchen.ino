#include <SoftwareSerial.h>
#include <stdio.h>
#include "lsem.h"
#include "pirem.h"


//------------------------------------------------
//--- GLOBAL VARIABLES ---------------------------
//------------------------------------------------
char    serialInputString[50]; // a string to hold incoming data
int     serialIx=0;
boolean serialInputStringReady = false; // whether the string is complete
const char inputBootString[] PROGMEM ={":LP0050:LT0070:LMT"}; // a string to welcome
boolean inputBootStringDone = false; // whether the string is complete


const char inputPIRLOW2HIGHstring[] PROGMEM = {":LCAA,BB,CC:LMA"};
const char inputPIRHIGH2LOWstring[] PROGMEM = {":LX"};

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
       serialIx=0;
       break;
     }
     serialInputString[serialIx++]=inChar;
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

uint8_t readSerialCommand(char *cmd) {
  char m=0,bk=0;
  uint8_t index=0;
  uint8_t len=0;
  uint32_t color=0;
  int br=0,bg=0,bb=0;
  char aux[50];
  char **kk;

  len=strlen(cmd);
  Serial.print(F("DEBUG: Parsing:")); Serial.println(cmd);  
  switch(cmd[index]){
    case TYPE_STATUS:
      //TODO
      break;
    case TYPE_LED:
      index++;
      if ((len-index) <=0)  {Serial.println(F("DEBUG: incomplete led command")); goto exitingCmd;}
      switch(cmd[index]){
        case LS_RESET:
          index++;
          LSEM.reset();
          break;
        case LS_COLOR:
          index++;
          if ((len-(index+8)) <0) {Serial.println(F("DEBUG: incomplete ls_color")); goto exitingCmd;}
          Serial.print(F("DEBUG: new LS color:"));
          sscanf(&cmd[index],"%X",&br); index+=3;
          sscanf(&cmd[index],"%X",&bg); index+=3;
          sscanf(&cmd[index],"%X",&bb); index+=2;
          //Serial.print(F("DEBUG:");Serial.print(br,HEX);Serial.print(bg,HEX);Serial.print(bb,HEX);
          color=(uint32_t) ( 
                            (((long int)(br))<<16 ) | 
                            (((long int)(bg))<<8)   | 
                            ((long int)(bb))  );
          LSEM.setColor(color);
          break;
        case LS_TIMEOUT:
          index++;
          if ((len-(index+4)) <0) {Serial.println(F("DEBUG: incomplete ls_timeout")); goto exitingCmd;}
          LSEM.setTimeout((uint16_t)strtol(&cmd[index],kk,10));
          index+=4;
          break;
        case LS_PAUSE:
          index++;
          if ((len-(index+4)) <0) {Serial.println(F("DEBUG: incomplete ls_timeout")); goto exitingCmd;}
          LSEM.setPause((uint16_t)strtol(&cmd[index],kk,10));
          index+=4;
          break;
        case LS_ENQUEUE:
          index++;
          LSEM.enqueueCommands(&cmd[index]);
          index+=len;
          break;
        case LS_MODE:
          index++;
          m=cmd[index];
          bk=LSEM.getMode();
          LSEM.setMode(m);
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
                if ((len-(index+2)) <0) {Serial.println(F("DEBUG: incomplete ls_mode_one")); goto exitingCmd;}
                strncpy(aux,&cmd[index],2); index+=2;
                LSEM.setLed((uint8_t)strtol(aux,kk,10));
                index+=2;
                break;
              default: LSEM.setMode(bk); Serial.println(F("DEBUG: LS unexpected mode, ignoring it")); goto exitingCmd;
          }
          break;
        default: Serial.println(F("DEBUG:LS unexpected subtype")); goto exitingCmd;
      };
      break;
    default: Serial.println(F("DEBUG: PROTOCOL unexpected type")); goto exitingCmd;
  };
  Serial.println(F("DEBUG: Command processed successfully"));

exitingCmd:;
return index;
}

//-------------------------------------------------
void readSerialCommands(int pirInfo) {
  int index=0,readbytes=0;
  int len=0;
  char inputString[50];

  if (!inputBootStringDone){
    //inputString=(char*)inputBootString;//
    Serial.println(F("DEBUG: inputBootString..."));
    strcpy_P(inputString,(char*)inputBootString);
    inputBootStringDone=true;
  }
  else if (serialInputStringReady){
    //inputString=(char*)serialInputString;//
    strcpy(inputString,serialInputString);
  }
  else if ( (LSEM.getMode()==0) && (LSEM.areThereEnqueuedCommands())) {
    LSEM.dequeueCommands(inputString);
  }
  else if ( (LSEM.getMode()==0) && (pirInfo==PIR_INFO_LOW2HIGH) ) {
    Serial.println(F("DEBUG: inputPIRLOW2HIGHstring..."));
    strcpy_P(inputString,(char*)inputPIRLOW2HIGHstring);
  }
  else if ( (pirInfo==PIR_INFO_HIGH2LOW) ) {
    Serial.println(F("DEBUG: inputPIRHIGH2LOWstring..."));
    strcpy_P(inputString,(char*)inputPIRHIGH2LOWstring);
  }
  else return;

  
  //Serial.print(F("DEBUG:")); Serial.println(inputString);

  len=strlen(inputString);
  while ((len-index) >0)
  { 
    char *subcmd=0; 
    subcmd=strchr(inputString,':');
    if (subcmd==0) {
      Serial.println(F("DEBUG: no more comands in the line"));
      break;
    }
    index++; 
    readbytes=readSerialCommand(&inputString[index]);
    index+=readbytes;  
  }


  if (serialInputStringReady){
    serialInputString[0]=0;
    serialInputStringReady = false;
  }

}
//-------------------------------------------------
void loop() { 

  //------------- INPUT REFRESHING ----------------
  // Receive command && read PIR
  readSerialCommands(PIREM.refresh()); 
 
  // Read light sensor
  //TODO
  // ------------- OUTPUT REFRESHING ---------------

  // Write led strip
  LSEM.refresh();

  // Write motor 
  //TODO



}
