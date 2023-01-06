/*
file for computer opponent functions
catroidvania 27 12 22
*/

#include <time.h>
#include <stdlib.h>

#include "main.h"
#include "opponent.h"
#include "board.h"

int countChars(Major m, char match) {
	int mx, my;
	int score = 0;

	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		if (m.Minor[mx][my] == match) {
			score++;
		}
	}}
	
	return score;
}

void initCpu() {
	srand((unsigned int)time(NULL));
}

void dummyMajor(Coord *c, Game g) {
	int Mx, My;

	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
		if (g.board[Mx][My].Minor[c->mx][c->my] == BOARDEMPTY) {
			c->Mx = Mx;
			c->My = My;
			return;
		}
	}}
	c->Mx = -1;
	c->My = -1;
}

Coord randomStrat(Coord lm, Game g) {
	Coord cpu;

	int randmajor = 0;
	
	if (lm.mx < 0 || lm.my < 0) {
		randmajor = 1;
	} else if (majorFilled(g.board[lm.mx][lm.my])) {
		randmajor = 1;
	} else {
		cpu.Mx = lm.mx;
		cpu.My = lm.my;
	}

	do {
		/*
		not truly random but its good enough for now
		*/
		if (randmajor) {
			cpu.Mx = rand() % 3;
			cpu.My = rand() % 3;
		}

		cpu.mx = rand() % 3;
		cpu.my = rand() % 3;
	
	} while (!validMove(cpu, lm, g));

	return cpu;
}

Coord stallStrat(Coord lm, Game g) {
	Coord cpu;

	int minscores[3][3], maxscores[3][3], copymin[3][3];
	int Mx, My, mx, my, plays, i;
	int findmajor = 0;

	if (lm.mx < 0 || lm.my < 0) {
		findmajor = 1;
	} else if (majorFilled(g.board[lm.mx][lm.my])) {
		findmajor = 1;
	} else {
		findmajor = 0;
		cpu.Mx = lm.mx;
		cpu.My = lm.my;
	}

	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		minscores[mx][my] = 9 - countChars(g.board[mx][my], BOARDEMPTY);
		copymin[mx][my] = 9 - countChars(g.board[mx][my], BOARDEMPTY);
		if (findmajor) {
			maxscores[mx][my] = countChars(g.board[mx][my], P2CHAR);
		}
	}}

	do {
		if (findmajor) {
			plays = -1;
			for (mx = 0; mx < 3; mx++) {
			for (my = 0; my < 3; my++) {
				/*
				randomised for a bit of variety
				stops the ai form just playing a1 -> a2 -> a3
				over and over again
				though it isnt much of a strategy
				*/
				if (maxscores[mx][my] > plays ||
				   (rand() % 2 && maxscores[mx][my] == plays)) {
					plays = maxscores[mx][my];
					Mx = mx;
					My = my;
				}
			}}

			/*
			a little bit of variable reuse shenanigans
			*/
			cpu.Mx = Mx;
			cpu.My = My;

			maxscores[Mx][My] = 0;
		}

		i = 0;

		do {
			plays = 10;
			for (mx = 0; mx < 3; mx++) {
			for (my = 0; my < 3; my++) {
				if (minscores[mx][my] < plays ||
				   (rand() % 2 && minscores[mx][my] == plays)) {
					plays = minscores[mx][my];
					Mx = mx;
					My = my;
				}
			}}

			cpu.mx = Mx;
			cpu.my = My;

			minscores[Mx][My] = 9;
			i++;

		} while (!validMove(cpu, lm, g) && i < 9);

		if (findmajor) {
			for (mx = 0; mx < 3; mx++) {
			for (my = 0; my < 3; my++) {
				minscores[mx][my] = copymin[mx][my];
			}}
		}

	} while (!validMove(cpu, lm, g));

	return cpu;
}

