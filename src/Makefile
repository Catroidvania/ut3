# makefile for the ultimate tic tac toe program source files
# catroidvania 27 12 22

OBJS = board.o opponent.o main.o

# in case this is called from src without a CC flag
ifndef CC
CC = gcc
endif

# file delete command for the correct version
ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

# flags if not given - happens if make is not run by ut3/Makefile
ifndef SFLAGS
SFLAGS = -Os -std=c99
endif

# if WINDOWS then define the WINDOWS macro
ifdef WINDOWS
SFLAGS += -D WINDOWS
endif

# if DEBUG mode the define the DEBUG macro
ifdef DEBUG
SFLAGS += -D DEBUG -g -Wall
endif

all : $(OBJS)
	@echo "Compiling sources..."
	@echo "Sources compiled!"

%.o : %.c
	@$(CC) $(SFLAGS) -I../deps -c $<

clean :
	@echo "Removing source object files..."
	@$(CLEAN) $(OBJS)
	@echo "Source object files removed!"

