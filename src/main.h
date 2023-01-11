/**
** header file for main.c
** created by catroidvania dec 27 2022
*/

/**
** prevents main.h from being declared more than once / recursivly declared
*/
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

/**
** macro for the name of the save file
** this is not expected nor needed to change so it is a constant
*/
#define DBNAME "SAVES.db"

/**
** macro for player representation characters
** may change to an extern char at some point for customisation but at the
** moment is just a macro
*/
#define P1CHAR 'x'
#define P2CHAR 'o'

/**
** pauses the program execution and gets a yes / no from the user
** takes no arguments and displays `(y/n):' then waits for user input
** returns 1 if a `y' is given or a 0 if any other character is given
*/
int waitForConfirm(void);

/**
** checks if a save slot is filled in the database
** int slot	is an integer corrsponding to the database id to check
** returns 1 if the column record is not null or 0 if it is empty
*/
int slotFull(int);

/**
** creates a save database if one does not exist
** takes no arguments and returns nothing but creates file DBNAME if it
** doesnt already exist and adds the table saves to it
*/
void initSaveFile(void);

/**
** pauses the program execution until a newline is sent
** takes no arguments and returns nothing
** it only displays `Press enter to continue...' and wait for a newline
** to be sent
*/
void waitForInput(void);

/**
** simple select operation from the database file
** takes no arguments and returns nothing
** selects all from saves and displays them to the user
*/
void displaySaves(void);

/**
** reads an amount of characters and discards the rest of the line
** char* buf 	is a pointer to a char array that any read chars are stored in
** int bufsize	is how many chars to store into buf 
**				should not exceed the size of buf
** FILE* stream the fileno to read from although intended to be used on stdin
** reads bufsize amount of characters from stream and stores them into buf
** then continues reading and discarding characters until it encounters a
** newline or end of file
** used to avoid characters left hanging in the buffer affecting user input
*/
void ffgets(char*, int, FILE*);

#endif

