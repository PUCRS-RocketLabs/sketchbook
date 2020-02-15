## Comments on BOARD_SUB on Arduino 1.6.X
## https://github.com/sudar/Arduino-Makefile/issues/398
BOARD_TAG = diecimila
BOARD_SUB = atmega328
ARDUINO_DIR = $(HOME)/local/latest
ARDUINO_SKETCHBOOK =  $(HOME)/Dropbox/sketchbook
ARDUINO_LIBS = WiFly SPI  # RTClib SD Wire LiquidCrystal SPI
MONITOR_PORT = /dev/ttyUSB0
CFLAGS_STD = -Wno-write-strings -std=gnu99
include $(HOME)/local/Arduino-Makefile/Arduino.mk
#include /usr/share/arduino/Arduino.mk

clean::
	rm -vrf *~

#ARDUINO_CORE_PATH = /home/filipi/.arduino15/packages/arduino/hardware/avr/1.6.2/cores/arduino
##AVR_TOOLS_DIR = /home/filipi/.arduino15/packages/arduino/tools/
##BOARD_TAG = uno (see make show_boards)
##BOARD_TAG = atmega328
##BOARD_TAG = atmega2560
#BOARD_TAG = leonardo
#ARDUINO_LIBS = WiFly SPI
##MONITOR_PORT = /dev/ttyUSB0
#MONITOR_PORT = /dev/ttyACM0
#CFLAGS_STD = -Wno-write-strings -std=gnu99
#include /usr/share/arduino/Arduino.mk
