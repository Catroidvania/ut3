# makefile for files in src
# catroidvania 27 12 22
# version 1.0

CC = gcc
OBJ = board.o opponent.o main.o
FINAL = ut3

all : $(OBJ)
	@echo "Linking object files..."
	$(CC) -o $(FINAL) $^
	@echo "Files linked!"

%.o : %.c
	$(CC) -c $<

clean : $(OBJ)
	@echo "Removing object files..."
	rm $(OBJ)
	@echo "Files removed!"

cleanall : $(OBJ) $(FINAL)
	@echo "Removing objects and executable..."
	rm $(OBJ) $(FINAL)
	@echo "Files removed!"
