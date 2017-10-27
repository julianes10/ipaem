#!/bin/bash
PYTHONPATH=/home/pi/snowboy python /opt/ipaem/snowboyHotwords/snowboyHotwords.py /opt/ipaem/snowboyHotwords/churri.pmdl   /opt/ipaem/snowboyHotwords/chencho.pmdl &
source /home/eespjul/prj/miniIPA/gasdk/env/bin/activate
source /home/pi/env/bin/activate

python /opt/ipaem/gaIpaDispatcher/gaIpaDispatcher.py 
