# ipaem
Small project to manage home devices exploring IPA technologies

## Benchmark study
Quick analysis trying to see what is the state of the art at september 2017
Disclaimer: if something is incorrect bellow, just consider that the trial consist basically in 4-6 hours...

### Google Assistant SDK

First thing i've tried is legacy v1 google speech API (following cool info from https://diyhacking.com/best-voice-recognition-software-for-raspberry-pi/). But it seems is deprecated. I've take a look to https://developers.google.com/ searching "voice" many info for android and pointers to  Google Cloud new services https://cloud.google.com/ with Cloud Speech API, but it seems to be pay services....  i've abandoned this way...

Then I come across with google assistant SDK https://developers.google.com/assistant/sdk/. It fit almost perfectly and the guide is for ubuntu and pi3, python part. It was great ¡¡¡ 

I followed the guide (https://developers.google.com/assistant/sdk/overview) easily, summing up:
- Got a script that run in target device that listen from your microphone waiting for 'ok google' as hot word
- Custom demo and, in a py scritp, receive a text with speech detected, and so that you can custom action in your device, for instance, open an app or act on a gpio in pi. FANTASTIC¡¡
- Together with IFTTT could install in the cloud a hook over a sentence and execute an smart action (send mails, interact with many apps in the internet) and back to your device with a custom speech as answer. GREAT¡¡. In addition, this kind of custom service could be used in other devices, not only your pi or your ubuntu, but your android phone reach this service too. 

Just a minor negative comment: unfortunalty, it seems not easy change hotword "hey google", in android there is a solution, but in sdk running in your pc or device not sure if exist.

### JASPER
TODO  https://jasperproject.github.io/ no remote access, promising?.

### Lucida

Good idea, but project is huge, maybe too much for my target. Apart of this it seems that there is no activity in recent months. 
https://github.com/claritylab/lucida  http://lucida.ai/

### Conclusion
TODO

## The project
So far, the idea is having config file to trigger actions besides default actions from GA.
In the config file will be possible run external commands, give feedback sound, etc
### Install dir
Stuff for deploying the software and create a service
### gaIpadispacher
Python scripts based on google assistant sdk that process and triggger actions using ga and the config file
### etc
Example of config file. It will be copy to /etc/ipaem when deploy the software

## Intersting links
-[Google products for developers][https://developers.google.com/]
-[Google Cloud sdk][https://cloud.google.com/sdk/docs/?hl=es]
-[Google assistant SDK][https://developers.google.com/assistant/sdk/]
-[JASPER][https://jasperproject.github.io/]
-[AIA project voice kit][https://aiyprojects.withgoogle.com/]
-[wikipedia][https://en.wikipedia.org/wiki/List_of_speech_recognition_software]
-[Shazam like][http://royvanrijn.com/blog/2010/06/creating-shazam-in-java/]

## TODO
Process config file
Upload sounds
Try trigger youtoube
Try trigger radio online
dockerizate it
Try to change hotword





