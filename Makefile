# makefile for the ultimate tic tac toe program
# catroidvania 27 12 22

# for personal cross compilation - not for use
#/opt/local/bin/x86_64-w64-mingw32-gcc

# variables
BUILDDIR = build
SRCDIR = src
DEPDIR = deps
MAKEFILES = src/Makefile deps/Makefile

# if there is not a make command provided
ifndef MAKE
# find the respective one for the current system
ifeq ($(OS),Windows_NT)
MAKE = mingw32-make
else
MAKE = make
endif
endif

# compiler flags
# global flags - also the default flags if none given
ifndef GFLAGS
GFLAGS = -Os -std=c99
else
endif
# source file flags
ifndef SFLAGS
SFLAGS = $(GFLAGS)
endif
# dependancies flags
ifndef DFLAGS
DFLAGS = $(GFLAGS)
endif
# build flags
ifndef BFLAGS
BFLAGS = $(GFLAGS)
endif

# for compiling with windows specific features
ifndef WINDOWS
# default false
WINDOWS =
endif

# for compiling with debuging features - not always used
ifndef DEBUG
DEBUG =
endif

# specify a differnt compiler
ifndef CC
CC = gcc
endif

# export variables needed in submakefiles
export GFLAGS SFLAGS DFLAGS BFLAGS WINDOWS DEBUG CC

all : objs
	@echo "Locating build Makefile..."
	@$(MAKE) -C $(BUILDDIR) 

objs : $(MAKEFILES)
	@echo "Locating source Makefile..."
	@$(MAKE) -C $(SRCDIR) 
	@echo "Locating dependancies Makefile..."
	@$(MAKE) -C $(DEPDIR)


clean : $(MAKEFILES)
	@echo "Locating source Makefile..."
	@$(MAKE) -C $(SRCDIR) clean
	@echo "Locating dependancies Makefile..."
	@$(MAKE) -C $(DEPDIR) clean
	@echo "Locating build Makefile..."
	@$(MAKE) -C $(BUILDDIR) clean

