# makefile for the ultimate tic tac toe program final build
# catroidvania 27 12 22

CC = gcc

SRCDIR = src
DEPDIR = deps

SRCS = board.o opponent.o main.o
DEPS = sqlite3.o

FINAL = ut3
SAVES = SAVES.db

all : $(patsubst %,../$(SRCDIR)/%,$(SRCS)) $(patsubst %,../$(DEPDIR)/%,$(DEPS))
	@echo "Building..."
	@$(CC) -o $(FINAL) $^
	@echo "Build complete!"

clean :
	@echo "Removing build..."
	@rm $(FINAL) $(SAVES)
	@echo "Build removed!"

