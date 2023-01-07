# makefile for the ultimate tic tac toe program source files
# catroidvania 27 12 22

CC = gcc
OBJS = board.o opponent.o main.o

ifeq ($(OS),Windows_NT)
CLEAN = del
else
CLEAN = rm
endif

all : $(OBJS)
	@echo "Compiling sources..."
	@echo "Sources compiled!"

%.o : %.c
	@$(CC) -I../deps -c $<

clean :
	@echo "Removing source object files..."
	@$(CLEAN) $(OBJS)
	@echo "Source object files removed!"

