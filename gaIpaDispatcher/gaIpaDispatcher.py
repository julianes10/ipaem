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
import threading



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

from flask import Flask, jsonify,abort,make_response,request, url_for



'''----------------------------------------------------------'''
'''----------------      API REST         -------------------'''
'''----------------------------------------------------------'''
api = Flask("api")


@api.route('/ipaem/api/v1.0/hotword', methods=['GET'])
def get_hotword():
    rt=jsonify({'result': 'KO'})
    if ga.getBusy()==False:
      helper.internalLogger.debug("Alternative hotword")
      assistant.start_conversation()
      rt=jsonify({'result': 'OK'})
    else:
      helper.internalLogger.debug("Alternative hotword ignored, conversation already ongoing")
    return rt


@api.route('/ipaem/api/v1.0/localaction', methods=['POST'])
def post_localaction():
    rt=jsonify({'result': 'TODO'})
    if not request.json or not 'input' in request.json:
        abort(400)
    #TBD 
    return rt, 201

'''----------------------------------------------------------'''
'''----------------      GA control object  -----------------'''

class Ga(object):
    def __init__(self):
        self.busy = True
    def setBusy(self,value):
        self.busy = value
    def getBusy(self):
        return self.busy
   
'''----------------------------------------------------------'''
'''----------------    runAction            -----------------'''

def runAction(action):
    helper.internalLogger.debug("Running '{0}'".format(action))
    if "background" in action:
      if action["background"]:
        subprocess.Popen(['bash','-c',action["cmd"]])
        return action["next"]
    
    #else blocking
    subprocess.call(['bash','-c',action["cmd"]])
    

    return action["next"]

'''----------------------------------------------------------'''
'''----------------    checkAnswer          -----------------'''

def checkAnswer(actions,text):
    helper.internalLogger.debug("Checking '{0}' on local actions {1}...".format(text,actions))
    if text is None:
      return None
    if text=="":
      return None
   
    for key,item in actions.items():
      helper.internalLogger.debug("Trying to match {0} on action {1}".format(text,key))
      if text.lower() in item['input'] or item['input'] in text.lower():
         helper.internalLogger.debug("Action found over '{0}': {1}".format(text,key))         
         return runAction(item['output'])
  
    helper.internalLogger.debug("No action found over '{0}'".format(text))
    return None

'''----------------------------------------------------------'''
'''----------------    process_event        -----------------'''

def process_event(cfg_Actions,event,ga):

    helper.internalLogger.debug("Processing event:{0}...".format(event))

    if event.type == EventType.ON_CONVERSATION_TURN_STARTED:
        ga.setBusy(True)
        helper.internalLogger.debug("Please say something")
        subprocess.call(['aplay','--format=S16_LE','--rate=21000','/home/pi/audios/ack.raw'])       
    elif event.type == EventType.ON_RECOGNIZING_SPEECH_FINISHED:
        helper.internalLogger.debug("Let's process what google say you have said")
        return checkAnswer(cfg_Actions,event.args['text'])
    elif event.type == EventType.ON_START_FINISHED:
        ga.setBusy(False)
    elif event.type == EventType.ON_CONVERSATION_TURN_FINISHED:
        ga.setBusy(False)
    elif event.type == EventType.ON_ASSISTANT_ERROR and event.args and event.args['is_fatal']:
        helper.internalLogger.critical("Error, exiting")
        sys.exit(1)


    '''    if (event.type == EventType.ON_CONVERSATION_TURN_FINISHED and
            event.args and not event.args['with_follow_on_turn']):
        helper.internalLogger.debug("ON_CONVERSATION_TURN_FINISHED with_follow_on_turn")
    ''' 


    return 0

'''----------------------------------------------------------'''
'''----------------       M A I N         -------------------'''
'''----------------------------------------------------------'''

def main(configfile,cred):
  print('IPAEM-start -----------------------------')   

  global ga
  global assistant
  ga=Ga()

  # Loading config file,
  # Default values
  cfg_log_traces="ipaem.log"
  cfg_log_exceptions="ipaeme.log"
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
  helper.internalLogger.debug('See logs traces in: {0} and exeptions in: {1}-----------'.format(cfg_log_traces,cfg_log_exceptions))  
  helper.internalLogger.critical('IPAEM-start -------------------------------')  
  helper.einternalLogger.critical('IPAEM-start -------------------------------')
  try:
      cfg_Actions = configuration["LocalActions"]
      helper.internalLogger.debug("Local actions {0}...".format(cfg_Actions))
  except Exception as e:
      helper.internalLogger.critical("Error processing configuration json {0} file. No action".format(configfile))
      helper.einternalLogger.exception(e)
      loggingEnd()
      return 0


  try:
    with open(cred, 'r') as f:
         credentials = google.oauth2.credentials.Credentials(token=None,
                                                            **json.load(f))      
    with Assistant(credentials) as assistant:
      ### Now let's run in background alternative ways to 'ok google' speech
      apiRestTask=threading.Thread(target=apirest_task,args=(assistant,ga,),name="theOtherTrigger")
      apiRestTask.daemon = True
      apiRestTask.start()
      for event in assistant.start():
            result=process_event(cfg_Actions,event,ga)
            if result == "end":
                helper.internalLogger.debug('Stop conversation')
                assistant.stop_conversation()
            elif result == "chat":
                helper.internalLogger.debug('Start conversation')
                assistant.start_conversation()
            else: # none or other
                helper.internalLogger.debug('Waiting next event')


  except Exception as e:
    e = sys.exc_info()[0]
    helper.internalLogger.critical('Error: Exception unprocessed properly. Exiting')
    helper.einternalLogger.exception(e)  
    helper.internalLogger.debug('IPAEM-General exeception captured. See ssms.log:{0}',format(cfg_log_exceptions))        
    loggingEnd()


'''----------------------------------------------------------'''
'''----------------     apirest_task      -------------------'''
def apirest_task(assistant,ga):
  api.run(debug=True, use_reloader=False)


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
                            os.path.expanduser('/etc/ipaem/'),
                            'google-oauthlib-tool',
                            'credentials.json'
                        ),
                        help='Path to store and read OAuth2 credentials')
    return parser.parse_args()

'''----------------------------------------------------------'''
'''----------------    helper.internalLogger.debugPlatformInfo  -------------------'''
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
