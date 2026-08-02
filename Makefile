#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = /Users/christian/GB/gbdk/
endif

LCC = $(GBDK_HOME)/bin/lcc

# include headers in /src
LCCFLAGS += -Isrc -Ires -DTARGET_GB
CLIFLAGS += -DTARGET_CLI


# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif

PROJECTNAME    = NUMBLOCKS

# source directories
COREDIR      := src/core
GBDIR	    := src/gb
CLIDIR	    := src/cli
RESDIR      := res
TESTDIR := test

# object output directories
GBOBJDIR      := build/gb
BINS	    := $(GBOBJDIR)/$(PROJECTNAME).gb

# source files
CORESOURCES := $(wildcard $(COREDIR)/*.c)
GBSOURCES   := $(wildcard $(GBDIR)/*.c)
CLISOURCES  := $(wildcard $(CLIDIR)/*.c)
RESSOURCES  := $(wildcard $(RESDIR)/*.c)
TESTSOURCES := $(wildcard $(TESTDIR)/*.c) $(filter-out $(CLIDIR)/main.c, $(wildcard $(CLIDIR)/*.c))

ASMSOURCES := $(wildcard $(COREDIR)/*.s) \
              $(wildcard $(GBDIR)/*.s)

# object files
GBSOURCES_ALL := $(CORESOURCES) $(GBSOURCES) $(RESSOURCES)

GBOBJS := $(GBSOURCES_ALL:%.c=$(GBOBJDIR)/%.o) \
          $(ASMSOURCES:%.s=$(GBOBJDIR)/%.o)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat


.PHONY: all rom cli test clean

all: rom

rom: $(BINS)

GBOBJS := $(GBSOURCES_ALL:%.c=$(GBOBJDIR)/%.o) \
          $(ASMSOURCES:%.s=$(GBOBJDIR)/%.o)

$(BINS): $(GBOBJS)
	$(LCC) $(LCCFLAGS) -o $@ $^

$(GBOBJDIR)/%.o: %.c
	mkdir -p $(@D)
	$(LCC) $(LCCFLAGS) -c -o $@ $<

cli:
	mkdir -p build/cli
	gcc -g -O0 $(CORESOURCES) $(CLISOURCES) $(CLIFLAGS) -Isrc -o build/cli/$(PROJECTNAME)

test:
	mkdir -p build/test
	gcc -g -O0 $(CORESOURCES) $(TESTSOURCES) -Isrc -o build/test/test

testrun:
	mkdir -p build/test
	gcc -g -O0 $(CORESOURCES) $(TESTSOURCES) -Isrc -o build/test/test
	./build/test/test

clean:
	rm -rf build