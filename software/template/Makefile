#
# Copyright 2012-2014 Sebastian Neuser
#
# This file is part of the muMIDI firmware.
#
# The muMIDI firmware is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The muMIDI firmware is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the muMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
#

# MCU name
MCU = atmega88p

# main oscillator and cpu frequencies
F_OSC = 1000000
F_CPU = 1000000

# optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
OPT = s

# target file name (without extension)
TARGET = main

# C source files
SRC = $(wildcard *.c)

# output directory.
DIR = ./

# fuse bits
HFUSE = 0xd1
LFUSE = 0xe1


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# normally you should not need to change the following settings
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# compiler flags.
CFLAGS = -mmcu=$(MCU)
CFLAGS += -O$(OPT)
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=c99
CFLAGS += -DF_OSC=$(F_OSC)
CFLAGS += -DF_CPU=$(F_CPU)

# avrdude settings and variables.
AVRDUDE_FLAGS = -p $(MCU)
AVRDUDE_FLAGS += -c STK500
AVRDUDE_FLAGS += -P /dev/ttyACM0
AVRDUDE_FLAGS += -B 4

AVRDUDE_WRITE_FLASH = -U flash:w:$(DIR)$(TARGET).hex
AVRDUDE_VERIFY_FLASH = -U flash:v:$(DIR)$(TARGET).hex
AVRDUDE_READ_EEPROM = -U eeprom:r:$(DIR)eeprom.eep:i
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(DIR)eeprom.eep:i
AVRDUDE_WRITE_FUSES = -U hfuse:w:$(HFUSE):m
AVRDUDE_WRITE_FUSES += -U lfuse:w:$(LFUSE):m


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# please do not edit below
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# define programs and commands.
SHELL = /bin/sh
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f
MKDIR = mkdir

# define Messages
MSG_ASSEMBLING = make: *** assembling...
MSG_BYE = make: *** have a nice day!
MSG_CLEANING = make: *** cleaning project...
MSG_COMPILING = make: *** compiling...
MSG_DOWNLOADING = make: *** backing up EEPROM...
MSG_DUMPING = make: *** writing EEPROM...
MSG_EEPROM = make: *** creating load file for EEPROM...
MSG_FLASH = make: *** creating load file for Flash...
MSG_LINKING = make: *** linking...
MSG_HELLO = make: *** hiho!
MSG_PROGRAMING = make: *** programing MC...
MSG_FUSE = make: *** setting fuse bits...

# default target.
help:
	@echo "#------------------------------------------------------------------------------#"
	@echo "Hello and welcome to haggl's awesome universal AVR Makefile! ;-)"
	@echo
	@echo "Here are the available targets:"
	@echo "\tmake all = clean, compile and install"
	@echo "\tmake allinc = backup, clean, compile, install and eep"
	@echo "\tmake asm = compile the software and create .asm-file"
	@echo "\tmake backup = read data from eeprom"
	@echo "\tmake compile = compile the software and create .hex and .asm-files"
	@echo "\tmake clean = clean up built project files"
	@echo "\tmake eep = write data to eeprom"
	@echo "\tmake fuse = write fuse bits"
	@echo "\tmake help = display this message"
	@echo "\tmake hex = compile the software and create .hex-file"
	@echo "\tmake install = load the hex file to the device, using avrdude"
	@echo
	@echo "Don't forget to adjust the variables in the Makefile header!"
	@echo "AND:"
	@echo "Be careful with fuse bits!"
	@echo "#------------------------------------------------------------------------------#"


# say hello.
hello:
	$(info )
	$(info $(MSG_HELLO))

# say bye.
bye:
	$(info )
	$(info $(MSG_BYE))
	$(info )

# compile: create object files from C source files.
$(DIR)%.o: %.c
	$(info )
	$(info $(MSG_COMPILING) $<)
	$(CC) -c $(CFLAGS) $< -o $@

# link: create ELF output file from object files.
OBJ = $(addprefix $(DIR),$(patsubst %.c,%.o,$(SRC)))
%.elf: $(OBJ)
	$(info )
	$(info $(MSG_LINKING) $@)
	$(CC) $(CFLAGS) $(OBJ) -o $(DIR)$@

# create final output file (.hex) from ELF output file.
hex: $(TARGET).hex
%.hex: %.elf
	$(info )
	$(info $(MSG_FLASH) $@)
	$(OBJCOPY) -O ihex -R .eeprom $(DIR)$< $(DIR)$@

# compile: create assembler files from C source files.
asm: $(TARGET).s
%.s: %.c
	$(info )
	$(info $(MSG_ASSEMBLING) $<)
	$(CC) -S $(CFLAGS) $< -o $(DIR)$@

# target: clean project.
clean: hello makeclean bye
makeclean:
	$(info )
	$(info $(MSG_CLEANING))
	$(REMOVE) $(DIR)$(TARGET).hex
	$(REMOVE) $(DIR)$(TARGET).elf
	$(REMOVE) $(DIR)$(TARGET).s
	$(REMOVE) $(OBJ)

# target: compile project
compile: hello hex asm bye

# target: program the device.
install: hello makeinstall bye
makeinstall:
	$(info )
	$(info $(MSG_PROGRAMING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_VERIFY_FLASH)

# target: set fuse bits.
fuse: hello makefuse bye
makefuse:
	$(info )
	$(info $(MSG_FUSE))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSES)

# target: backup eeprom data
backup: hello makebackup bye
makebackup:
	$(info )
	$(info $(MSG_DOWNLOADING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_EEPROM)

# target: eeprom write
eep: hello makeeep bye
makeeep:
	$(info )
	$(info $(MSG_DUMPING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

# target: compile and install.
all: hello makeclean hex makeinstall bye

# target: backup, compile, install and eep.
allinc: hello makeclean hex makebackup makeinstall makeeep bye

# phony targets.
.PHONY: all allinc asm backup compile bye clean eep fuse hello hex makebackup makeclean makeeep makefuse makeinstall install


.SECONDARY: $(OBJ)
.NOTPARALLEL:
