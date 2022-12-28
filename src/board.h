/*
header file for board.c
catroidvania 27 12 22
version 1.0
*/

#ifndef BOARD_H

#include "main.h"

#define BOARD_H

#define DISPLAYWIDTH 80
#define DISPLAYHEIGHT 24

/*
arrays for game board and output buffer
*/

extern char board[3][3][3][3];
extern char buffer[DISPLAYHEIGHT][DISPLAYWIDTH];

void drawBuffer(int width, int height, char[height][width]);

#endif
