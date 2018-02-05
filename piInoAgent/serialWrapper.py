#!/usr/bin/env python
import sys
import json
import subprocess
import os
import time
import serial
import helper
'''----------------------------------------------------------'''
'''---------------- class led strip wrapper------------'''

class serialWrapper():

    def __init__(self,port,speed,timeout):
      self.port=port
      self.speed=speed
      self.timeout=timeout
      self.ser=None

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

    def getData(self):
      helper.internalLogger.debug("Getting data led strip {0}...".format(self.port))
      #Check ser status
      if self.ser==None:
        if self.tryReconnect():
           helper.internalLogger.error('Serial line ready to be read.')
        else:
           helper.internalLogger.error('Serial line is not seem to be read.') 
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
        helper.internalLogger.debug("Got line, cool")                  
        if line is not None: 
            try:
              helper.internalLogger.debug("Something read yeah...:" + line)
            except Exception as e:
              helper.internalLogger.debug("Error processing line. Ignoring it")
              e = sys.exc_info()[0]
              helper.einternalLogger.exception(e)  
        else:
            helper.internalLogger.debug("No data available on LD yet...")

    def sendComand(self):
      helper.internalLogger.debug("Sending command to led strip {0}...".format(self.port))
      #Check ser status
      if self.ser==None:
        if self.tryReconnect():
          helper.internalLogger.error('Serial line ready to be read.')            
        else:
          helper.internalLogger.error('Serial line is not seem to be read.') 
          time.sleep(10)
          return None;
      self.ser.writeline("TEST TODO")
          
 

