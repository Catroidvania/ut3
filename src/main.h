/*
header file for main.c
catroidvania 27 12 22
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

extern char P1CHAR;
extern char P2CHAR;

int waitForConfirm(void);
int slotFull(int);

void initSaveFile(void);
void waitForInput(void);
void ffgets(char*, int, FILE*);

#endif

#ifndef DBNAME
#define DBNAME "SAVES.db"
#endif
