echo "Test GET all tasks..."
curl -i http://localhost:5001/ipaem/api/v1.0/lstest

echo "Test POST new color..."
curl -i -H "Content-Type: application/json" -X POST -d '{"color":"red"}' http://localhost:5001/ipaem/api/v1.0/color

