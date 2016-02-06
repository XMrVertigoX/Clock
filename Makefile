include config.mk

# ------------------------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Sources ----------------------------------------------------------------

INCLUDES += lib/atmega328p/inc
SOURCES += $(wildcard lib/atmega328p/src/*.cpp)

INCLUDES += src/include
SOURCES += $(wildcard src/*.cpp)

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

# CPPFLAGS +=

CFLAGS   += -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,--gc-sections

# ----- Rules ------------------------------------------------------------------

include rules.mk

.PHONY: program

program: $(BINARY)
	avrdude -p atmega328p -c avrispmkII -U flash:w:$<
