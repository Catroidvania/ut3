/*
header file for board.c
catroidvania 27 12 22
*/

#ifndef BOARD_H
#define BOARD_H

#include "main.h"

#define BOARDEMPTY '.'

struct Coord {
	// Major x, Major y, minor x, minor y
	int Mx, My, mx, my;
};

typedef struct Coord Coord;

struct Major {
	char Minor[3][3];
};

typedef struct Major Major;

struct Game {
	int turn;
	char moverecord[(81 * 4) + 1];
	Major board[3][3];
};

typedef struct Game Game;

Coord coordToBoardIndex(char[4]);

int validMove(Coord, Coord, Game);
int fillScored(Game*);
int gameTied(Game);
int majorFilled(Major);

char gameWon(Game);
char majorScored(Major);
char scorePositions(Major);

void drawBoard(Game);
void initBoard(Game*);
void playToBoard(Coord, Game*, char);
void emptyCoord(Coord*);
void fillMajor(Major*, char);
void recordMove(Coord, Game*, char);
void playRecordToBoard(Game*, Coord*, Coord*, char, char);
void loadSave(int, Game*);
void addSave(int, Game);

#endif


