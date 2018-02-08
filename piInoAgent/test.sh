echo "Test GET status..."
curl -i http://localhost:5001/ipaem/api/v1.0/status

echo "Test POST new debug on..."
curl -i -H "Content-Type: application/json" -X POST -d '{"debug":true}' http://localhost:5001/ipaem/api/v1.0/debug

echo "Test POST new color..."
curl -i -H "Content-Type: application/json" -X POST -d '{"color":"red"}' http://localhost:5001/ipaem/api/v1.0/color


echo "Test POST new timeout..."
curl -i -H "Content-Type: application/json" -X POST -d '{"timeout":200}' http://localhost:5001/ipaem/api/v1.0/timeout


echo "Test POST new pause..."
curl -i -H "Content-Type: application/json" -X POST -d '{"pause":2000}' http://localhost:5001/ipaem/api/v1.0/pause


echo "Test POST new mode..."
curl -i -H "Content-Type: application/json" -X POST -d '{"mode":"T"}' http://localhost:5001/ipaem/api/v1.0/mode

echo "Test POST misc..."
curl -i -H "Content-Type: application/json" -X POST -d '{"mode":"K", "color":"blue"}' http://localhost:5001/ipaem/api/v1.0/misc


echo "Test POST new debug off.."
curl -i -H "Content-Type: application/json" -X POST -d '{"debug":false}' http://localhost:5001/ipaem/api/v1.0/debug

