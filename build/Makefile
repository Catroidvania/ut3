# makefile for the ultimate tic tac toe program final build
# catroidvania 27 12 22

SRCDIR = src
DEPDIR = deps

SRCS = board.o opponent.o main.o
DEPS = sqlite3.o

FINAL = ut3
SAVES = SAVES.db

# in case this is called from deps without a CC flag
ifndef CC
CC = gcc
endif

# flags if not given - happens if make is not run by ut3/Makefile
ifndef BFLAGS
BFLAGS = -Os -std=c99
endif

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

# if WINDOWS then define the WINDOWS macro
ifdef WINDOWS
BFLAGS += -D WINDOWS
endif

# if DEBUG mode the define the DEBUG macro
ifdef DEBUG
BFLAGS += -D DEBUG -g -Wall
endif

all : $(patsubst %,../$(SRCDIR)/%,$(SRCS)) $(patsubst %,../$(DEPDIR)/%,$(DEPS))
	@echo "Building..."
	@$(CC) $(BFLAGS) -o $(FINAL) $^
	@echo "Build complete!"

clean :
	@echo "Removing build..."
	@$(CLEAN) $(FINAL) $(SAVES)
	@echo "Build removed!"

