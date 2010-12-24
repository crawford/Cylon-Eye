-include local.mk

CC ?= gcc
DC ?= gcc
GCJ ?= gcj

CFLAGS += -fPIC -std=gnu99
CPPFLAGS += -Wall -Iinclude
JAVAFLAGS += -Icysim/cmdproc

CPPFLAGS += $(shell pkg-config --cflags libxml-2.0) $(shell pkg-config --cflags glib-2.0)
LDFLAGS += $(shell pkg-config --libs libxml-2.0) $(shell pkg-config --libs glib-2.0)

TOPDIR := $(PWD)
CURDIR := $(TOPDIR)

SUBDIRS := src cysim/cmdproc

CLEAN_TARGETS := $(SUBDIRS:=/clean)
DEPENDENCIES_TARGETS := $(SUBDIRS:=/dependencies)
COMPILE_TARGETS := $(SUBDIRS:=/compile)

.PHONY: dependencies clean all compile
.DEFAULT_GOAL: all

all:
	$(MAKE) dependencies
	$(MAKE) compile

compile: $(COMPILE_TARGETS)
dependencies: $(DEPENDENCIES_TARGETS)
clean: $(CLEAN_TARGETS)
	
#define variableRule
# CURDIR := $$(TOPDIR)/$$$(1)
# include $$(CURDIR)/variables.mk
#endef
#$(foreach subdir, $(SUBDIRS), $(eval $(call variableRule, $(subdir))))

# This defines the following for every dir in SUBDIRS:
#   Sets CURDIR to the $(TOPDIR)/$(dir)
#   Includes a makefile in $(CURDIR)/Makefile
define subdirRule
 CURDIR := $$(TOPDIR)/$$$(1)
 $$$(1)/compile: CURDIR := $$(CURDIR)
 $$$(1)/clean: CURDIR := $$(CURDIR)
 $$$(1)/dependencies: CURDIR := $$(CURDIR)
 include $$(CURDIR)/Makefile
endef
# This is what actually does the work.
# The "call" command replaces every $(1) variable reference in subdirRule with $(subdir)
# The "eval" command parses the result of the "call" command as make syntax
$(foreach subdir, $(SUBDIRS), $(eval $(call subdirRule, $(subdir))))
# Reset CURDIR back to what it should be.
CURDIR := $(TOPDIR)

%.d: %.c
	$(DC) -MM $(CPPFLAGS) -MQ $(@:.d=.o) -MQ $@ -MF $*.d $<

%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

%.so:
	$(CC) -shared $(LDFLAGS) -o $@ $(SO_OBJECTS)

# vim:tw=80
