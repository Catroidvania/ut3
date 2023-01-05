/*
header file for main.c
catroidvania 27 12 22
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#define DBNAME "SAVES.db"
#define P1CHAR 'x'
#define P2CHAR 'o'

int waitForConfirm(void);
int slotFull(int);

void initSaveFile(void);
void waitForInput(void);
void displaySaves(void);
void ffgets(char*, int, FILE*);

#endif

