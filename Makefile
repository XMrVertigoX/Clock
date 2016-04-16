include config.mk

ISP = avrispmkII

# ------------------------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Sources ----------------------------------------------------------------

INCLUDES += src/drivers
INCLUDES += src/modules

SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/drivers/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += -include src/heap.h

CFLAGS   += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,--gc-sections

# ----- Rules ------------------------------------------------------------------

include rules.mk

.PHONY: download

download: $(BINARY)
	avrdude -p atmega328p -c $(ISP) -U flash:w:$<
