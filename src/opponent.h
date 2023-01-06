/*
header file for opponent.c
catroidvania 27 12 22
*/

#ifndef OPPONENT_H
#define OPPONENT_H

#include "main.h"
#include "board.h"

void initCpu(void);
void dummyMinor(Coord*, Game);

int scoresEmpty(int*);
int countChars(Major, char);

Coord randomStrat(Coord, Game);
Coord stallStrat(Coord, Game);
Coord loganStrat(Coord, Game);

#endif
