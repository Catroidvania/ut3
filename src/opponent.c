/*
file for computer opponent functions
catroidvania 27 12 22
*/

#include <time.h>
#include <stdlib.h>

#include "opponent.h"
#include "board.h"

void initCpuRandom() {
	srand((unsigned int)time(NULL));
}

Coord randomStrat(Coord lastmove, Game g) {
	Coord cpumove;
	int randmajor = 0;
	
	if (lastmove.mx < 0 || lastmove.my < 0) {
		randmajor = 1;
	} else if (majorScored(g.board[lastmove.mx][lastmove.my]) != BOARDEMPTY) {
		randmajor = 1;
	} else {
		cpumove.Mx = lastmove.mx;
		cpumove.My = lastmove.my;
	}

	do {
		/*
		not truly random but its good enough for now
		*/
		if (randmajor) {
			cpumove.Mx = rand() % 3;
			cpumove.My = rand() % 3;
		}

		cpumove.mx = rand() % 3;
		cpumove.my = rand() % 3;
	
	} while (!validMove(cpumove, lastmove, g));

	return cpumove;

}
