# Compiler settings
CC = g++
CXXFLAGS = -std=c++11 -Wall  # Compiler flags: C++11 standard, all warnings
LDFLAGS =                    # Linker flags (empty by default)

# Makefile settings - Can be customized.
BUILDDIR = build
APPNAME = $(BUILDDIR)/detector
EXT = .cpp
SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*$(EXT))             # Get all .cpp files in src/ (becomes src/main.cpp src/utils.cpp)
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)    # Convert src/file.cpp -> obj/file.o (becomes obj/main.o obj/utils.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)      			# Convert obj/file.o -> file.d (main.d utils.d)

# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)


# Targets
all: $(APPNAME)

# Build the app $@ - target name, $^ - all prerequisites
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)      

# Create the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Cleaning rules for Unix-based OS
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

# Cleaning rules for Windows OS
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)


# Here part where we run code
ARGS = /dev/pts/4

run: $(APPNAME)
	./$(APPNAME) $(ARGS)
