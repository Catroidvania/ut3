/*****************************************************************************
**
** opponent.c
** for computer opponent strategies
** created by catroidvania dec 27 2022
**
******************************************************************************
*/

#include <stdlib.h>		/* for random number generation						*/
#include <time.h>		/* for seeding the rand function					*/
#include <math.h>		/* for rounding	and pythagoras						*/

#include "main.h"
#include "opponent.h"
#include "board.h"

/*
** counts the amount of a given char in a Major
*/
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

/*
** for any cpu related processess that only need to be called once
*/
void initCpu() {
	
	/*
	** is only seeding the random number generator at the moment
	*/
	srand((unsigned int)time(NULL));
}

/*
** finds a valid minor coordinate for a given major
*/
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

	/*
	** so we can check when no valid coordinate is found
	*/
	c->mx = -1;
	c->my = -1;
}

/*
** gets a random ish move
*/
Coord randomStrat(Coord lm, Game g) {
	Coord cpu;

	int randmajor = 0;
	
	/*
	** check if we need to find the major coordinate as well
	*/
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
		** not truly random but its good enough for now
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

/*
** try and send to the least played in Major
*/
Coord stallStrat(Coord lm, Game g) {
	Coord cpu;

	int minscores[3][3], maxscores[3][3], copymin[3][3];
	int Mx, My, mx, my, plays, i;
	int findmajor = 0;

	/*
	** if the major coord needs to be found
	*/
	if (lm.mx < 0 || lm.my < 0) {
		findmajor = 1;
	} else if (majorFilled(g.board[lm.mx][lm.my])) {
		findmajor = 1;
	} else {
		findmajor = 0;
		cpu.Mx = lm.mx;
		cpu.My = lm.my;
	}

	/*
	** score each Major by the amount of non BOARDEMPTY character in it
	*/
	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		minscores[mx][my] = 9 - countChars(g.board[mx][my], BOARDEMPTY);
		copymin[mx][my] = 9 - countChars(g.board[mx][my], BOARDEMPTY);
		if (findmajor) {
			/*
			** if allowed to play anywhere it chooses its the board it
			** dominates the most in
			*/
			maxscores[mx][my] = countChars(g.board[mx][my], P2CHAR);
		}
	}}

	/*
	** tries moves in order of lowest scores
	*/
	do {
		if (findmajor) {
			plays = -1;
			for (mx = 0; mx < 3; mx++) {
			for (my = 0; my < 3; my++) {
				/*
				** randomised for a bit of variety
				** stops the ai form just playing a1 -> a2 -> a3
				** over and over again
				** though it isnt much of a strategy
				*/
				if (maxscores[mx][my] > plays ||
				   (rand() % 2 && maxscores[mx][my] == plays)) {
					plays = maxscores[mx][my];
					Mx = mx;
					My = my;
				}
			}}

			/*
			** a little bit of variable reuse shenanigans
			*/
			cpu.Mx = Mx;
			cpu.My = My;

			/*
			** eliminates the contender in case it is not valid
			*/
			maxscores[Mx][My] = 0;
		}

		i = 0;
		/*
		** repeat but for the minor coord
		*/
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
		/*
		** in case we are finding the major we need to copy back the minor
		** scores for each major we try
		*/
		if (findmajor) {
			for (mx = 0; mx < 3; mx++) {
			for (my = 0; my < 3; my++) {
				minscores[mx][my] = copymin[mx][my];
			}}
		}

	} while (!validMove(cpu, lm, g));

	return cpu;
}

