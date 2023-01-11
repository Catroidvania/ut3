# makefile for the ultimate tic tac toe program source files
# catroidvania 27 12 22

#CC = /opt/local/bin/x86_64-w64-mingw32-gcc
CC = gcc

OBJS = board.o opponent.o main.o
FLAGS = -Os -g -std=c99

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

all : $(OBJS)
	@echo "Compiling sources..."
	@echo "Sources compiled!"

%.o : %.c
	@$(CC) $(FLAGS) -I../deps -c $<

clean :
	@echo "Removing source object files..."
	@$(CLEAN) $(OBJS)
	@echo "Source object files removed!"

