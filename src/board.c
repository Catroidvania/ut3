/*****************************************************************************
**
** board.c
** file for board related functions
** created by catroidvania dec 27 2022
**
******************************************************************************
*/

#include <stdio.h>		/* for io operations								*/
#include <stdlib.h>		/* for random number generation						*/
#include <math.h>		/* for square root and absolute value*/

#ifdef WINDOWS			/* if windows macro is set at compile time			*/
#include <windows.h>	/* for animating explosions on windows				*/
#else
#include <time.h>		/* for animating explosions	on unix					*/
#endif

#include "sqlite3.h"	/* for database operations							*/

#include "board.h"
#include "main.h"

/*
** turns a text coordinate to a coord struct
*/
Coord coordToBoardIndex(char coord[4]) {
	Coord c;

	/*
	** subtracts values corresponding to each chars ascii values
	** with a being 97 and 1 being 49 which sets it to valid board indexes
	*/
	c.Mx = (int)coord[0] - 97;
	c.My = (int)coord[1] - 49;
	c.mx = (int)coord[2] - 97;
	c.my = (int)coord[3] - 49;

	return c;
}

/*
** checks if a move is legal
*/
int validMove(Coord c, Coord lc, Game g) {
	/*
	** makes sure its even able to be used as a board index
	*/
	if (c.Mx < 0 || c.Mx > 2 ||
		c.My < 0 || c.My > 2 ||
		c.mx < 0 || c.mx > 2 ||
		c.my < 0 || c.my > 2) {
		return 0;
	}

	/*
	** check each valid move case
	** starting with whether the board square is empty to be played in
	*/
	if (g.board[c.Mx][c.My].Minor[c.mx][c.my] == BOARDEMPTY) {
		/*
		** check that the major played in is the one that was sent to
		*/
		if (c.Mx == lc.mx && c.My == lc.my) {
			return 1;
		/*
		** valid to go any major if sent to a filled major
		*/
		} else if (majorFilled(g.board[lc.mx][lc.my])) {
			return 1;
		/*
		** also valid to go anywhere if last move scored
		*/
		} else if (lc.mx < 0 || lc.my < 0) {
			return 1;
		}
	}

	/*
	** move is not legal otherwise
	*/
	return 0;
}

/*
** if any majors are scored then fill them with the scorees tile
*/
int fillScored(Game *g) {
	int majorx, majory; 
	int scored = 0;
	char tile;

	for (majorx = 0; majorx < 3; majorx++) {
	for (majory = 0; majory < 3; majory++) {
		tile = scorePositions(g->board[majorx][majory]);
		if (tile != BOARDEMPTY) {
			if (majorScored(g->board[majorx][majory]) == BOARDEMPTY) {
				fillMajor(&g->board[majorx][majory], tile);
				scored = 1;
			}
		}
	}}

	return scored;
}

/*
** check if a the board is filled and no legal moves can be played
*/
int gameTied(Game g) {
	int Mx, My, mx, my;

	for (Mx = 0; Mx < 3; Mx++) {
	for (My = 0; My < 3; My++) {
	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		if (g.board[Mx][My].Minor[mx][my] == BOARDEMPTY) {
			return 0;
		}
	}}}}

	return 1;
}

/*
** checks if a major is filled and cannot be played in
*/
int majorFilled(Major m) {
	int mx, my;
	
	for (mx = 0; mx < 3; mx++) {
	for (my = 0; my < 3; my++) {
		if (m.Minor[mx][my] == BOARDEMPTY) {
			return 0;
		}
	}}

	return 1;
}

/*
** checks specifically if a major has been filled from scoring
*/
char majorScored(Major m) {
	int minorx, minory;
	char c = m.Minor[0][0];
		
	for (minorx = 0; minorx < 3; minorx++) {
	for (minory = 0; minory < 3; minory++) { 
		if (m.Minor[minorx][minory] != c) {
			return BOARDEMPTY;
		}
	}}

	return c;
}

/*
** checks if a player has won the game
*/
char gameWon(Game g) {
	Major m;
	int x, y;

	for (x = 0; x < 3; x++) {
	for (y = 0; y < 3; y++) {
		m.Minor[x][y] = majorScored(g.board[x][y]);
	}}

	return scorePositions(m);
	
}

