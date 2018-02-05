#!/usr/bin/env python
import argparse
import time
import sys
import json
import subprocess
import os
import platform
import threading
import helper

from helper import *
from serialWrapper import *

from flask import Flask, jsonify,abort,make_response,request, url_for

configuration={}


'''----------------------------------------------------------'''
'''----------------      API REST         -------------------'''
'''----------------------------------------------------------'''
api = Flask("api")


@api.route('/ipaem/api/v1.0/lstest', methods=['GET'])
def get_hotword():
    rt=jsonify({'result': 'KO'})
    ls.putData()
    if True:
      helper.internalLogger.debug("lstest is requested")
      rt=jsonify({'result': 'OK'})
    else:
      helper.internalLogger.debug("lstest ignored")
    return rt


@api.route('/ipaem/api/v1.0/color', methods=['POST'])
def post_localaction():
    rt=jsonify({'result': 'TODO'})
    if not request.json or not 'color' in request.json:
        abort(400)
    #TBD 
    return rt, 201



'''----------------------------------------------------------'''
'''----------------       M A I N         -------------------'''
'''----------------------------------------------------------'''

def main(configfile):
  print('PIINOAGENT-start -----------------------------')
  

  # Loading config file,
  # Default values
  cfg_log_traces="piinoagent.log"
  cfg_log_exceptions="piinoagente.log"
  cfg_SensorsDirectory={}
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
  helper.internalLogger.critical('PIINOAGENT-start -------------------------------')  
  helper.einternalLogger.critical('PIINOAGENT-start -------------------------------')
  try:
    pass
  except Exception as e:
    helper.internalLogger.critical("Error processing configuration json {0} file. Exiting".format(configfile))
    helper.einternalLogger.exception(e)
    loggingEnd()
    return  

  try:    

    apiRestTask=threading.Thread(target=apirest_task,name="restapi")
    apiRestTask.daemon = True
    apiRestTask.start()
   
    global ls
    ls=serialWrapper( configuration["ledStrip"]["port"],
                      configuration["ledStrip"]["speed"],
                      configuration["ledStrip"]["timeout"]);
   
    while True:
        ls.tryReconnect()
        time.sleep(10) 

  except Exception as e:
    e = sys.exc_info()[0]
    helper.internalLogger.critical('Error: Exception unprocessed properly. Exiting')
    helper.einternalLogger.exception(e)  
    print('PIINOAGENT-General exeception captured. See ssms.log:{0}',format(cfg_log_exceptions))        
    loggingEnd()


'''----------------------------------------------------------'''
'''----------------       loggingEnd      -------------------'''
def loggingEnd():      
  helper.internalLogger.critical('PIINOAGENT-end -----------------------------')        
  print('PIINOAGENT-end -----------------------------')


'''----------------------------------------------------------'''
'''----------------     apirest_task      -------------------'''
def apirest_task():
  api.run(debug=True, use_reloader=False,port=5001)

'''----------------------------------------------------------'''
'''----------------     parse_args        -------------------'''
def parse_args():
    """Parse the args."""
    parser = argparse.ArgumentParser(
        description='Arduino handler agend for ipaem')
    parser.add_argument('--configfile', type=str, required=False,
                        default='/etc/ipaem/piinoagent.conf',
                        help='Config file for piinoagent service')
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
    main(configfile=args.configfile)
