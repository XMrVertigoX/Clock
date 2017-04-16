PROJECT_NAME = clock

ISP = -c avrispmkII
# ISP = -c arduino -P /dev/ttyACM0

# ----- Symbols ---------------------------------------------------------------

SYMBOLS += __DELAY_BACKWARD_COMPATIBLE__
SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
# SYMBOLS += NDEBUG

# ----- Source files ----------------------------------------------------------

# Libraries
INCLUDE_DIRS += libs
INCLUDE_DIRS += libs/FreeRTOS

# xXx
SOURCE_FILES += $(wildcard libs/xXx/os/*.cpp)
SOURCE_FILES += $(wildcard libs/xXx/support/*.cpp)
SOURCE_FILES += $(wildcard libs/xXx/utils/*.cpp)

# FreeRTOS
SOURCE_FILES += $(wildcard libs/FreeRTOS/*.c)

# Project
SOURCE_FILES += $(shell find ./src -type f -name *.c)
SOURCE_FILES += $(shell find ./src -type f -name *.cpp)

# ----- Flags -----------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

COMMON_CFLAGS += -O3

# ----- Rules -----------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

include libs/xXx/utils/rules.mk

download: $(EXECUTABLE)
	avrdude $(ISP) -q -p atmega328p -U flash:w:$<
