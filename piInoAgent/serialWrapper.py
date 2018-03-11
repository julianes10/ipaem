#!/usr/bin/env python
import sys
import json
import subprocess
import os
import time
import serial
import helper
from colour import Color

'''----------------------------------------------------------'''
'''---------------- class led strip wrapper------------'''

class serialWrapper():

    def __init__(self,port,speed,timeout):
      self.port=port
      self.speed=speed
      self.timeout=timeout
      self.ser=None
      self.buffer=""

    def tryReconnect(self):
      self.ser=None
      try:
        helper.internalLogger.info("Try to reconnect to port: {0}".format(self.port))
        self.ser = serial.Serial(
          port=self.port,\
          baudrate=self.speed,\
          parity=serial.PARITY_NONE,\
          stopbits=serial.STOPBITS_ONE,\
          bytesize=serial.EIGHTBITS,\
          timeout=self.timeout)
      except KeyboardInterrupt:
        print("Ok ok, quitting")
        sys.exit(1)
      except Exception as e:
        e = sys.exc_info()[0]
        helper.internalLogger.error('Unexpected error accesing to serial port. It will be retried later.')
        helper.einternalLogger.exception(e)  
        self.ser=None
        return False
      return True

    def readCommand(self):
      helper.internalLogger.debug("Getting info from arduino {0}...".format(self.port))
      #Check ser status
      if self.ser==None:
        if self.tryReconnect():
           helper.internalLogger.error('Serial line ready to be read.')
        else:
           helper.internalLogger.error('Serial line is not seem to be ready.') 
           time.sleep(10)
           return None;
          
      line=None     
      data=None
      i=0
      while True:
        try:
          line = self.ser.readline()
        except KeyboardInterrupt:
          print("Ok ok, quitting")
          sys.exit(1)
        except Exception as e:
          e = sys.exc_info()[0]
          helper.internalLogger.error('Unexpected error reading line from remote serial bt device.')
          helper.einternalLogger.exception(e)
          self.ser=None
          return data
        #helper.internalLogger.debug("Got line, cool") 
        line=line.strip('\n\r ')                 
        if line is not None and line.strip is not "":
            try:
              helper.internalLogger.debug("RX>:" + line)
              #TODO process the line
            except Exception as e:
              helper.internalLogger.debug("Error processing line. Ignoring it")
              e = sys.exc_info()[0]
              helper.einternalLogger.exception(e)  
        else:
            helper.internalLogger.debug("No data available on LD yet...")

    def writeCommand(self,cmd):
      helper.internalLogger.debug("Sending command to arduino {0}:{1}...".format(self.port,cmd))
      #Check ser status
      if self.ser==None:
        if self.tryReconnect():
          helper.internalLogger.error('Serial line ready to be read.')            
        else:
          helper.internalLogger.error('Serial line is not seem to be read.') 
          time.sleep(10)
          return None;
      self.ser.write(cmd+"\n")

    def flush(self):
      self.writeCommand(self.buffer)
      self.buffer=""
          
    def sendMode(self,mode):
      helper.internalLogger.debug("Setup Mode: {0}".format(mode)) 
      self.buffer+=":LM"+str(mode)

    def sendColor(self,color):
      c=Color(color)
      aux=c.hex_l
      helper.internalLogger.debug("Setup Color: {0}".format(aux)) 
      self.buffer+=":LC"+aux[1:3]+","+aux[3:5]+","+aux[5:]


    def sendTimeout(self,t):
      helper.internalLogger.debug("Setup Timeout: {0}".format(t)) 
      self.buffer+=":LT"+str(round(t/10)).zfill(4)

    def sendPause(self,t):
      helper.internalLogger.debug("Setup Pause: {0}".format(t)) 
      self.buffer+=":LP"+str(t).zfill(4)

    def sendReset(self):
      helper.internalLogger.debug("Setup Reset") 
      self.buffer+=":LX"


    def sendDebug(self,flag):
      helper.internalLogger.debug("Setup Debug {0}".format(flag))
      self.buffer+=":L"
      if flag: 
        self.buffer+="D"
      else:
        self.buffer+="d"

    def sendStatus(self):
      helper.internalLogger.debug("Ask status") 
      self.buffer+=":LS"



