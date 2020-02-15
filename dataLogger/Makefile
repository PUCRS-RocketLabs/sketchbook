## Comments on BOARD_SUB on Arduino 1.6.X
## https://github.com/sudar/Arduino-Makefile/issues/398
BOARD_TAG = mega
BOARD_SUB = atmega2560
#BOARD_TAG = nano
#BOARD_SUB = atmega328
ARDUINO_DIR = $(HOME)/local/latest
#ARDUINO_SKETCHBOOK =  $(HOME)/Dropbox/sketchbook
ARDUINO_SKETCHBOOK =  $(HOME)/sketchbook
ARDUINO_LIBS = SD SPI RTClib Wire #LiquidCrystal 
#MONITOR_PORT = /dev/ttyUSB0
MONITOR_PORT = /dev/ttyACM0
CFLAGS_STD = -Wno-write-strings -std=gnu99
include $(HOME)/local/Arduino-Makefile/Arduino.mk
#include /usr/share/arduino/Arduino.mk

check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

clean::
	@rm -rfv *~
