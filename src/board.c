/*****************************************************************************
**
** board.c
** file for board related functions
** created by catroidvania dec 27 2022
**
******************************************************************************
*/

#include <stdio.h>		/* for io operations								*/

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
		} else if (lc.Mx < 0 || lc.My < 0 || lc.mx < 0 || lc.my < 0) {
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

char majorScored(Major m) {
	/*
	this is only needed for gameWon()
	majorFilled() does this but less verbose
	nvm its used in fillScored() so scoring
	on the last empty square still works
	*/
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

char gameWon(Game g) {
	Major m;
	int x, y;

	for (x = 0; x < 3; x++) {
	for (y = 0; y < 3; y++) {
		m.Minor[x][y] = majorScored(g.board[x][y]);
	}}

	return scorePositions(m);
	
}

char scorePositions(Major m) {
	int i;

	/*
	must be a better way to loop over all this
	
	horizontals
	*/
	for (i = 0; i < 3; i++) {
		if (m.Minor[i][0] == m.Minor[i][1] && m.Minor[i][0] == m.Minor[i][2]) {
			if (m.Minor[i][0] != BOARDEMPTY) {
				return m.Minor[i][0];
			}
		}
	}
	/*
	verticals
	*/
	for (i = 0; i < 3; i++) {
		if (m.Minor[0][i] == m.Minor[1][i] && m.Minor[0][i] == m.Minor[2][i]) {
			if (m.Minor[0][i] != BOARDEMPTY) {
				return m.Minor[0][i];
			}
		}
	}
	/*
	diagonals
	*/
	if ((m.Minor[1][1] == m.Minor[0][0] && m.Minor[1][1] == m.Minor[2][2]) ||
		(m.Minor[1][1] == m.Minor[2][0] && m.Minor[1][1] == m.Minor[0][2])) {
		if (m.Minor[1][1] != BOARDEMPTY) {
			return m.Minor[1][1];
		}
	}

	return BOARDEMPTY;
}

void drawBoard(Game g) {
	int majorx, majory, minorx, minory;

	printf(" _________________________ \n");
	printf("|                         |\n");

	for (majory = 2; majory >= 0; majory--) { // needs to be draw y first lmao
	for (minory = 2; minory >= 0; minory--) {
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

	printf("| @  a b c  a b c  a b c  |\n");
	printf("|______a______b______c____|\n");
}

void drawExplosion(Coord c, Coord lc,  Game g, char turn) {
	int Mx, My, mx, my, x, y;
	int frame;
	char buf[16][28];
	/*
	** char arrays containing the decoration to make
	** it easier to copy
	*/
	char *topdecor[] = {
		" _________________________ \n",
		"|                         |\n"};
	char *spacer = 
		"|                         |\n";
	char *bottomdecor[] = {
		"| @  a b c  a b c  a b c  |\n",
		"|______a______b______c____|\n"};

	for (frame = 0; frame < 15; frame++) {
		for (My = 0; My < 16; My++) {
		for (Mx = 0; Mx < 28; Mx++) {
			if (My < 2) {
				buf[My][Mx] = topdecor[My][Mx];
			} else if (My > 25) {
				buf[My][Mx] = bottomdecor[My - 26][Mx];
			} else {
				mx = ((Mx - 5) / 2) / 4, my = (My - 2) / 4;
				x  = ((Mx - 5) / 2) % 4, y  = (My - 2) % 4;

				if (Mx > 4) {
					if (y == 0) {
						buf[My][Mx] = spacer[Mx];
					} else {
						if (Mx % 2) {
							buf[My][Mx] = ' ';
						} else {
							buf[My][Mx] = g.board[mx][my].Minor[x][y];
						}
					}
				} else if (Mx > 25) {
					buf[My][Mx] = Mx == 26 ? '|' : '\n';
				} else if (Mx == 0) {
					buf[My][0] = y == 1 ? (char)(my + 50) : '|';
					buf[My][1] = ' ';
					buf[My][2] = (char)(y + 50);
					buf[My][3] = ' ';
					buf[My][4] = ' ';	
				}
			}
		}}

		/* TODO animations here */
		if (frame < 5) {

		} else if (frame < 10) {

		} else {

		}

		defsleep(200);
	}
}

/*
** my own sleep function because sleep is non standard lol
*/
void defsleep(int msec) {
	/*
	** enclosed in a preprocesser block as the command 
	** is system depandant hurrah for that
	*/
#ifdef WINDOWS
	Sleep(msec);
#else
	/*
	** so large milisecond arguments dont overflow
	*/
	time_t sec = 0;

	if (msec > 1000) {
		sec = msec / 1000;
		msec %= 1000;
	}

	nanosleep((const struct timespec[]){{sec, (long)msec * 1000000L}}, NULL);
#endif
}

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

void playToBoard(Coord c, Game *g, char tile) {
	g->board[c.Mx][c.My].Minor[c.mx][c.my] = tile;
}

void fillMajor(Major *m, char c) {
	int minorx, minory;

	for (minorx = 0; minorx < 3; minorx++) {
	for (minory = 0; minory < 3; minory++) {
		m->Minor[minorx][minory] = c;
	}}
}

void emptyCoord(Coord *c) {
	c->Mx = -1;
	c->My = -1;
	c->mx = -1;
	c->my = -1;
}

void recordMove(Coord c, Game *g, char strat) {
	int recordindex = ((g->turn - 1) * 4) + 1;

	g->moverecord[recordindex] = (char)(c.Mx + 97);
	g->moverecord[recordindex + 1] = (char)(c.My + 49);
	g->moverecord[recordindex + 2] = (char)(c.mx + 97);
	g->moverecord[recordindex + 3] = (char)(c.my + 49);
	g->moverecord[0] = strat;
}

void playRecordToBoard(Game *g, Coord *m1, Coord *m2, char p1, char p2) {
	Coord move;

	int i, turn;

	char c;

	for (i = 1; i <= 81 * 4; i += 4) {
		if (g->moverecord[i] != BOARDEMPTY) {
			g->turn++;
		}
	}

	if (!(g->turn % 2)) {
		c = p1;
		p1 = p2;
		p2 = c;

		move = *m1;
		*m1 = *m2;
		*m2 = move;
	}

	for (i = 1; i < ((g->turn - 1) * 4); i += 4) {
		m1->Mx = (int)g->moverecord[i] - 97;
		m1->My = (int)g->moverecord[i + 1] - 49;
		m1->mx = (int)g->moverecord[i + 2] - 97;
		m1->my = (int)g->moverecord[i + 3] - 49;

		if (m1->Mx < 3) {
			playToBoard(*m1, g, p1);
		}

		if (fillScored(g)) {
			emptyCoord(m1);
		}

		c = p1;
		p1 = p2;
		p2 = c;

		move = *m1;
		*m1 = *m2;
		*m2 = move;
	}
}

void addSave(int slot, Game g) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int rc, saveno;

	char *sql;

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	} else {
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

		sqlite3_finalize(stmt);

		sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

		sqlite3_close(db);
	}
}

void loadSave(int slot, Game *g) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int rc, i;

	char *record;

	char *sql = "SELECT record FROM saves WHERE id = ?;";

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, slot);

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


