# vim: noexpandtab tabstop=4
#
# Copyright 2012-2015 Sebastian Neuser
#
# This file is part of the uMIDI firmware.
#
# The uMIDI firmware is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# The uMIDI firmware is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
#

# Target file name (without extension)
TARGET = main

# Output directory.
DIR = .

# Fuse bits
FUSE1 = 0x00
FUSE2 = 0xbf
FUSE4 = 0xfe
FUSE5 = 0xff

# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
OPT = s

# Programmer configuration for avrdude
AVRDUDE_PROGRAMMER_CONFIG = -c atmelice_pdi


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Normally you should not need to change the following settings
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# MCU name
MCU = atxmega128a4u

# Main CPU frequency
F_CPU = 32000000

# LUFA submodule directory
LUFA_PATH = lib/lufa

# LUFA configuration
ARCH = XMEGA
BOARD = NONE
F_USB = 48000000

# Compiler flags.
CFLAGS = -mmcu=$(MCU)
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -O$(OPT)
CFLAGS += -DF_CPU=$(F_CPU)UL

# LUFA library compile-time options and predefined tokens
CFLAGS += -Wl,--gc-sections
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections
CFLAGS += -fno-inline-small-functions
CFLAGS += -fno-strict-aliasing
CFLAGS += -DARCH=ARCH_$(ARCH)
CFLAGS += -DBOARD=BOARD_$(BOARD)
CFLAGS += -DF_USB=$(F_USB)UL
CFLAGS += -D USB_DEVICE_ONLY
CFLAGS += -D FIXED_CONTROL_ENDPOINT_SIZE=8
CFLAGS += -D FIXED_NUM_CONFIGURATIONS=1
CFLAGS += -D USE_FLASH_DESCRIPTORS
CFLAGS += -D USE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_BUSEVENT_PRILOW | USB_OPT_PLLCLKSRC)"
CFLAGS += -I$(LUFA_PATH)

XBOOT_PATH = lib/xboot

# Avrdude settings and variables.
AVRDUDE_FLAGS = $(AVRDUDE_PROGRAMMER_CONFIG)
AVRDUDE_FLAGS += -p x128a4u

AVRDUDE_WRITE_FLASH = -U flash:w:$(DIR)/$(TARGET).hex
AVRDUDE_VERIFY_FLASH = -U flash:v:$(DIR)/$(TARGET).hex
AVRDUDE_READ_EEPROM = -U eeprom:r:$(DIR)/eeprom.eep:i
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(DIR)/eeprom.eep:i
AVRDUDE_WRITE_FUSES = -U fuse1:w:$(FUSE1):m
AVRDUDE_WRITE_FUSES += -U fuse2:w:$(FUSE2):m
AVRDUDE_WRITE_FUSES += -U fuse4:w:$(FUSE4):m
AVRDUDE_WRITE_FUSES += -U fuse5:w:$(FUSE5):m


# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# Please do not edit below
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


# Define programs and commands.
SHELL = /bin/sh
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f
MKDIR = mkdir

# Define Messages
MSG_ASSEMBLING  = *** Assembling
MSG_CLEANING    = *** Cleaning project...
MSG_COMPILING   = *** Compiling
MSG_DOWNLOADING = *** Backing up EEPROM...
MSG_EEPROM      = *** Writing EEPROM...
MSG_FLASH       = *** Creating hex load file
MSG_FUSE        = *** Setting fuse bits...
MSG_GENERATING  = *** Generating source files...
MSG_LINKING     = *** Linking
MSG_PROGRAMING  = *** Programing MC...
MSG_UPDATE      = *** Creating firmware update file...

# Create the LUFA source path variables by including the LUFA root makefile
include $(LUFA_PATH)/LUFA/makefile
.DEFAULT_GOAL =

