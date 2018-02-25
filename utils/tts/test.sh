#This is a benchmark 

EXAMPLE_ESP="Hola amigo como estas, la temperatura es de 23 grados" 
EXAMPLE_ENG="Hello my friend, how are you, the temperature is 23 degrees" 
ENG="This is a test in english language" 
ESP="Esto es una prueba en la lengua española" 


function theTest 
{
 echo "This is a test with $1" | $1
 sleep 1
 echo $ENG | $1
 sleep 1
 echo $EXAMPLE_ENG | $1
 sleep 1
 echo $ESP | $1 $2
 sleep 1
 echo $EXAMPLE_ESP | $1 $2
}

theTest "espeak" "-ven" 
sleep 2
theTest "festival --tts" "--language spanish" 



