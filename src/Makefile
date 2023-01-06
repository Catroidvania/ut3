# makefile for the ultimate tic tac toe program source files
# catroidvania 27 12 22

CC = gcc
DEPSDIR = ../deps
OBJS = board.o opponent.o main.o

all : $(OBJS)
	@echo "Compiling sources..."
	@echo "Sources compiled!"

%.o : %.c
	@$(CC) -I{DEPSDIR} -c $<

clean :
	@echo "Removing source object files..."
	@rm $(OBJS)
	@echo "Source object files removed!"

