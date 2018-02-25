#!/bin/bash 
echo "Wrapper to access piInoAgent"
echo "Here we go: $@"

################################################
function usage
{ 
  echo  "$0 
           --color <COLOR>
           --on
           --off   
           --help"
  echo "Notes:
           --color will change main color theme on led strip
           <COLOR> should be a color known by python colour module or #RRGGBB html notation  
"
}

################################################
function setColor 
{
  echo "Setting up main color in led strip: \"$1\"..."
  curl -i -H "Content-Type: application/json" -X POST -d '{"color":"'$1'"}' http://localhost:5001/ipaem/api/v1.0/color
} 

################################################
function turnOn 
{
  setColor "white"
} 
################################################
function turnOff 
{
  echo "Turning off led strip..."
  curl -i -H "Content-Type: application/json" -X POST -d '{"reset":true}' http://localhost:5001/ipaem/api/v1.0/reset
} 

################################################
################################################
################################################
if [ "$1" == "-h" -o "$1" == "--help" ]; then
  usage
  exit 0
elif [ "$1" == "" ]; then
  echo "ERROR, args needed"
  usage
  exit -1
elif [ "$1" == "--color" ]; then
  setColor $2
  exit 0
elif [ "$1" == "--on" ]; then
  turnOn
  exit 0
elif [ "$1" == "--off" ]; then
  turnOff
  exit 0
else
  echo "ERROR, unknown option $1"
  usage
  exit -1
fi