/*
** checks if a major has been scored in
*/
char scorePositions(Major m) {
	int i;

	/*	
	** horizontals
	*/
	for (i = 0; i < 3; i++) {
		if (m.Minor[i][0] == m.Minor[i][1] && m.Minor[i][0] == m.Minor[i][2]) {
			if (m.Minor[i][0] != BOARDEMPTY) {
				return m.Minor[i][0];
			}
		}
	}

	/*
	** verticals
	*/
	for (i = 0; i < 3; i++) {
		if (m.Minor[0][i] == m.Minor[1][i] && m.Minor[0][i] == m.Minor[2][i]) {
			if (m.Minor[0][i] != BOARDEMPTY) {
				return m.Minor[0][i];
			}
		}
	}

	/*
	**diagonals
	*/
	if ((m.Minor[1][1] == m.Minor[0][0] && m.Minor[1][1] == m.Minor[2][2]) ||
		(m.Minor[1][1] == m.Minor[2][0] && m.Minor[1][1] == m.Minor[0][2])) {
		if (m.Minor[1][1] != BOARDEMPTY) {
			return m.Minor[1][1];
		}
	}

	return BOARDEMPTY;
}

/*
** draws the board with in an asthetically pleasing way
*/
void drawBoard(Game g) {
	int majorx, majory, minorx, minory;

	/*
	** top borders
	*/
	printf(" _________________________ \n");
	printf("|                         |\n");

	/*
	** board contents
	*/
	for (majory = 2; majory >= 0; majory--) {
	for (minory = 2; minory >= 0; minory--) {
		/*
		** y coordinates
		*/
		if (minory == 1) {
			printf("%d %d  ", majory + 1, minory + 1);
		} else {
			printf("| %d  ", minory + 1);
		}
		for (majorx = 0; majorx < 3; majorx++) {
		for (minorx = 0; minorx < 3; minorx++) {
			printf("%c ", g.board[majorx][majory].Minor[minorx][minory]);
		}
			printf(" ");
		}
		printf("|\n");
	}
		printf("|                         |\n");
	}

	/*
	** x coordinates
	*/
	printf("| @  a b c  a b c  a b c  |\n");
	printf("|______a______b______c____|\n");
}

