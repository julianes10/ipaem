# Copyright 2017 Google Inc.
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

"""Carry out voice commands by recognising keywords."""

import datetime
import logging
import subprocess



import json 
import pprint
import re
#import RPi.GPIO as GPIO
import time
import urllib
import vlc
import youtube_dl


class TuneInRadio(object):

    """Plays a radio stream from TuneIn radio"""
    
    BASE_URL = 'http://tunein.com/'
    FILTER_STATIONS = 'Stations'
    
    def __init__(self):
        print = "say"
        self.keyword = "keyword"
        self._init_player()
        self._init_gpio(23)
        
    def run(self, voice_command):
        
        search_str = voice_command.lower().replace(self.keyword, '', 1).strip()
     
        if not search_str:
            print('Please specify a station')
            return
     
        """stations = self._search(search_str)
        if not stations:
            print("Didn't find any stations")
            #return
 
        station = stations[0]
        """
        url = self._get_stream_url(50646)
        if not url:
            print("Didn't find any streams")
            #return
       
        url="https://tunein.com/radio/BBC-World-Service-UK-s50646"
        print(url)
        media = self.instance.media_new(url)
        self.player.set_media(media)
        
        self.player.play()

        self.done = False
        while not self.done:
            time.sleep(1)
            
    def _init_gpio(self,k):
        """, channel, polarity=GPIO.FALLING, pull_up_down=GPIO.PUD_UP):
        self.input_value = polarity == GPIO.RISING
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(channel, GPIO.IN, pull_up_down=pull_up_down)
        try:
            GPIO.add_event_detect(channel, polarity, callback=self._on_input_event)
        except RuntimeError:
            logging.info('Event already added')
            GPIO.add_event_callback(channel, self._on_input_event)
        """
    
    def _init_player(self):
        self.now_playing = None
        self.done = False
        self.instance = vlc.get_default_instance()
        self.player = self.instance.media_player_new()
        events = self.player.event_manager()
        events.event_attach(vlc.EventType.MediaPlayerEndReached, self._on_player_event)
        events.event_attach(vlc.EventType.MediaPlayerEncounteredError, self._on_player_event)
    
    def _on_input_event(self, channel):
        if GPIO.input(channel) == self.input_value:
            self.player.stop()
            self.done = True
            
    def _on_player_event(self, event):
        if event.type == vlc.EventType.MediaPlayerEndReached:
            self.done = True
        elif event.type == vlc.EventType.MediaPlayerEncounteredError:
            print("Can't play " + self.now_playing)
            self.done = True

    def _search(self, search_str, search_filter=FILTER_STATIONS):
    
        ret_results= None
        
        url = TuneInRadio.BASE_URL + 'search/?query=' + urllib.parse.quote(search_str)
        print(url)
        req = urllib.request.Request(url)
        fp = urllib.request.urlopen(req)
        xml_str = fp.read().decode('ascii', 'ignore')
        fp.close()
        
        pattern = r'TuneIn.payload = (\{.*\})'
        print(pattern)
        print(xml_str)
        result = re.search(pattern, xml_str)
        print(result)        
        if not result:
            return None
        payload = result.group(1)
        result = json.loads(payload)
        print(result)        
        categories = result['ContainerGuideItems']['containers']
        for category in categories:
            if category['Title'] == search_filter:
                ret_results = category['GuideItems']
                break;
        
        return ret_results
                
    def _get_stream_url(self, station_id):
    
        url = TuneInRadio.BASE_URL + 'station/?stationId=' + str(station_id)
        print(url)
        req = urllib.request.Request(url)
        fp = urllib.request.urlopen(req)
        xml_str = fp.read().decode('ascii', 'ignore')
        fp.close()
        
        #pattern = r'"StreamUrl":"(.*?)"'
        pattern = r'"url":"(.*?)"'

        result = re.search(pattern, xml_str)
        print(result)
        if not result.group(1):
            return None
        
        json_url = 'http:' + result.group(1)
        streams = self._get_stream_list(json_url)
        
        if not streams:
            return None
            
        stream = streams[0]
        return stream['Url']
        
    def _get_stream_list(self, url):
        
        print(url)
        req = urllib.request.Request(url)
        fp = urllib.request.urlopen(req)
        json_str = fp.read().decode('ascii', 'ignore')
        fp.close()
        
        result = json.loads(json_str)
        return result['Streams']



        

