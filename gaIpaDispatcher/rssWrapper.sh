#!/bin/bash 
echo "This act as a wrapper to tts rss news"
#args: $1(stationId | STOP) 
echo "Here we go $@"

function usage
{
  echo "$0 (rss | STOP)" 
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
echo "Stopping rss speech..."
killall pico2wave

if [ "$1" == "STOP" ]; then
   exit 0
fi

#If args is not just stop, then let's put the rss to sing
echo "Checkout rss news from: $1..."

feed -o $1 | head -n3 >/tmp/news
input=/tmp/news
while IFS= read -r var
do
  pico2wave -l es-ES -w /tmp/lookdave.wav "$var" && aplay /tmp/lookdave.wav
  sleep 1
done < "$input"




