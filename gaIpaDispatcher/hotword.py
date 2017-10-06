#!/usr/bin/env python

# Copyright (C) 2017 Google Inc. --- included as reference, some of the code comes from samples ga sdk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Custom adaptations

from __future__ import print_function

import argparse
import os.path
import json


import google.oauth2.credentials

from google.assistant.library import Assistant
from google.assistant.library.event import EventType
from google.assistant.library.file_helpers import existing_file

import argparse
import time
import sys
import json
import subprocess
import os
import platform
import helper

from helper import *

configuration={}


def process_event(event):
    """Pretty prints events.

    Prints all events that occur with two spaces between each new
    conversation and a single space between turns of a conversation.

    Args:
        event(event.Event): The current event to process.
    """
    if event.type == EventType.ON_CONVERSATION_TURN_STARTED:
        print()
        import subprocess
        subprocess.call(['aplay','--format=S16_LE','--rate=21000','/home/pi/audios/ack.raw'])
       

    print(event)

    if event.type == EventType.ON_RECOGNIZING_SPEECH_FINISHED:
        if event.args['text'] == "start":
            print("------------------------- got it star¡¡¡¡")
            return 1
        if event.args['text'] == "1":
            print("------------------------- got it 1¡¡¡¡")
            return 1
        if event.args['text'] == "2":
            print("------------------------- got it 2¡¡¡¡")
            return 1
        if event.args['text'] == "3":
            print("------------------------- got it 3¡¡¡¡")
            return 2       
        elif event.args['text'] == "stop":
            print("----------------- got it radio¡¡¡¡")
            return 2

    if (event.type == EventType.ON_CONVERSATION_TURN_FINISHED and
            event.args and not event.args['with_follow_on_turn']):
        print()
    
    return 0



'''----------------------------------------------------------'''
'''----------------       M A I N         -------------------'''
'''----------------------------------------------------------'''

def main(configfile,cred):
  print('IPAEM-start -----------------------------')   

  # Loading config file,
  # Default values
  cfg_log_traces="ipaem.log"
  cfg_log_exceptions="ipaeme.log"
  cfg_Actions={}
  # Let's fetch data
  with open(configfile) as json_data:
      configuration = json.load(json_data)
  #Get log names
  if "log" in configuration:
      if "logTraces" in configuration["log"]:
        cfg_log_traces = configuration["log"]["logTraces"]
      if "logExceptions" in configuration["log"]:
        cfg_log_exceptions = configuration["log"]["logExceptions"]
  helper.init(cfg_log_traces,cfg_log_exceptions)
  print('See logs traces in: {0} and exeptions in: {1}-----------'.format(cfg_log_traces,cfg_log_exceptions))  
  helper.internalLogger.critical('IPAEM-start -------------------------------')  
  helper.einternalLogger.critical('IPAEM-start -------------------------------')
  try:
    #Get sensors list
    cfg_Actions = configuration["LocalActions"]

  except Exception as e:
    helper.internalLogger.critical("Error processing configuration json {0} file. Exiting".format(configfile))
    helper.einternalLogger.exception(e)
    loggingEnd()
    return  

  try:
    with open(cred, 'r') as f:
         credentials = google.oauth2.credentials.Credentials(token=None,
                                                            **json.load(f))      
    with Assistant(credentials) as assistant:
        for event in assistant.start():
            result=process_event(event)
            if result==1:
                print('Start conversation')
                assistant.start_conversation()
            elif result==2:
                assistant.stop_conversation()
            else: # 0 or other
                print('Waiting next event')


  except Exception as e:
    e = sys.exc_info()[0]
    helper.internalLogger.critical('Error: Exception unprocessed properly. Exiting')
    helper.einternalLogger.exception(e)  
    print('IPAEM-General exeception captured. See ssms.log:{0}',format(cfg_log_exceptions))        
    loggingEnd()


'''----------------------------------------------------------'''
'''----------------       loggingEnd      -------------------'''
def loggingEnd():      
  helper.internalLogger.critical('IPAEM-end -----------------------------')        
  print('IPAEM-end -----------------------------')

   
'''----------------------------------------------------------'''
'''----------------     parse_args        -------------------'''
def parse_args():
    """Parse the args."""
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter)
    parser = argparse.ArgumentParser(
        description='Intelligent Personal Assistant EM')
    parser.add_argument('--configfile', type=str, required=False,
                        default='/etc/ipaem/ipaem.conf',
                        help='Config file for ipaem service')
    parser.add_argument('--credentials', type=existing_file,
                        metavar='OAUTH2_CREDENTIALS_FILE',
                        default=os.path.join(
                            os.path.expanduser('/home/pi/.config'),
                            'google-oauthlib-tool',
                            'credentials.json'
                        ),
                        help='Path to store and read OAuth2 credentials')
    return parser.parse_args()

'''----------------------------------------------------------'''
'''----------------    printPlatformInfo  -------------------'''
def printPlatformInfo():
    print("Running on OS '{0}' release '{1}' platform '{2}'.".format(os.name,platform.system(),platform.release()))
    print("Uname raw info: {0}".format(os.uname()))
    print("Arquitecture: {0}".format(os.uname()[4]))
    print("Python version: {0}".format(sys.version_info))

'''----------------------------------------------------------'''
'''----------------       '__main__'      -------------------'''
if __name__ == '__main__':
    printPlatformInfo()
    args = parse_args()
    main(configfile=args.configfile,cred=args.credentials)    