/*
** neat lil animation to make seeing where the computer went easier
*/
void drawExplosion(Coord c, Coord lc, Game g, char turn) {
	int Mx, My, mx, my, Lx, Ly, lx, ly;
	float x, y, maxframe;
	float frame = 0;

	/*
	** text buffers
	*/
	char clrbuf[16][27], buf[16][27];

	/*
	** char arrays containing the decoration to make
	** it easier to copy
	*/
	char *topdecor = 		" _________________________ ";
	char *spacer =			"|                         |";
	char *bottomdecor1 = 	"| @  a b c  a b c  a b c  |";
	char *bottomdecor2 = 	"|______a______b______c____|";

	/*
	** create a 2d copy of the board display
	** to make clearing the buffer faster
	*/

	/*
	** decor bits
	*/
	for (Mx = 0; Mx < 27; Mx++) {
		clrbuf[0][Mx] = topdecor[Mx];
		clrbuf[14][Mx] = bottomdecor1[Mx];
		clrbuf[15][Mx] = bottomdecor2[Mx];
	}

	/*
	** empty body
	*/
	for (My = 1; My < 14; My++) {
	for (Mx = 0; Mx < 27; Mx++) {
		clrbuf[My][Mx] = spacer[Mx];
	}}

	/*
	** line numbers
	*/
	x = 3, y = 3;
	for (My = 2; My < 13; My++) {
		if (My % 4 == 1) {
			continue;
		}
		
		clrbuf[My][0] = y == 2 ? (char)(x + 48) : '|';
		clrbuf[My][2] = (char)(y + 48);
		y--;

		if (y < 1) {
			x--;
			y = 3;
		}
	}

	/*
	** board contents and converting coordinates
	*/
	x = 5, y = 2;
	for (My = 2; My >= 0; My--) {
	for (my = 2; my >= 0; my--) {
		x = 5;
		for (Mx = 0; Mx < 3; Mx++) {
		for (mx = 0; mx < 3; mx++) {
			clrbuf[(int)y][(int)x] =
			g.board[Mx][My].Minor[mx][my];
			if (lc.Mx == Mx && lc.My == My &&
				lc.mx == mx && lc.my == my) {
				lx = x;
				ly = y;
			} else if (c.Mx == Mx && c.My == My &&
					   c.mx == mx && c.my == my) {
				Lx = x;
				Ly = y;
			}
			x += 2;
		}
			x++;
		}
		y++;
	}
		y++;
	}

	/*
	** find out how roughly long the animation should be
	*/
	x = Lx - lx;
	y = Ly - ly;

	/*
	** three cheers for pythagoras!
	*/
	maxframe = (int)sqrt(fabsf(x * x) + fabsf(y * y));

	/*
	** create the buffer
	** because of rounding / integer truncation part two which would have
	** been the line being cleared, was never added as the math would not
	** allow for the line to be cleared perfectly the second go
	*/
	for (My = 0; My < 16; My++) {
	for (Mx = 0; Mx < 27; Mx++) {
		buf[My][Mx] = clrbuf[My][Mx];
	}}

	/*
	** animation loop
	*/
	while (frame < maxframe + 5) {
		/*
		** parts of the animation
		*/
		if (frame <= maxframe) {
			/*
			** part one - smoke trail
			*/
			buf[ly + (int)(y*(frame/maxframe))]
			   [lx + (int)(x*(frame/maxframe))] = 
			rand() % 5 < 3 ? '#' : '%';
		} else {
			/*
			** part three - burst
			*/

			/*
			** inner circle
			*/
			for (Mx = -1; Mx < 2; Mx++) {
			for (My = -1; My < 2; My++) {
				if ((Lx + Mx < 1 || Lx + Mx > 25 ||
					 Ly + My < 1 || Ly + My > 15)) {
					continue;
				}

				buf[Ly + My][Lx + Mx] = rand() % 5 < 3 ? '#' : '%';
			}}

			/*
			** outer spokes
			*/
			for (Mx = -2; Mx < 3; Mx += 2) {
			for (My = -2; My < 3; My += 2) {
				if ((Lx + Mx < 1 || Lx + Mx > 25 ||
					 Ly + My < 1 || Ly + My > 15)) {
					continue;
				}

				buf[Ly + My][Lx + Mx] = rand() % 5 < 3 ? '#' : '%';
			}}

			/*
			** middle
			*/
			buf[Ly][Lx] = turn;
		}

		/*
		** draw buffer along wiht clearing the screen
		*/
		printf(CLEARSCREEN);

		/*
		** quick title bar so the animation is in line with the board
		*/
		printf("Computer is playing...\n");

		for (My = 0; My < 16; My++) {
		for (Mx = 0; Mx < 27; Mx++) {
			printf("%c", buf[My][Mx]);
		}
			printf("\n");
		}

		/*
		** pause a bit so animation can be seen
		*/
		defsleep(100);
		frame++;
	}

	/*
	** clear the screen when finished
	*/
	printf(CLEARSCREEN);
}

/*
** my own sleep function because sleep is non standard lol
*/
void defsleep(int msec) {
	/*
	** enclosed in a preprocesser block as the command 
	** is system depandant hurrah for that
	*/
#	ifdef WINDOWS
	Sleep(msec);
#	else
	/*
	** so large milisecond arguments dont overflow
	*/
	time_t sec = 0;

	if (msec > 1000) {
		sec = msec / 1000;
		msec %= 1000;
	}

	nanosleep((const struct timespec[]){{sec, (long)msec * 1000000L}}, NULL);
#	endif
}

/*
** initialise all the values of the game so the we dont get
** undefined values being used accidentally
*/
void initBoard(Game *g) {
	int majorx, majory, minorx, minory, i;

	for (majory = 0; majory < 3; majory++) {
	for (majorx = 0; majorx < 3; majorx++) {
	for (minory = 0; minory < 3; minory++) {
	for (minorx = 0; minorx < 3; minorx++) {
		g->board[majory][majorx].Minor[minory][minorx] = BOARDEMPTY;
	}}}}

	g->turn = 1;

	g->moverecord[0] = 'r';
	
	for (i = 0; i <= 81 * 4; i++) {
		g->moverecord[i] = BOARDEMPTY;
	}
}

/*
** plays a move into the board
*/
void playToBoard(Coord c, Game *g, char tile) {
	g->board[c.Mx][c.My].Minor[c.mx][c.my] = tile;
}

