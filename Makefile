# ----- Toolchain --------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

GCC     = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
SIZE    = $(TOOLCHAIN_PREFIX)size

# ----- Tools ------------------------------------------------------------------

MKDIR = mkdir -p
RMDIR = rm -rf

# ----- Directories and files --------------------------------------------------

NAME = firmware

OBJDIR = _obj
OUTDIR = _out

EXECUTABLE = $(OUTDIR)/$(NAME).elf
MAPFILE    = $(OUTDIR)/$(NAME).map

# ----- Source files -----------------------------------------------------------

INCLUDES += src
INCLUDES += src/drivers
INCLUDES += src/modules
INCLUDES += src/freertos/include
INCLUDES += src/freertos/portable/GCC/ATMega328P

SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/drivers/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)
SOURCES += $(wildcard src/freertos/*.c)
SOURCES += src/freertos/portable/GCC/ATMega328P/port.c
SOURCES += src/freertos/portable/MemMang/heap_3.c

# ----- Symbols ----------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDES))

CFLAGS   += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,-Map=$(MAPFILE)
LDFLAGS  += -Wl,--gc-sections

# ----- Objects ----------------------------------------------------------------

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

# ----- Rules ------------------------------------------------------------------

.PHONY: all clean download

all: $(EXECUTABLE)
	@echo # New line for better reading
	$(SIZE) $^
	@echo

clean:
	$(RMDIR) $(OUTDIR)
	$(RMDIR) $(OBJDIR)

download: $(EXECUTABLE)
	avrdude -p atmega328p -c avrispmkII -U flash:w:$<

$(OBJDIR)/%.o: %.c
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo $@

$(OBJDIR)/%.o: %.cpp
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo $@

$(EXECUTABLE): $(OBJECTS)
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(LDFLAGS) $^ -o $@
	@echo $@
