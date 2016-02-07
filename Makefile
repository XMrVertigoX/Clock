include config.mk

ISP = avrispmkII
# ISP = usbtiny

# ------------------------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Sources ----------------------------------------------------------------

INCLUDES += lib/atmega328p/inc
SOURCES += $(wildcard lib/atmega328p/src/*.cpp)

INCLUDES += lib/modules/inc
SOURCES += $(wildcard lib/modules/src/*.cpp)

INCLUDES += src/include
SOURCES += $(wildcard src/*.cpp)

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

# CPPFLAGS +=

CFLAGS   += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,--gc-sections

# ----- Rules ------------------------------------------------------------------

include rules.mk

.PHONY: download

download: $(BINARY)
	avrdude -p atmega328p -c $(ISP) -U flash:w:$<
