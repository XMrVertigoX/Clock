PROJECT_NAME = clock

# ----- Toolchain --------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

GCC     = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
SIZE    = $(TOOLCHAIN_PREFIX)size

# ----- Tools ------------------------------------------------------------------

MKDIR = mkdir -p
RMDIR = rm -rf

# ----- Directories and files --------------------------------------------------

OBJDIR = _obj
OUTDIR = _out

EXECUTABLE = $(OUTDIR)/$(PROJECT_NAME).elf
MAPFILE    = $(OUTDIR)/$(PROJECT_NAME).map

# ----- Source files -----------------------------------------------------------

# FreeRTOS
INCLUDES += freertos
SOURCES += $(wildcard freertos/*.c)

# Project
INCLUDES += src
INCLUDES += src/drivers
INCLUDES += src/modules
SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/drivers/*.c)
SOURCES += $(wildcard src/modules/*.cpp)

# ----- Symbols ----------------------------------------------------------------

SYMBOLS += BAUD=9600
SYMBOLS += F_CPU=16000000
SYMBOLS += __DELAY_BACKWARD_COMPATIBLE__

# ----- Flags ------------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDES))

CFLAGS   += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer
CXXFLAGS += -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-builtin -fno-common -fomit-frame-pointer

LDFLAGS  += -Wl,-Map=$(MAPFILE)
LDFLAGS  += -Wl,--gc-sections

# ----- Objects ----------------------------------------------------------------

SOURCE_FILES = $(sort $(realpath $(SOURCES)))
OBJECT_FILES = $(addsuffix .o,$(basename $(SOURCE_FILES)))

OBJECTS = $(addprefix $(OBJDIR),$(OBJECT_FILES))

# ----- Rules ------------------------------------------------------------------

.PHONY: all clean download

all: $(EXECUTABLE)
	@echo # New line for better reading
	$(SIZE) $^
	@echo # Another new line for even better reading

clean:
	$(RMDIR) $(OUTDIR)
	$(RMDIR) $(OBJDIR)

download: $(EXECUTABLE)
	avrdude -patmega328p -cavrispmkII -Uflash:w:$<

$(EXECUTABLE): $(OBJECTS)
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: /%.c
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: /%.cpp
	$(MKDIR) $(dir $@)
	$(GCC) $(GCCFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
