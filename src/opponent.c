/*
file for computer opponent functions
catroidvania 27 12 22
*/

#include <stdlib.h>		/* for random number generation						*/
#include <time.h>		/* for seeding the rand function					*/
#include <math.h>		/* for rounding										*/

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

void dummyMinor(Coord *c, Game g) {
	int Mx, My;

	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
		if (g.board[c->Mx][c->My].Minor[Mx][My] == BOARDEMPTY) {
			c->mx = Mx;
			c->my = My;
			return;
		}
	}}

	c->mx = -1;
	c->my = -1;
}

void initNode(Node *n, Node *p, int branch) {
	int c;

	n->Wins = 0.0;
	n->Visits = 0.0;
	n->Parent = p;

	if (p != NULL) {
		n->State = p->State;
		n->Turn = p->Turn == P1CHAR ? P2CHAR : P1CHAR;
		p->Children[branch] = n;
	}
	
	for (c = 0; c < 81; c++) {
		n->Children[c] = NULL;
	} 
}

void backpropagate(Node *n, int wins) {
	n->Wins += wins;
	n->Visits += 1;

	if (n->Parent != NULL) {
		backpropagate(n->Parent, wins);
	}
}

void freeTree(Node *n, int branch) {
	int c;

	for (c = 0; c < 81; c++) {
		if (n->Children[c] != NULL) {
			printf("down ");
			freeTree(n->Children[c], c);
		}
	}
	
	if (n->Parent != NULL) {
		printf("parent ");
		n->Parent->Children[branch] = NULL;
	}

#ifdef DEBUG
	printf("[%f/%f] ", n->Wins, n->Visits);
#endif

	free(n);
}

int simulate(Node *n) {
	Game g = n->State;
	Coord move, lm;

	int c;

	char turn = n->Turn;

	for (c = 0; c < 81; c++) {
		if (n->Parent->Children[c] == n) {
			lm = numberToCoord(c);
			break;
		}
	}
	
	while (gameWon(g) == BOARDEMPTY) {
		move = randomStrat(lm, g);

		playToBoard(move, &g, turn);
		
		if (fillScored(&g)) {
			emptyCoord(&move, -1);
		}

		if (gameTied(g)) {
			break;
		}

		lm = move;

		turn = turn == P1CHAR ? P2CHAR : P1CHAR;
	}

	if (gameWon(g) == P2CHAR) {
		return 1;
	}

	return 0;
}

Coord numberToCoord(int i) {
	Coord move;
	int Mx, My, mx, my;
	int sqr = 0;

	for (My = 0; My < 3; My++) {
	for (Mx = 0; Mx < 3; Mx++) {
	for (my = 0; my < 3; my++) {
	for (mx = 0; mx < 3; mx++) {
		if (sqr == i) {
			move.Mx = Mx;
			move.My = My;
			move.mx = mx;
			move.my = my;
			return move;
		}
		sqr++;
	}}}}

	emptyCoord(&move, -1);
	return move;
}

