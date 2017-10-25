#!/bin/bash
python /opt/ipaem/snowboyHotwords/snowboyHotwords.py 
source /home/eespjul/prj/miniIPA/gasdk/env/bin/activate
source /home/pi/env/bin/activate

python /opt/ipaem/gaIpaDispatcher/gaIpaDispatcher.py 
