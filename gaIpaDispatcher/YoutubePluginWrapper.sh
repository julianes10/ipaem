#!/bin/bash 
echo "Wrapper to access youtube plugin in kodi"
echo "Here we go: $@"

################################################
function usage
{ 
  echo  "$0 
           --video-id        <VIDEOID>  
           --search         <QUOTED STRING> <VIDEO ID OUTPUT FILE> 
           --search-random  <QUOTED STRING> <VIDEO ID OUTPUT FILE> 
           --next           <OUTPUT_FILE>  
           --back           <OUTPUT_FILE>  
           --stop  
           --url            <URL> 
           --help"
  echo "Notes:
          --search option will play first item on search results list 
          --search-random option will play random item on search results list 
          --search,--search-random,--back and --stop will save result list and current playing item in  <OUTPUT_FILE> "
}

################################################
function playVideoId 
{
  echo "Asking youtube to play VideoID \"$1\"..."
  curl -i -X POST http://localhost:8080/jsonrpc  \
    -H "Content-Type: application/json" \
    --data '{ "jsonrpc": "2.0", "method": "Player.Open", "params": [{"file" : "plugin://plugin.video.youtube/?action=play_video&videoid='$1'"}], "id": 1 }'
}  


################################################
function stopPlayer 
{
  echo "Stopping kodi player..."
  curl -i -X POST http://localhost:8080/jsonrpc  \
    -H "Content-Type: application/json" \
    --data '{ "jsonrpc": "2.0", "method": "Player.Stop", "params": {"playerid":0}, "id": 1 }'
  curl -i -X POST http://localhost:8080/jsonrpc  \
    -H "Content-Type: application/json" \
    --data '{ "jsonrpc": "2.0", "method": "Player.Stop", "params": {"playerid":1}, "id": 1 }'

} 


################################################
### Picked from https://stackoverflow.com/questions/296536/how-to-urlencode-data-for-curl-command
function urlencode 
{
    if [[ $? != 3 ]]; then
        echo "Unexpected error" 1>&2
        return 2
    fi
    #echo "${data##/?}"
    echo "$1"
}
################################################
function searchString 
{
  local data
  data="$(curl -s -o /dev/null -w %{url_effective} --get --data-urlencode "$1" "")"
  local encoded
  encoded="${data##/?}"
  echo "Searching in Youtube: \"$1\" that is encoded as \"$encoded\"..."
  echo "Saving results in file: \"$2\"..."
  curl -s https://www.youtube.com/results?search_query="$encoded" | sed -ne 's/.*\(data-video-ids="[^ ]*"\).*/\1/p' | uniq > $2
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
elif [ "$1" == "--stop" ]; then
  stopPlayer
  exit 0
elif [ "$1" == "--url" ]; then
  video=$( echo "$2" | tr '?&#' '\n\n' | grep -e '^v=' | cut -d'=' -f2 )
  playVideoId $video
elif [ "$1" == "--video-id" ]; then
  playVideoId $2
elif [ "$1" == "--search" ]; then
  searchString "$2" "$3"
  echo "TODO PLAY FIRST"
elif [ "$1" == "--random" ]; then
  searchString "$2" "$3"
  echo "TODO PLAY RANDOM"
elif [ "$1" == "--next" ]; then
  echo "TODO PLAY NEXT from saved list $2"
elif [ "$1" == "--back" ]; then
  echo "TODO PLAY BACK from saved list $2"
else
  echo "ERROR, unknown option $1"
  usage
  exit -1
fi