Coord loganStrat(Coord lm, Game g) {
	Coord cpu;

	int scores[3][3], selfscores[3][3];
	int Mx, My, mx, my, x, y, plays, findmajor;

	if (lm.mx < 0 || lm.my < 0) {
		findmajor = 1;
	} else if (majorFilled(g.board[lm.mx][lm.my])) {
		findmajor = 1;
	} else {
		findmajor = 0;
		cpu.Mx = lm.mx;
		cpu.My = lm.my;
	}

	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
		scores[Mx][My] = 9 - countChars(g.board[Mx][My], BOARDEMPTY);
		if (findmajor) {
			selfscores[Mx][My] = countChars(g.board[Mx][My], P2CHAR);
		}
	}}

	do {
		x = 1;
		y = 1;

		/*
		corners
		*/
		plays = 10;
		for (Mx = 0; Mx < 3; Mx += 2) {
		for (My = 0; My < 3; My += 2) {
			cpu.mx = Mx;
			cpu.my = My;
			if (findmajor) {
				dummyMajor(&cpu, g);
			}
			if (validMove(cpu, lm, g) &&
			   (scores[Mx][My] < plays ||
			   (rand() % 2 && scores[Mx][My] == plays))) {
				plays = scores[Mx][My];
				x = Mx;
				y = My;
			}
		}}
		
		/* 
		edges 
		*/
		for (Mx = 0; Mx < 2; Mx++) {
		for (My = 0; My < 2; My++) {
			cpu.mx = Mx;
			cpu.my = My;
			if (findmajor) {
				dummyMajor(&cpu, g);
			}
			if (Mx != My && validMove(cpu, lm, g) &&
			   (scores[Mx][My] < plays ||
			   (rand() % 2 && scores[Mx][My] == plays))) {
				plays = scores[Mx][My];
				x = Mx;
				y = My;
			}
		}}
		
		for (Mx = 1; Mx < 3; Mx++) {
		for (My = 1; My < 3; My++) {
			cpu.mx = Mx;
			cpu.my = My;
			if (findmajor) {
				dummyMajor(&cpu, g);
			}
			if (Mx != My && validMove(cpu, lm, g) &&
			   (scores[Mx][My] < plays ||
			   (rand() % 2 && scores[Mx][My] == plays))) {
				plays = scores[Mx][My];
				x = Mx;
				y = My;
			}
		}}

		cpu.mx = x;
		cpu.my = y;
	
		if (findmajor) {

			/* 
			edges 
			*/
			plays = -1;
			for (Mx = 0; Mx < 2; Mx++) {
			for (My = 0; My < 2; My++) {
				cpu.Mx = Mx;
				cpu.My = My;
				if (Mx != My && validMove(cpu, lm, g) &&
				   (selfscores[Mx][My] > plays ||
				   (rand() % 2 && selfscores[Mx][My] == plays))) {
					plays = selfscores[Mx][My];
					x = Mx;
					y = My;
				}
			}}

			for (Mx = 1; Mx < 3; Mx++) {
			for (My = 1; My < 3; My++) {
				cpu.Mx = Mx;
				cpu.My = My;
				if (Mx != My && validMove(cpu, lm, g) &&
				   (selfscores[Mx][My] > plays ||
				   (rand() % 2 && selfscores[Mx][My] == plays))) {
					plays = selfscores[Mx][My];
					x = Mx;
					y = My;
				}
			}}

			/*
			corners
			*/
			for (Mx = 0; Mx < 3; Mx += 2) {
			for (My = 0; My < 3; My += 2) {
				cpu.Mx = Mx;
				cpu.My = My;
				if (validMove(cpu, lm, g) &&
				   (selfscores[Mx][My] > plays ||
				   (rand() % 2 && selfscores[Mx][My] == plays))) {
					plays = selfscores[Mx][My];
					x = Mx;
					y = My;
				}
			}}

			/*
			centre
			*/
			cpu.Mx = 1;
			cpu.My = 1;
			if (validMove(cpu, lm, g)) {
				x = 1;
				y = 1;
			}
			/*
			having it do dummyMajor for minor plays doesnt work as well
			as doing a dummyMinor for major so it can prioritise centre
			over sending to a preferred major :// TODO later ig it works
			now so i dont want to touch it lol
			*/

			cpu.Mx = x;
			cpu.My = y;
		}
	} while (!validMove(cpu, lm, g));

	return cpu;
}
