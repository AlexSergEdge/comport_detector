CC = g++
CFLAGS = -std=c++11 -Wall  # Compiler flags: C++11 standard, all warnings

BUILDDIR = build
TARGET = $(BUILDDIR)/set_signal
EXT = .cpp
SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*$(EXT))             # Get all .cpp files in src/
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)    # Get object files for all .cpp files
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)      			# Get dependency files (.d) for all .o files

.PHONY: default all clean

default: $(TARGET)
all: default

# Build the app ($@ - target name, $^ - all prerequisites)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Create the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Include dependency rules
-include $(DEP)

# Build .o files
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f $(OBJ) $(DEP) $(TARGET)