/*
** find the lowermost node from on the most promising branch
*/
Node* selection(Node *n, Coord lm) {
	Node *leaf;
	Coord move;

	int c, selected;
	int full = 1, scope = 0;

	double uct;
	double uctmax = 0;

	if (n == NULL) {
		return n;
	}

	if (gameWon(n->State) != BOARDEMPTY) {
		return n;
	}

	if (lm.mx < 0 || lm.my < 0 || majorFilled(n->State.board[lm.mx][lm.my])) {
		for (c = 0; c < 81; c++) {
			if (n->Children[c] == NULL) {
				full = 0;
				uct = 999990.0 + (rand() % 9);
			} else {
				uct = round((n->Children[c]->Wins/n->Children[c]->Visits)
				+ (BIAS * sqrt(log(n->Visits)/n->Children[c]->Visits)));
			}
				
			if (uct > uctmax && validMove(numberToCoord(c), lm, n->State)) {
				move = numberToCoord(c);
				selected = c;
				uctmax = uct;
			}
		}
		
		if (full) {
			leaf = selection(n->Children[selected], move);
		} else {
			leaf = malloc(sizeof(*leaf));

			if (leaf != NULL) {
				initNode(leaf, n, selected);
				playToBoard(move, &leaf->State, leaf->Turn);
				fillScored(&leaf->State);
			} else {
				printf("\nNot enough heap memory to create new node!\n");
				leaf = n;
			}
		}
	} else	{
		scope = (lm.mx * 9) + (lm.my * 27);
		for (c = scope; c < scope + 10; c++) {
			if (n->Children[c] == NULL) {
				full = 0;
				uct = 999990.0 + (rand() % 9);
			} else {
				uct = round((n->Children[c]->Wins/n->Children[c]->Visits)
				+ (BIAS * sqrt(log(n->Visits)/n->Children[c]->Visits)));
			}
				
			if (uct > uctmax && validMove(numberToCoord(c), lm, n->State)) {
				move = numberToCoord(c);
				selected = c;
				uctmax = uct;
			}
		}
		
		if (full) {
			leaf = selection(n->Children[selected], move);
		} else {
			leaf = malloc(sizeof(*leaf));

			if (leaf != NULL) {
				initNode(leaf, n, selected);
				playToBoard(move, &leaf->State, leaf->Turn);
				fillScored(&leaf->State);
			} else {
				printf("\nNot enough heap memory to create new node!\n");
				leaf = n;
			}
		}
	}

	return leaf;
}

Coord smartStrat(Coord lm, Game g, int limit) {
	Node* root = malloc(sizeof(*root));
	Node* leaf;
	Node* rootcopy[81];
	Coord cpu;
	
	int cycle, sims, wins;

	double winrate, winmax;

	emptyCoord(&cpu, -1);
	initNode(root, NULL, 0);

	root->Turn = P2CHAR;
	root->State = g;

	/*
	** keep trying nodes until limit is reached
	*/
	for (cycle = 0; cycle < limit; cycle++) {
		/*
		** select an unevaluated node
		*/
		leaf = selection(root, lm);
		if (leaf == NULL) {
			printf("Could not allocate memory!\n");
			freeTree(root, 0);
			return randomStrat(lm, g);
		}
		for (sims = 0; sims < 18; sims++) {
			wins = simulate(leaf);
			backpropagate(leaf, wins);
		}		
	}

	for (sims = 0; sims < 81; sims++) {
		rootcopy[sims] = NULL;
		if (root->Children[sims] != NULL) {
			rootcopy[sims] = root->Children[sims];
		}
	}

	do {
		winmax = 0.0;
		wins = 0;
		for (sims = 0; sims < 81; sims++) {
			if (rootcopy[sims] == NULL) {
				continue;
			}
			winrate = rootcopy[sims]->Wins / rootcopy[sims]->Visits; 
			if (winrate > winmax) {
				wins = sims;
				winmax = winrate;
			}
		}
		
		cpu = numberToCoord(wins);

		rootcopy[sims] = NULL;
	} while (!validMove(cpu, lm, g));

	freeTree(root, 0);

	return cpu;
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
	int Mx, My, x, y, plays, findmajor;

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
		if (findmajor) {

			/* 
			edges 
			*/
			plays = -1;
			for (Mx = 0; Mx < 2; Mx++) {
			for (My = 0; My < 2; My++) {
				cpu.Mx = Mx;
				cpu.My = My;
				dummyMinor(&cpu, g);
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
				dummyMinor(&cpu, g);
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
				dummyMinor(&cpu, g);
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
			dummyMinor(&cpu, g);
			if (validMove(cpu, lm, g)) {
				x = 1;
				y = 1;
			}

			cpu.Mx = x;
			cpu.My = y;
		}
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
	
	} while (!validMove(cpu, lm, g));

	return cpu;
}

