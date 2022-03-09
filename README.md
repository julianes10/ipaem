# ipaem  (ABANDONED PROJECT)
Just for fun custom personal assistance project. 
An excuse for explore IPA technologies and others stuff.

List of materials is a rapsberry pi 3 and a arduino nano (or one can be used to) plus a set of sensors and peripherls like dht11/22, light sensor, let stript, shock sensor and maybe others coming.


With this software is possible:
* Trigger voice commands using google hotword, others using snowboy or other input sensor
* Listen (apart from what amazing google assistant already provide):
  * radio stations (using tuneIn) 
  * music/audios (using youtube)
  * news (TTS from RSS feeds)
  * (in progress) local temperature from several sensors
  * (in progress) custom weather forecast details to specific hours or wind status. 
* Control led strip with an arduino communicated with your pi and some sensor
  * change lights mode and color of a arduino-controlled led strip
* start GA interaction using other inputs as standard "ok google".

So far, the idea is having config file to trigger actions besides default actions from GA.

In the config file will be possible run external commands, custom feedback sound to easy interact with the system. 

## Instalation
Follow requirements.txt instruction to install from a vanilla raspbian without desktop.

### Local instalation
Download or clone this repo in your target pi and execute:
<pre>
cd ipaem/install
sudo ./deploySofware.sh local
cd /opt/ipaem/install
sudo ./setupService.sh
</pre>

It will setup several services e.g ipaemg (tuned google assistant app) and ipaems (snowboy alternatives hot words)
Config file is in /etc/ipaem/ you can custome it

### Remote instalation or install from your development host
Remote instalation from your laptop is possible:
* Download repo in your host 
* Custom your pi connectiviy parameters in install/deploySoftware.sh file (PI_USER,IP or NAME and PIPORT). Ssh,scp wil be used for command and copying files.
* Run:
<pre>
cd ipaem/install
./deploySoftware.sh remote local [arduino] [config]. 
</pre>
If you include arduino, firmware will be updated too. If you include config, example config will be replace config files in /etc/ipaem/

Later only if never done before or you know that something has changed regards startup, ssh your pi manually and do:
<pre>
cd /opt/ipaem/install
sudo ./setupService.sh
</pre>


## Source code walkthrough

### Install 
Stuff for deploying the software and create a service

### gaIpadispacher
Python scripts based on google assistant sdk that process and triggger actions using ga and the config file. RESTapi available.
Youtube, tuniin and piInoAgent bash wrappers are there too.

### snowboyHotwords
Python scripts based on snowboy demo, it detects hotwords and RESTapi to communicate to previous service to start ga conversation or trigger directly actions.

### etc
Example of config file. It will be copy to /etc/ipaem when deploy the software

### piInoAgent
Python scritp with APIREST that control arduino using serial port

### arduino
Firmaware for arduino that integrate some sensors and a light strip led.


## Benchmark studies

### IPA
Quick analysis trying to see what is the state of the art at september 2017
Disclaimer: if something is incorrect bellow, just consider that the trial consist basically in 4-6 hours...

#### Google Assistant SDK

First thing i've tried is legacy v1 google speech API (following cool info from https://diyhacking.com/best-voice-recognition-software-for-raspberry-pi/). But it seems is deprecated. I've take a look to https://developers.google.com/ searching "voice" many info for android and pointers to  Google Cloud new services https://cloud.google.com/ with Cloud Speech API, but it seems to be pay services....  i've abandoned this way...

Then I come across with google assistant SDK https://developers.google.com/assistant/sdk/. It fit almost perfectly and the guide is for ubuntu and pi3, python part. It was great ¡¡¡ 

I followed the guide (https://developers.google.com/assistant/sdk/overview) easily, summing up:
- Got a script that run in target device that listen from your microphone waiting for 'ok google' as hot word
- Custom demo and, in a py scritp, receive a text with speech detected, and so that you can custom action in your device, for instance, open an app or act on a gpio in pi. FANTASTIC¡¡
- Together with IFTTT could install in the cloud a hook over a sentence and execute an smart action (send mails, interact with many apps in the internet) and back to your device with a custom speech as answer. GREAT¡¡. In addition, this kind of custom service could be used in other devices, not only your pi or your ubuntu, but your android phone reach this service too. 

Just a minor negative comment: unfortunalty, it seems not easy change hotword "hey google", in android there is a solution, but in sdk running in your pc or device not sure if exist.

#### JASPER
TODO  https://jasperproject.github.io/ no remote access, promising?.

#### Lucida

Good idea, but project is huge, maybe too much for my target. Apart of this it seems that there is no activity in recent months. 
https://github.com/claritylab/lucida  http://lucida.ai/

#### Conclusion
Lots of possibilities, some extra tuning for raspberry but still feasible.

### TTS - Text to speech
Not sure if GA will offer in the feature the ability to report a speech from a give text...
apparently TTS free service was removed some time ago.
It's true that in combination with IFTTT maybe a lot of things are possible, but what about having something simplier?
Festival, Flite, pico2wave and eSpeak seems to be the alternatives so far and all off them are straight forward of installing and test.
https://elinux.org/RPi_Text_to_Speech_(Speech_Synthesis)
After testing them (see utils/tts/test.sh) my selection is -in order-:
* for english is festival, picowave, flite and espeak.
* for spanish festival put xx instead of ñ, WTF¡. So picowave is the best to me.

### SDR 
One of the target is listen radio on Pi, interfacing of course using voice.
A bit rid of inestability of online radio, I considered tune FM radio using SDR device
Finally disregard this option (not investigated really), tunein pluggin on top of kodi works fine.

## Intersting links
* [Google products for developers](https://developers.google.com/)
* [Google Cloud sdk](https://cloud.google.com/sdk/docs/?hl=es)
* [Google assistant SDK](https://developers.google.com/assistant/sdk/)
* [JASPER](https://jasperproject.github.io/)
* [AIA project voice kit](https://aiyprojects.withgoogle.com/)
* [wikipedia](https://en.wikipedia.org/wiki/List_of_speech_recognition_software)
* [Shazam like](http://royvanrijn.com/blog/2010/06/creating-shazam-in-java/)
* [Customize hotword](https://snowboy.kitt.ai)
* [Python flask REST API tutorial](https://blog.miguelgrinberg.com/post/designing-a-restful-api-with-python-and-flask)
* [Audio setup in raspberry](http://www.instructables.com/id/Test-Sound-Card-and-Speakers-in-Raspberry-Pi/)
* [Free Sounds to custom your project](https://www.soundjay.com/index.html)
* [Free Sounds to custom your project](https://www.soundjay.com/index.html)
* [Amixer commands](https://www.soundjay.com/index.html)http://blog.scphillips.com/posts/2013/01/sound-configuration-on-raspberry-pi-with-alsa/)
* [TTS analisys from adafruit](https://learn.adafruit.com/speech-synthesis-on-the-raspberry-pi/introduction)
* [TTS analisys from elinux.org](https://elinux.org/RPi_Text_to_Speech_(Speech_Synthesis))


## Hardware details
TODO TODO TODO
dht11: pi GPI04=PIN7
IR1: GPI17=PIN11
IR2: GPI27=PIN13

## TODO LIST
* Document better
* Describe how to tune sounds
* Dockerizate solution
* ... lots of improvement for fun¡




