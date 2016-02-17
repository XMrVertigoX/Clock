# ----- Files ------------------------------------------------------------------

BINARY     = $(OUTDIR)/$(NAME).bin
EXECUTABLE = $(OUTDIR)/$(NAME).elf
MAPFILE    = $(OUTDIR)/$(NAME).map

# ----- Flags ------------------------------------------------------------------

CPPFLAGS += $(addprefix -D,$(SYMBOLS))
CPPFLAGS += $(addprefix -I,$(INCLUDES))

LDFLAGS  += $(addprefix -l,$(LIBS))
LDFLAGS  += -Wl,-Map=$(MAPFILE)

# ----- Other ------------------------------------------------------------------

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SOURCES))))

# ----- Rules ------------------------------------------------------------------

.PHONY: all clean size

.PRECIOUS: $(OBJECTS)

.SILENT:

all: $(BINARY)

clean:
	$(RM) $(OUTDIR)
	$(RM) $(OBJDIR)

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
