# makefile for the ultimate tic tac toe program source files
# catroidvania 27 12 22

CC = gcc
OBJS = board.o opponent.o main.o

all : $(OBJS)
	@echo "Compiling sources..."
	@echo "Sources compiled!"

%.o : %.c
	@$(CC) -c $<

clean :
	@echo "Removing source object files..."
	@rm $(OBJS)
	@echo "Source object files removed!"

