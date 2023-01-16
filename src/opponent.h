/*
header file for opponent.c
catroidvania 27 12 22
*/

#ifndef BIAS
#define BIAS 1
#endif

#ifndef OPPONENT_H
#define OPPONENT_H

#include "main.h"
#include "board.h"

/*
** monte carlo tree search node
*/
typedef struct Node Node;
struct Node {
	char Turn;

	double Wins;
	double Visits;
	
	Game State;
	
	Node *Parent;
	Node *Children[81];
};

void initCpu(void);
void initNode(Node*, Node*, int);
void dummyMinor(Coord*, Game);
void freeTree(Node*, int branch);
void backpropagate(Node*, int);

int simulate(Node*);
int scoresEmpty(int*);
int countChars(Major, char);
int coordToNumber(Coord);

Node* selection(Node*, Coord);

Coord numberToCoord(int);
Coord randomStrat(Coord, Game);
Coord stallStrat(Coord, Game);
Coord loganStrat(Coord, Game);
Coord smartStrat(Coord, Game, int);

#endif
