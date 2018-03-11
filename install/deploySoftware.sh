#!/bin/bash 
# Deploy and setup release into pi system
source ./commonvars.sh
PI_USER=pi
PI_IPNAME=pi32
PI_PORT=22

IPAEM_SERVICES="ipaemg ipaems ipaemp ipaemb kodi noip2"


echo "Here we go: $@"
usage(){
	echo "Usage: $0 (latest | tag <HASH> | local | remote  (latest | tag <HASH> | local) [arduino][config])"
  echo "NOTE: ONLY remote local arduino config is fully supported."
	exit 1
}
rt=55

echo "This script will deploy software in your system to $DEPLOY_FOLDER to be run later usign systemctl utility.  Run it being root or with sudo"
if [ "$1" == "-h" -o "$1" == "--help" ]; then
  usage
  exit 0
fi

#---------------------------------------------------------------------------
deployConfig=0
deployArduino=0
for i in "$@"
do
case $i in
    config)
      deployConfig=1
    ;;
    arduino)
      deployArduino=1
    ;;
esac
done

#-----------------------------------------------------------------



aux="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "Check that your current directory '$aux' is not deploy one $DEPLOY_FOLDER..."
if [ "$aux" == "$DEPLOY_FOLDER" ]; then
  echo "ERROR: you must not deploy from deployed directory. Exiting"
  exit
fi

echo "Check the options and deploy folder '$DEPLOY_FOLDER' status..."


if [ "$1" != "remote" ]; then
  rm -rf $DEPLOY_FOLDER
fi 

if [ "$1" == "latest" ]; then
  echo "Clonning latest..."
  git clone git@github.com:julianes10/ipaem.git $DEPLOY_FOLDER
  rt=$?
elif [ "$1" == "tag" ]; then
  echo "Clonning latest..."
  git clone git@github.com:julianes10/ipaem.git $DEPLOY_FOLDER
  echo "Moving to tag $2..."
  pushd $DEPLOY_FOLDER
  if [ $? -eq 0 ];  then
    git reset --hard $2
    rt=$?
  else
    echo "Error... exiting"
    exit
  fi 
  popd
elif [ "$1" = "local" ]; then
  echo "WARNING this will use local version. Deployed version will be removed"
  echo "TODO uninstall legacy service, bbdd... "
  rm -rf $DEPLOY_FOLDER
  if [ $? -eq 0 ]; then
    echo "Copying local version to $DEPLOY_FOLDER... "
    cp -rf ../ $DEPLOY_FOLDER
    rt=$?
  else
    echo "Error deleting current deploy dir, exiting..."
  fi 
elif [ "$1" == "remote" ]; then
  if [ "$2" == "latest" ] || [ "$2" == "tag" ]; then
    echo "TODO"
  elif [ "$2" == "local" ]; then
    echo "Copying local files"
    ssh -p $PI_PORT pi@$PI_IPNAME "sudo rm -rf /home/pi/ipaem.tmp; mkdir -p /home/pi/ipaem.tmp"
    export GLOBIGNORE="./../../ipaem/.git:./../../ipaem/arduino/build-uno";
    scp -r -P $PI_PORT ./../../ipaem/* $PI_USER@$PI_IPNAME:/home/pi/ipaem.tmp/
    DEPLOY_CONFIG=""
    if [ $deployConfig -eq 1 ]; then
       DEPLOY_CONFIG="sudo cp /opt/ipaem/etc/ipaem.conf.example /etc/ipaem/ipaem.conf;"
    fi
    DEPLOY_ARDUINO=""
    if [ $deployArduino -eq 1 ]; then
       DEPLOY_ARDUINO="cd /opt/ipaem/install && sudo ./deployArduino.sh;"
    fi
    ssh -p $PI_PORT pi@$PI_IPNAME "sudo systemctl stop $IPAEM_SERVICES;sudo rm -rf $DEPLOY_FOLDER; sudo mv /home/pi/ipaem.tmp $DEPLOY_FOLDER;$DEPLOY_CONFIG sudo systemctl start $IPAEM_SERVICES;sudo systemctl status $IPAEM_SERVICES;$DEPLOY_ARDUINO"
  else
    echo "ERROR: no extra option selected for deployed remotely"
    usage
  fi
else
  echo "ERROR: no option selected for deployed"
  usage
fi

if [ $rt -eq 0 ]; then
  echo "Deployment is DONE. Now is recommended you run setup from $DEPLOY_FOLDER"
else
  echo "Deployment FAILED. Check above messages"
fi


