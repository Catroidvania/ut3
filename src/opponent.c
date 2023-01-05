/*
file for computer opponent functions
catroidvania 27 12 22
*/

#include <time.h>
#include <stdlib.h>

#include "main.h"
#include "opponent.h"
#include "board.h"

void initCpu() {
	srand((unsigned int)time(NULL));
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

	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
		minscores[Mx][My] = 0;
		maxscores[Mx][My] = 0;
		copymin[Mx][My] = 0;
	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		if (g.board[Mx][My].Minor[mx][my] != BOARDEMPTY) {
			minscores[Mx][My]++;
			copymin[Mx][My]++;
		}

		if (findmajor && g.board[Mx][My].Minor[mx][my] == P2CHAR) {
			maxscores[Mx][My]++;
		}
	}}}}

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

	printf("%d: %d %d\n", i, cpu.mx, cpu.my);

	return cpu;
}