/*
** fills a major with one character
*/
void fillMajor(Major *m, char c) {
	int minorx, minory;

	for (minorx = 0; minorx < 3; minorx++) {
	for (minory = 0; minory < 3; minory++) {
		m->Minor[minorx][minory] = c;
	}}
}

/*
** fills all parts of a coord with a number
*/
void emptyCoord(Coord *c, int replace) {
	c->Mx = replace;
	c->My = replace;
	c->mx = replace;
	c->my = replace;
}

/*
** adds a played move to the boards record of moves
*/
void recordMove(Coord c, Game *g, char strat) {
	int recordindex = ((g->turn - 1) * 4) + 1;

	g->moverecord[recordindex] = (char)(c.Mx + 97);
	g->moverecord[recordindex + 1] = (char)(c.My + 49);
	g->moverecord[recordindex + 2] = (char)(c.mx + 97);
	g->moverecord[recordindex + 3] = (char)(c.my + 49);
	g->moverecord[0] = strat;
}

/*
** replays a match by playing the moves found in the move record
*/
void playRecordToBoard(Game *g, Coord *m1, Coord *m2, char p1, char p2) {
	Coord move;

	int i;

	char c;

	/*
	** find out how many moves have been played
	*/
	for (i = 1; i <= 81 * 4; i += 4) {
		if (g->moverecord[i] != BOARDEMPTY) {
			g->turn++;
		}
	}

	/*
	** swap the last moves if it is an odd number of turns so we have the
	** correct last moves
	*/
	if (!(g->turn % 2)) {
		c = p1;
		p1 = p2;
		p2 = c;

		move = *m1;
		*m1 = *m2;
		*m2 = move;
	}

	/*
	** read the moves and play them to the board
	*/
	for (i = 1; i < ((g->turn - 1) * 4); i += 4) {
		m1->Mx = (int)g->moverecord[i] - 97;
		m1->My = (int)g->moverecord[i + 1] - 49;
		m1->mx = (int)g->moverecord[i + 2] - 97;
		m1->my = (int)g->moverecord[i + 3] - 49;

		if (m1->Mx < 3) {
			playToBoard(*m1, g, p1);
		}

		if (fillScored(g)) {
			emptyCoord(m1, -1);
		}

		/*
		** swap turns
		*/
		c = p1;
		p1 = p2;
		p2 = c;

		move = *m1;
		*m1 = *m2;
		*m2 = move;
	}
}

/*
** writes a save to the database
*/
void addSave(int slot, Game g) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int rc = 0;

	char *sql;

	sqlite3_open(DBNAME, &db);

	/*
	** sqlite open returns a null pointer if the connection attempt fails
	** not the most helpful since sqlite can also create a temporary database
	** when one is not found
	*/
	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	} else {
		/*
		** write save to table
		*/
		sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

		if (slotFull(slot)) {
			sql = "UPDATE saves SET record = ? WHERE id = ?;";
			sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
			sqlite3_bind_blob(stmt, 1, g.moverecord,
							((81 * 4) + 1) * sizeof(char), SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, slot);
		} else {
			sql = "INSERT INTO saves (id, record) VALUES (?, ?);";
			sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
			sqlite3_bind_int(stmt, 1, slot);
			sqlite3_bind_blob(stmt, 2, g.moverecord,
							((81 * 4) + 1) * sizeof(char), SQLITE_STATIC);
		}
	
		while (rc != SQLITE_DONE) {
			rc = sqlite3_step(stmt);
		}

		/*
		** so we dont leak memory
		*/
		sqlite3_finalize(stmt);

		sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

		/*
		** so we dont hit the files open limit
		*/
		sqlite3_close(db);
	}
}

/*
** read a save from the database
*/
void loadSave(int slot, Game *g) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int i;
	int rc = 0;

	char *record;

	char *sql = "SELECT record FROM saves WHERE id = ?;";

	sqlite3_open(DBNAME, &db);

	/*
	** check for a successful connection
	*/
	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, slot);

	/*
	** execute sql statement until finished
	*/
	while (rc != SQLITE_DONE) {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			record = (char*)sqlite3_column_blob(stmt, 0);
			if (record) {
				for (i = 0; i <= 81 * 4; i++) {
					g->moverecord[i] = record[i];
				}
			}
		}
	}

	sqlite3_finalize(stmt);

	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	sqlite3_close(db);
}


