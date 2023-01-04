# makefile for the ultimate tic tac toe program
# catroidvania 27 12 22

BUILDDIR = build
SRCDIR = src
DEPDIR = deps
MAKEFILES = src/Makefile deps/Makefile

ifeq ($(OS),Windows_NT)
MAKE = mingw32-make
else
MAKE = make
endif

all : objs
	@echo "Locating build Makefile..."
	@$(MAKE) -C $(BUILDDIR)

objs : $(MAKEFILES)
	@echo "Locating source Makefile..."
	@$(MAKE) -C $(SRCDIR)
	@echo "Locating dependancies Makefile..."
	@$(MAKE) -C $(DEPDIR)

clean : $(MAKEFILES)
	@echo "Locating source Makefile..."
	@$(MAKE) -C $(SRCDIR) clean
	@echo "Locating dependancies Makefile..."
	@$(MAKE) -C $(DEPDIR) clean
	@echo "Locating build Makefile..."
	@$(MAKE) -C $(BUILDDIR) clean

