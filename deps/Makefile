# makefile for the ultimate tic tac toe program dependancies
# catroidvania 27 12 22

CC = gcc
OBJS = sqlite3.o

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

all : $(OBJS)
	@echo "Compiling dependancies..."
	@echo "Dependancies compiled!"

%.o : %.c
	@$(CC) -c $<

clean :
	@echo "Removing dependancies object files..."
	@$(CLEAN) $(OBJS)
	@echo "Dependancies object files removed!"

