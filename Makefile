# makefile but from the main directory
# catroidvania 27 12 22
# version 1.0

CC = gcc
MAKE = make
SRCDIR = src
SRCMAKE = all 

all : $(SRCMAKE)
	$(MAKE) -C $(SRCDIR) $(SRCMAKE)