/*
** based on the strategy logan gave me
*/
Coord loganStrat(Coord lm, Game g) {
	Coord cpu;

	int scores[3][3], selfscores[3][3];
	int Mx, My, x, y, plays, findmajor;

	/*
	** check whether the major coord needs to be found
	*/
	if (lm.mx < 0 || lm.my < 0) {
		findmajor = 1;
	} else if (majorFilled(g.board[lm.mx][lm.my])) {
		findmajor = 1;
	} else {
		findmajor = 0;
		cpu.Mx = lm.mx;
		cpu.My = lm.my;
	}

	/*
	** plays like stall where it scores each major and plays in the least full
	*/
	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
		scores[Mx][My] = 9 - countChars(g.board[Mx][My], BOARDEMPTY);
		if (findmajor) {
			selfscores[Mx][My] = countChars(g.board[Mx][My], P2CHAR);
		}
	}}

	do {
		/*
		** but applies a preference to the squares it wants to play in
		** for the major it wants centre most corners second edges last
		*/
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

		/*
		** has a different preference for playing the minor coord
		** it prioritises edges > corners > centre
		*/
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

/*
** section for all the unused functions
*/
#ifdef DEBUG

/*
** unused - initialises all values in a node struct
*/
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

/*
** unused - update the visit / win values of a given nodes branch
*/
void backpropagate(Node *n, int wins) {
	n->Wins += wins;
	n->Visits += 1;

	if (n->Parent != NULL) {
		backpropagate(n->Parent, wins);
	}
}

/*
** unused - frees all the memory allocated for nodes
*/
void freeTree(Node *n, int branch) {
	int c;

	/*
	** if the node has any children try and free those first
	*/
	for (c = 0; c < 81; c++) {
		if (n->Children[c] != NULL) {
			freeTree(n->Children[c], c);
		}
	}
	
	/*
	** removes the pointer from its parent unless it is the root node
	*/
	if (n->Parent != NULL) {
		n->Parent->Children[branch] = NULL;
	}

	/*
	** then deallocate itself now that nothing points to it
	*/
	free(n);
}

/*
** unused - simulates a game of only random moves until the end
*/
int simulate(Node *n) {
	Game g = n->State;
	Coord move, lm;

	int c;

	char turn = n->Turn;

	/*
	** get the last move
	*/
	for (c = 0; c < 81; c++) {
		if (n->Parent->Children[c] == n) {
			lm = numberToCoord(c);
			break;
		}
	}
	
	/*
	** simulate the game until there is a winner
	*/
	while (gameWon(g) == BOARDEMPTY) {
		move = randomStrat(lm, g);

		playToBoard(move, &g, turn);
		
		if (fillScored(&g)) {
			emptyCoord(&move, -1);
		}

		/*
		** or until the board is completely full
		*/
		if (gameTied(g)) {
			break;
		}

		lm = move;

		turn = turn == P1CHAR ? P2CHAR : P1CHAR;
	}

	/*
	** return an int representing a win loss or tie for backpropagation
	*/
	if (gameWon(g) == P2CHAR) {
		return 1;
	}

	return 0;
}

/*
** unused - turns a value from 0 - 80 to a board coordinate
*/
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
** unused - find the lowermost node from on the most promising branch
*/
Node* selection(Node *n, Coord lm) {
	Node *leaf;
	Coord move;

	int c;
	int selected = 0;
	int full = 1, scope = 0;

	double uct;
	double uctmax = 0;

	/*
	** if we get passed a NULL pointer, or the node is terminal
	** though if were getting NULL pointer being passed its already too late
	*/
	if (n == NULL || gameWon(n->State) != BOARDEMPTY) {
		return n;
	}

	/*
	** check for valid children from the entire board if can play anywhere
	*/
	if (lm.mx < 0 || lm.my < 0 || majorFilled(n->State.board[lm.mx][lm.my])) {
		for (c = 0; c < 81; c++) {
			/*
			** prioritise unvisited nodes
			*/
			if (n->Children[c] == NULL) {
				full = 0;
				/*
				** randomness to keep things interesting
				*/
				uct = 999990.0 + (rand() % 9);
			} else {
				/*
				** upper confidance bounds formula for the 
				** exploration - exploitation balance when selecting children
				*/
				uct = round((n->Children[c]->Wins/n->Children[c]->Visits)
				+ (BIAS * sqrt(log(n->Visits)/n->Children[c]->Visits)));
			}
			
			/*
			** for finding the highest uct value
			*/
			if (uct > uctmax && validMove(numberToCoord(c), lm, n->State)) {
				move = numberToCoord(c);
				selected = c;
				uctmax = uc
				;
			}
		}
		
		/*
		** keep selecting until a terminal node is found
		*/
		if (full) {
			leaf = selection(n->Children[selected], move);
		} else {
			/*
			** or create a new child
			*/
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
	/*
	** version of above but accounting for send rules
	*/
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

	/*
	** returns the best leaf node found
	*/
	return leaf;
}

/*
** unused - test strategy using monte carlo tree search to find moves
*/
Coord smartStrat(Coord lm, Game g, int limit) {
/*
** only availible if compiled with DEBUG set
** WARNING THIS IS INCOMPLETE AND CAUSES MEMORY LEAKS SO USE AT YOUR OWN RISK
*/

	/*
	** allocate memory for the root node and a copy of the root node
	*/
	Node* root = malloc(sizeof(*root));
	Node* rootcopy = malloc(sizeof(*rootcopy));
	Node* leaf;
	Coord cpu;
	
	int cycle, sims, wins;

	double winrate, winmax;

	emptyCoord(&cpu, -1);

	/*
	** initialises both root nodes so pointers at least point to NULL
	*/
	initNode(root, NULL, 0);
	initNode(rootcopy, NULL, 0);

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
		/*
		** then simulate and backpropagate results a hardcoded amount of times
		*/
		for (sims = 0; sims < 18; sims++) {
			wins = simulate(leaf);
			backpropagate(leaf, wins);
		}		
	}

	/*
	** copy over the contents of root to rootcopy so we can eleminate branches
	** when searhing for the most promising one without orphaning memory
	*/
	*rootcopy = *root;
	
	/*
	** find the most promising branch that can be played
	*/
	do {
		winmax = 0.0;
		wins = 0;
		for (sims = 0; sims < 81; sims++) {
			if (rootcopy->Children[sims] == NULL) {
				continue;
			}
			winrate = rootcopy->Children[sims]->Wins /
					  rootcopy->Children[sims]->Visits; 
			if (winrate > winmax) {
				wins = sims;
				winmax = winrate;
			}
		}
		
		cpu = numberToCoord(wins);

		/*
		** then eliminate the branch from contention
		*/
		rootcopy->Children[wins] = NULL;

	} while (!validMove(cpu, lm, g));

	/*
	** then free all memory allocated - freeTree frees the root node
	** this would idealy prevent memory leaks but the program tends to
	** segfault before it reaches this point and calling the function
	** from gdb is sometimes either too late or the data is too corrupted
	** for that to have any difference but does free all nodes when allowed to
	*/
	freeTree(root, 0);
	free(rootcopy);

	return cpu;
}

#endif
