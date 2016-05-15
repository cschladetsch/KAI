# Christian: 
#
# Top-level Makefile for all components and tests

SHELL = /bin/bash
INSTALL = /usr/bin/install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# Add your generic Compiler and Linker flags in here
include Makefile.conf

Projects = Library Executor Console 
DIRS = $(foreach proj, $(Projects), Projects/$(proj)/Source)
TEST_DIRS = $(foreach proj, $(Projects), Tests/$(proj))

# the sets of directories to do various things in
BUILDDIRS = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
CLEANDIRS = $(DIRS:%=clean-%)
TESTDIRS = $(TEST_DIRS)
DEPENDDIRS = $(DIRS:%=depend-%)

#MAKE = make -s

all: $(BUILDDIRS)
$(DIRS): $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C $(@:build-%=%)

depend: $(DEPENDDIRS)
$(DIRS): $(DEPENDDIRS)
$(DEPENDDIRS):
	$(MAKE) -C $(@:depend-%=%) depend
	$(MAKE) -C $(@:test-%=%) depend

install: $(INSTALLDIRS) all
$(INSTALLDIRS):
	$(MAKE) -C $(@:install-%=%) install

tests: $(TESTDIRS) all
$(TESTDIRS): 
	$(MAKE) -C $(@:test-%=%) tests

clean: $(CLEANDIRS)
	$(MAKE) -C $(@:clean-%=%) clean
	
.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all install clean test

