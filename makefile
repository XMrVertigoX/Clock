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

GCCFLAGS      += -mmcu=atmega328p

COMMON_CFLAGS += -fdata-sections
COMMON_CFLAGS += -ffunction-sections
COMMON_CFLAGS += -fno-builtin
COMMON_CFLAGS += -fno-exceptions
COMMON_CFLAGS += -fno-unwind-tables
COMMON_CFLAGS += -g
COMMON_CFLAGS += -nostdlib
COMMON_CFLAGS += -Og

CFLAGS        +=

CXXFLAGS      += -fno-rtti
CXXFLAGS      += -fno-threadsafe-statics

CPPFLAGS      += $(addprefix -D,$(SYMBOLS))
CPPFLAGS      += $(addprefix -I,$(INCLUDE_DIRS))

LDFLAGS       += -Wl,--gc-sections

# ----- Rules -----------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

include libs/xXx/utils/rules.mk

download: $(EXECUTABLE)
	@avrdude -q -patmega328p -cavrispmkII -Uflash:w:$<
