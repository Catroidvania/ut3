# makefile for the ultimate tic tac toe program dependancies
# catroidvania 27 12 22

#CC = /opt/local/bin/x86_64-w64-mingw32-gcc
CC = gcc

OBJS = sqlite3.o
FLAGS = -Os -g -std=c99

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

all : $(OBJS)
	@echo "Compiling dependancies..."
	@echo "Dependancies compiled!"

%.o : %.c
	@$(CC) $(FLAGS) -c $<

clean :
	@echo "Removing dependancies object files..."
	@$(CLEAN) $(OBJS)
	@echo "Dependancies object files removed!"

