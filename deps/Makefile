# makefile for the ultimate tic tac toe program dependancies
# catroidvania 27 12 22

OBJS = sqlite3.o

# in case this is called from deps without a CC flag
ifndef CC
CC = gcc
endif

# flags if not given - happens if make is not run by ut3/Makefile
ifndef DFLAGS
DFLAGS = -Os -std=c99
endif

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

# if WINDOWS then define the WINDOWS macro
ifdef WINDOWS
DFLAGS += -D WINDOWS
endif

# if DEBUG mode the define the DEBUG macro
ifdef DEBUG
DFLAGS += -D DEBUG -g -Wall
endif

all : $(OBJS)
	@echo "Compiling dependancies..."
	@echo "Dependancies compiled!"

%.o : %.c
	@$(CC) $(DFLAGS) -c $<

clean :
	@echo "Removing dependancies object files..."
	@$(CLEAN) $(OBJS)
	@echo "Dependancies object files removed!"

