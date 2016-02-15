include config.mk

ISP = avrispmkII

# ------------------------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Sources ----------------------------------------------------------------

INCLUDES += lib/drivers/abstract/
SOURCES += $(wildcard lib/drivers/abstract/*.cpp)

INCLUDES += lib/drivers/atmega328p
SOURCES += $(wildcard lib/drivers/atmega328p/*.cpp)

INCLUDES += $(wildcard lib/modules/*)
SOURCES += $(wildcard lib/modules/*/*.cpp)

INCLUDES += src/include
SOURCES += $(wildcard src/*.cpp)

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += -include lib/drivers/atmega328p/operators.h

CFLAGS   += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,--gc-sections

# ----- Rules ------------------------------------------------------------------

include rules.mk

.PHONY: download

download: $(BINARY)
	avrdude -p atmega328p -c $(ISP) -U flash:w:$<
