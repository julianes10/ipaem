BOARD_TAG    = uno
ARDUINO_PORT = /dev/ttyUSB0
ARDUINO_LIBS = SoftwareSerial FastLED SimpleTimer

USER_LIB_PATH = $(realpath ./lib)

AVRDUDE_ARD_BAUDRATE = 115200

ARDUINO_DIR  = /usr/share/arduino

include $(ARDUINO_DIR)/Arduino.mk


