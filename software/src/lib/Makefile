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

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# normally you should not need to change the following settings
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #

# output directory.
DIR = ./

# C file for lookup tables
LOOKUP_TABLES_FILE = lookup_tables.c
LOOKUP_TABLES_XRES = 7
LOOKUP_TABLES_YRES = 14



# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
# please do not edit below
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #

# define programs and commands.
REMOVE = rm -f

# define Messages
MSG_BYE = make: *** have a nice day!
MSG_CLEANING = make: *** cleaning project...
MSG_GENERATING = make: *** generating source files...
MSG_HELLO = make: *** hiho!

# default target.
help:
	@echo "#------------------------------------------------------------------------------#"
	@echo "Hello and welcome to haggl's awesome universal AVR Makefile! ;-)"
	@echo
	@echo "Here are the available targets:"
	@echo "\tmake clean = clean up built project files"
	@echo "\tmake lookup = generate lookup tables"
	@echo "\tmake help = display this message"
	@echo
	@echo "Don't forget to adjust the variables in the Makefile header!"
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

# lookup: generate lookup tables file.
lookup:
	$(info )
	$(info $(MSG_GENERATING))
	./compute_lookup_tables.pl -x $(LOOKUP_TABLES_XRES) -y $(LOOKUP_TABLES_YRES) $(LOOKUP_TABLES_FILE)

# target: clean project.
clean: hello makeclean bye
makeclean:
	$(info )
	$(info $(MSG_CLEANING))
	$(REMOVE) $(LOOKUP_TABLES_FILE)

# phony targets.
.PHONY: bye clean hello lookup


.SECONDARY: $(OBJ)
.NOTPARALLEL:
