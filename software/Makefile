# This Makefile cleans and builds all projects and generates documentation.

LIBRARY=./src/lib
PROJECTS=$(filter-out src/lib/.,$(wildcard src/*/.))

DOXYGEN=doxygen
MAKE=make

all: $(PROJECTS) doc

doc:
	$(DOXYGEN)

$(PROJECTS):
	$(info $(PROJECTS))
	$(MAKE) -C $@ clean compile

.NOTPARALLEL:
.PHONY: all doc $(PROJECTS)
# vim: tabstop=4 shiftwidth=4 noexpandtab
