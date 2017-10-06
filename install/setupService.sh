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
  cp -rf etc/ipaem.conf.example /etc/ipaem/ipaem.conf
else
  echo "Keeping existing configuration file in /etc/ipaem/. Only updating example one"
  cp -rf etc/ipaem.conf.example /etc/ipaem/
fi 
cp -rf ipaem.service /lib/systemd/system/ipaem.service
chmod 644 /lib/systemd/system/ipaem.service
systemctl daemon-reload
systemctl enable ipaem.service
systemctl start ipaem.service
systemctl status ipaem.service

echo "Remember cheatsheet:"
echo "  Check status: sudo systemctl status ipaem.service"
echo "  Start service: sudo systemctl start ipaem.service"
echo "  Stop service: sudo systemctl stop ipaem.service"
echo "  Check service's log: sudo journalctl -f -u ipaem.service"

