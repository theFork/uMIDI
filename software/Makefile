# This Makefile cleans and builds all projects and generates documentation.

LIBRARY=./src/lib
PROJECTS= $(filter-out src/lib/.,$(wildcard src/*/.))

DOXYGEN="doxygen"
MAKE="make"

all: $(PROJECTS) doc

doc:
	$(DOXYGEN)

$(PROJECTS): $(LIBRARY)
	$(info $(PROJECTS))
	$(MAKE) -C $@ clean compile

$(LIBRARY):
	$(MAKE) -C $@ clean lookup

.NOTPARALLEL:
.PHONY: all doc $(LIBRARY) $(PROJECTS)
# vim: tabstop=4 shiftwidth=4 noexpandtab
