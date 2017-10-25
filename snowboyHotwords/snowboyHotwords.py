import snowboydecoder
import sys
import signal
import subprocess
import sched, time
from threading import Timer

# Demo code for listening two hotwords at the same time

interrupted = False

def hotword_handler():
    print("Alternative hotword detected");
    subprocess.Popen(['bash','-c',"curl -i http://localhost:5000/ipaem/api/v1.0/hotword"])


def signal_handler(signal, frame):
    global interrupted
    interrupted = True
    exit(-1)


def interrupt_callback():
    global interrupted
    return interrupted

if len(sys.argv) != 3:
    print("Error: need to specify 2 model names")
    print("Usage: python demo.py 1st.model 2nd.model")
    sys.exit(-1)

models = sys.argv[1:]

# capture SIGINT signal, e.g., Ctrl+C
signal.signal(signal.SIGINT, signal_handler)
churri_is_said = False
sensitivity = [0.5]*len(models)
detector = snowboydecoder.HotwordDetector(models, sensitivity=sensitivity)
callbacks = [lambda: hotword_handler(),
             lambda: hotword_handler()]
print('Listening... Press Ctrl+C to exit')

# main loop
# make sure you have the same numbers of callbacks and models
detector.start(detected_callback=callbacks,
               interrupt_check=interrupt_callback,
               sleep_time=0.03)

detector.terminate()