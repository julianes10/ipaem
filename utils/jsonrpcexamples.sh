curl -i -X POST http://localhost:8080/jsonrpc  -H "Content-Type: application/json" --data '{ "jsonrpc": "2.0", "method": "Player.Open", "params": [{"file" : "plugin://plugin.video.youtube/?action=play_video&videoid='P8ZHToXlp1o'"}], "id": 1 }'


curl -i -X POST http://localhost:8080/jsonrpc      -H "Content-Type: application/json"     --data '{"jsonrpc": "2.0", "method": "Player.GetActivePlayers", "id":1}'


curl -i -X POST http://localhost:8080/jsonrpc -H "Content-Type: application/json" --data '{ "jsonrpc": "2.0", "method": "Player.Stop", "params": { "playerid":1 }, "id": 1 }'

