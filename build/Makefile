# makefile for the ultimate tic tac toe program final build
# catroidvania 27 12 22

#CC = /opt/local/bin/x86_64-w64-mingw32-gcc
CC = gcc

SRCDIR = src
DEPDIR = deps

SRCS = board.o opponent.o main.o
DEPS = sqlite3.o

FLAGS = -Os -g -std=c99
FINAL = ut3
SAVES = SAVES.db

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

all : $(patsubst %,../$(SRCDIR)/%,$(SRCS)) $(patsubst %,../$(DEPDIR)/%,$(DEPS))
	@echo "Building..."
	@$(CC) $(FLAGS) -o $(FINAL) $^
	@echo "Build complete!"

clean :
	@echo "Removing build..."
	@$(CLEAN) $(FINAL) $(SAVES)
	@echo "Build removed!"

