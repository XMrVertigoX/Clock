PROJECT_NAME = clock

# ----- Symbols ---------------------------------------------------------------

SYMBOLS += BAUD=57600
SYMBOLS += F_CPU=16000000
SYMBOLS += __DELAY_BACKWARD_COMPATIBLE__

# ----- Source files ----------------------------------------------------------

# FreeRTOS
INCLUDE_DIRS += freertos
SOURCE_FILES += $(wildcard freertos/*.c)

# Project
INCLUDE_DIRS += src
INCLUDE_DIRS += src/drivers
INCLUDE_DIRS += src/modules

SOURCE_FILES += $(shell find ./src -type f -name *.c)
SOURCE_FILES += $(shell find ./src -type f -name *.cpp)

# ----- Flags -----------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

COMMON_CFLAGS = -Og -g -nostdlib -ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -fno-builtin

CFLAGS +=

CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-threadsafe-statics

LDFLAGS  += -Wl,--gc-sections

# ----- Rules -----------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

include libs/xXx/utils/rules.mk

flash: $(EXECUTABLE)
	@avrdude -q -patmega328p -cavrispmkII -Uflash:w:$<
