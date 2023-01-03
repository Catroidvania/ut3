# makefile for the ultimate tic tac toe program dependancies
# catroidvania 27 12 22

CC = gcc
OBJS = sqlite3.o

all : $(OBJS)
	@echo "Compiling dependancies..."
	@echo "Dependancies compiled!"

%.o : %.c
	@$(CC) -c $<

clean :
	@echo "Removing dependancies object files..."
	@rm $(OBJS)
	@echo "Dependancies object files removed!"

