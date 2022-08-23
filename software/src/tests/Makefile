# Dimensions for lookup tables
LOOKUP_TABLES_FILE = lib/lookup_tables.c
LOOKUP_TABLES_YRES = 10

# Compiler flags
CFLAGS = -DUNIT_TEST
CFLAGS += -Ilib
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wstrict-prototypes

# Define programs and commands.
SHELL = /bin/sh
CC = cc
OBJCOPY = objcopy
REMOVE = rm -f
MKDIR = mkdir

# Define Messages
MSG_ASSEMBLING  = *** Assembling
MSG_CLEANING    = *** Cleaning project...
MSG_COMPILING   = *** Compiling
MSG_GENERATING  = *** Generating source files...
MSG_LINKING     = *** Linking

# Determine the list of C source files
SRC  = $(LOOKUP_TABLES_FILE) lib/math.c $(wildcard *.c)

# List of object files
OBJ = $(patsubst %.c,%.o,$(SRC))

# Default target.
help:
	@echo "#------------------------------------------------------------------------------#"
	@echo "These are the available targets:"
	@echo "    make clean    :  clean up built project files"
	@echo "    make help     :  display this message"
	@echo "#------------------------------------------------------------------------------#"

# Generate lookup tables file.
$(LOOKUP_TABLES_FILE):
	$(info )
	$(info $(MSG_GENERATING))
	../../toolchain/compute_lookup_tables.pl -y $(LOOKUP_TABLES_YRES) $(LOOKUP_TABLES_FILE)

# Compile object files from C source files.
%.o: %.c
	$(info )
	$(info $(MSG_COMPILING) $<)
	$(CC) -c $(CFLAGS) $< -o $@

# Link ELF output file from object files.
%: $(OBJ)
	$(info )
	$(info $(MSG_LINKING) $@)
	$(CC) $(CFLAGS) $(OBJ) -o $@

# Clean project.
clean:
	$(info )
	$(info $(MSG_CLEANING))
	$(REMOVE) $(LOOKUP_TABLES_FILE)
	$(REMOVE) $(OBJ)

.PHONY: Makefile clean
.SECONDARY: $(OBJ)
.NOTPARALLEL:
