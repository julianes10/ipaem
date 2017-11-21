#!/bin/bash 
# Thanks to easy to follow guide http://www.diegoacuna.me/how-to-run-a-script-as-a-service-in-raspberry-pi-raspbian-jessie/
source ./commonvars.sh

echo "This script will install in your pi a service ipaem from $DEPLOY_FOLDER usign systemctl utility. No arguments required. Run it being root or with sudo"
if [ "$1" == "-h" -o "$1" == "--help" ]; then
  exit 0
fi
  
if [ ! -f /etc/ipaem/ipaem.conf ]; then
  echo "Taking config file example as configuration file in /etc/ipaem/, please review it and restart the service"
  mkdir -p /etc/ipaem
  cp -rf ../etc/ipaem.conf.example /etc/ipaem/ipaem.conf
else
  echo "Keeping existing configuration file in /etc/ipaem/. In opt theres is a modern example one"
fi

cp -rf ../etc/HIDDEN.asoundrc /root/.asoundrc
cp -rf ../etc/HIDDEN.asoundrc /home/pi/.asoundrc

 
cp -rf ipaemg.service /lib/systemd/system/ipaemg.service
cp -rf ipaems.service /lib/systemd/system/ipaems.service
cp -rf kodi.service /lib/systemd/system/kodi.service
cp -rf noip2.service /lib/systemd/system/noip2.service
chmod 644 /lib/systemd/system/ipaemg.service
chmod 644 /lib/systemd/system/ipaems.service
chmod 644 /lib/systemd/system/kodi.service
chmod 644 /lib/systemd/system/noip2.service


systemctl daemon-reload
systemctl enable ipaemg.service
systemctl enable ipaems.service
systemctl enable kodi.service
systemctl enable noip2.service

systemctl start ipaemg.service
systemctl start ipaems.service
systemctl start kodi.service
systemctl start noip2.service

systemctl status ipaemg.service
systemctl status ipaems.service
systemctl status kodi.service
systemctl status noip2.service

echo "Remember cheatsheet:"
echo "  Check status: sudo systemctl status ipaemg.service"
echo "  Start service: sudo systemctl start ipaemg.service"
echo "  Stop service: sudo systemctl stop ipaemg.service"
echo "  Check service's log: sudo journalctl -f -u ipaemg.service"

