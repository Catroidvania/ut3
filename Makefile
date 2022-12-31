# makefile for the ultimate tic tac toe program
# catroidvania 27 12 22

CC = gcc
SRCDIR = src
BUILDDIR = build
_OBJ = board.o opponent.o main.o
OBJ = $(patsubst %,./$(SRCDIR)/%,$(_OBJ))
FINAL = ut3

all : $(OBJ)
	@echo "Linking object files..."
	@mkdir -p $(BUILDDIR)
	@# this method of changing directories feels quite ugly ://
	@cd $(BUILDDIR) && $(CC) -o $(FINAL) $(subst $(SRCDIR),../$(SRCDIR),$^)
	@echo "Files linked!"

%.o : %.c
	@cd $(SRCDIR) && $(CC) -c $(subst $(SRCDIR)/,,$<)

clean : $(OBJ)
	@echo "Removing object files..."
	@rm $(OBJ)
	@echo "Files removed!"

