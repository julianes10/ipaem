#!/usr/bin/env python
import sys
import signal
import subprocess
import sched, time
from threading import Timer
import RPi.GPIO as GPIO


interrupted = False

def hotword_handler(arg1):
    print('-handler')   
    p=subprocess.Popen(['bash','-c',"curl -i http://localhost:5000/ipaem/api/v1.0/hotword"])
    p.wait()

def signal_handler(signal, frame):
    global interrupted
    interrupted = True
    print('-end -----------------------------')   
    exit(-1)


def interrupt_callback():
    global interrupted
    return interrupted

# capture SIGINT signal, e.g., Ctrl+C
signal.signal(signal.SIGINT, signal_handler)


'''----------------------------------------------------------'''
'''----------------       M A I N         -------------------'''
'''----------------------------------------------------------'''

def main():
  print('-start -----------------------------')   
  GPIO.setwarnings(False)
  GPIO.setmode(GPIO.BOARD)
  GPIO.setup(11, GPIO.IN)         #Read output from shock sensor
  GPIO.add_event_detect(11, GPIO.FALLING)

  cb = lambda arg1=1: hotword_handler(arg1)
  GPIO.add_event_callback(11, cb)


  while True:
    time.sleep(0.1)

'''----------------------------------------------------------'''
'''----------------       '__main__'      -------------------'''
if __name__ == '__main__':
    main()
