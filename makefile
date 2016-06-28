PROJECT_NAME = clock

# ----- Toolchain -------------------------------------------------------------

TOOLCHAIN_PREFIX = avr-

GCC     = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
SIZE    = $(TOOLCHAIN_PREFIX)size

# ----- Tools -----------------------------------------------------------------

MKDIR = mkdir -p
RMDIR = rm -rf

# ----- Directories and files -------------------------------------------------

OBJDIR = _obj
OUTDIR = _out

EXECUTABLE = $(OUTDIR)/$(PROJECT_NAME).elf
MAPFILE    = $(OUTDIR)/$(PROJECT_NAME).map

# ----- Source files ----------------------------------------------------------

# FreeRTOS
INCLUDES_DIRS += freertos
SOURCE_FILES += $(wildcard freertos/*.c)

# Project
INCLUDES_DIRS += src
INCLUDES_DIRS += src/drivers
INCLUDES_DIRS += src/modules
SOURCE_FILES += $(wildcard src/*.cpp)
SOURCE_FILES += $(wildcard src/drivers/*.c)
SOURCE_FILES += $(wildcard src/modules/*.cpp)
SOURCE_FILES += $(wildcard src/support/*.cpp)

# ----- Symbols ---------------------------------------------------------------

SYMBOLS += BAUD=57600
SYMBOLS += F_CPU=16000000
SYMBOLS += __DELAY_BACKWARD_COMPATIBLE__

# ----- Flags -----------------------------------------------------------------

GCCFLAGS += -mmcu=atmega328p

CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDES_DIRS))

COMMON_COMPILER_FLAGS = -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-unwind-tables -nostdlib

CFLAGS += $(COMMON_COMPILER_FLAGS)

CXXFLAGS += $(COMMON_COMPILER_FLAGS)
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-threadsafe-statics

LDFLAGS  += -Wl,-Map=$(MAPFILE)
LDFLAGS  += -Wl,--gc-sections

# ----- Objects ---------------------------------------------------------------

SORTED_SOURCE_FILES = $(sort $(realpath $(SOURCE_FILES)))
SORTED_OBJECT_FILES =  $(addprefix $(OBJDIR),$(addsuffix .o,$(basename $(SORTED_SOURCE_FILES))))

# ----- Rules -----------------------------------------------------------------

.PHONY: all clean download

.SILENT:

all: $(EXECUTABLE)
	@echo # New line for better reading
	@$(SIZE) $^
	@echo # Another new line for even better reading

clean:
	@echo [ RMD ] $(OUTDIR) & $(RMDIR) $(OUTDIR)
	@echo [ RMD ] $(OBJDIR) & $(RMDIR) $(OBJDIR)

download: $(EXECUTABLE)
	@avrdude -patmega328p -cavrispmkII -Uflash:w:$<

$(EXECUTABLE): $(SORTED_OBJECT_FILES)
	@$(MKDIR) $(dir $@)
	@echo [ LNK ] $(notdir $@)
	@$(GCC) $(GCCFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: /%.c
	@$(MKDIR) $(dir $@)
	@echo [ CMP ] $(notdir $@)
	@$(GCC) $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: /%.cpp
	@$(MKDIR) $(dir $@)
	@echo [ CMP ] $(notdir $@)
	@$(GCC) $(GCCFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
