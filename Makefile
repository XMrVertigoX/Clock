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

BINARY     = $(OUTDIR)/$(NAME).bin
EXECUTABLE = $(OUTDIR)/$(NAME).elf
MAPFILE    = $(OUTDIR)/$(NAME).map

# ----- Source files -----------------------------------------------------------

INCLUDES += src/drivers
INCLUDES += src/modules

SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/drivers/*.cpp)
SOURCES += $(wildcard src/modules/*.cpp)

# ----- Symbols ----------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += F_SCL=400000

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += -include src/heap.h
CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDES))

CFLAGS   += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += $(addprefix -l,$(LIBS))
LDFLAGS  += -Wl,-Map=$(MAPFILE)
LDFLAGS  += -Wl,--gc-sections

# ----- Other ------------------------------------------------------------------

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

# ----- Rules ------------------------------------------------------------------

.PHONY: all clean download size

.PRECIOUS: $(OBJECTS)

.SILENT:

all: $(BINARY)

clean:
	$(RMDIR) $(OUTDIR)
	$(RMDIR) $(OBJDIR)

download: $(BINARY)
	avrdude -p atmega328p -c avrispmkII -U flash:w:$<

size: $(EXECUTABLE)
	$(SIZE) $^

$(EXECUTABLE): $(OBJECTS)
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(LDFLAGS) $^ -o $@
	echo $@

$(BINARY): $(EXECUTABLE)
	$(OBJCOPY) -O binary $< $@
	echo $@

# ----- Pattern rules ----------------------------------------------------------

$(OBJDIR)/%.o: %.c
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
	echo $@

$(OBJDIR)/%.o: %.cpp
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	echo $@