# Determine the list of C source files
SRC  = $(wildcard lib/*.c) $(wildcard *.c)
SRC += $(XBOOT_PATH)/xbootapi.c
SRC += $(LUFA_SRC_USB_DEVICE) $(LUFA_PATH)/LUFA/Drivers/USB/Class/Device/CDCClassDevice.c

# Default target.
help:
	@echo "#------------------------------------------------------------------------------#"
	@echo "Hello and welcome to haggl's awesome universal AVR Makefile! ;-)"
	@echo
	@echo "These are the available targets:"
	@echo "    make all      :  Clean, compile and install"
	@echo "    make allinc   :  Backup, clean, compile, install and eep"
	@echo "    make asm      :  Compile the software and create .asm-files"
	@echo "    make backup   :  Read data from eeprom"
	@echo "    make compile  :  Compile the software and create .hex-file"
	@echo "    make clean    :  Clean up built project files"
	@echo "    make eep      :  Write data to eeprom"
	@echo "    make fuse     :  Write fuse bits"
	@echo "    make help     :  Display this message"
	@echo "    make install  :  Load the hex file to the device, using avrdude"
	@echo "    make package  :  Generate a firmware update file"
	@echo
	@echo "You may want to adjust some variables in the Makefile header..."
	@echo "AND:"
	@echo "Be careful with fuse bits!"
	@echo "#------------------------------------------------------------------------------#"


# Internal rule:
# Compile object files from C source files.
$(DIR)/%.o: %.c
	$(info )
	$(info $(MSG_COMPILING) $<)
	$(CC) -c $(CFLAGS) $< -o $@

# Internal rule:
# Link ELF output file from object files.
OBJ = $(addprefix $(DIR)/,$(patsubst %.c,%.o,$(SRC)))
%.elf: $(OBJ)
	$(info )
	$(info $(MSG_LINKING) $@)
	$(CC) $(CFLAGS) $(OBJ) -o $(DIR)/$@

# Internal rule:
# Create final output file (.hex) from ELF.
%.hex: %.elf
	$(info )
	$(info $(MSG_FLASH) $@)
	$(OBJCOPY) -O ihex -R .eeprom $(DIR)/$< $(DIR)/$@

# Internal rule:
# Create assembler files from C source files.
%.s: %.c
	$(info )
	$(info $(MSG_ASSEMBLING) $<)
	$(CC) -S $(CFLAGS) $< -o $(DIR)/$@

# Target:
# Compile project and generate hex load file
compile: $(TARGET).hex

# Target:
# Generate main target assembly.
ASM = $(addprefix $(DIR)/,$(patsubst %.c,%.s,$(SRC)))
asm: $(ASM)

# Target:
# Clean project.
clean:
	$(info )
	$(info $(MSG_CLEANING))
	$(REMOVE) $(DIR)/$(TARGET).elf
	$(REMOVE) $(DIR)/$(TARGET).hex
	$(REMOVE) $(DIR)/$(TARGET).bin
	$(REMOVE) $(OBJ)
	$(REMOVE) $(ASM)

# Target:
# Program the device.
install: compile
	$(info )
	$(info $(MSG_PROGRAMING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_VERIFY_FLASH)

# Target:
# Create firmware update package
package: $(TARGET).elf
	$(info )
	$(info $(MSG_UPDATE))
	$(OBJCOPY) -O binary -R .eeprom $(DIR)/$< $(DIR)/$(patsubst %.elf,%.bin,$<)
	./pack_fwupdate.pl $(DIR)/$(patsubst %.elf,%.bin,$<)

# Target:
# Set fuse bits.
fuse:
	$(info )
	$(info $(MSG_FUSE))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FUSES)

# Target:
# Backup EEPROM data
backup:
	$(info )
	$(info $(MSG_DOWNLOADING))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_READ_EEPROM)

# Target:
# EEPROM write
eep:
	$(info )
	$(info $(MSG_EEPROM))
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

# Target:
# Clean, compile and install.
all: clean install

# Target:
# Clean, backup EEPROM, compile, install and write EEPROM.
allinc: clean backup install eep

# Phony targets.
.PHONY: all allinc asm backup compile clean eep fuse install package


.SECONDARY:
.NOTPARALLEL:
