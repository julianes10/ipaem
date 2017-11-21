#!/bin/bash 
echo "This act as a wrapper to access youtube plugin in kodi"
#args: $1(stationId | STOP) 
echo "Here we go $@"

function usage
{
  echo "$0 (url | STOP)" 
}

if [ "$1" == "-h" -o "$1" == "--help" ]; then
  usage
  exit 0
fi

if [ "$1" == "" ]; then
  echo "ERROR, args needed"
  usage
  exit -1
fi

#Stop whatever it is playing
echo "Stopping player..."
curl -i -X POST http://localhost:8080/jsonrpc  \
    -H "Content-Type: application/json" \
    --data '{ "jsonrpc": "2.0", "method": "Player.Stop", "params": {"playerid":0}, "id": 1 }'

if [ "$1" == "STOP" ]; then
   exit 0
fi


video=$( echo "$1" | tr '?&#' '\n\n' | grep -e '^v=' | cut -d'=' -f2 )

#If args is not just stop, then let's put the station using amazing tuneIn¡
echo "Tune In statios: $1..."
curl -i -X POST http://localhost:8080/jsonrpc  \
    -H "Content-Type: application/json" \
    --data '{ "jsonrpc": "2.0", "method": "Player.Open", "params": [{"file" : "plugin://plugin.video.youtube/?action=play_video&videoid='$video'"}], "id": 1 }'


