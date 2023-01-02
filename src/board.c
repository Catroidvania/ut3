/* 
file for board functions
catroidvania 27 12 22
*/

#include <stdio.h>

#include "board.h"
#include "main.h"

Coord coordToBoardIndex(char coord[4]) {
	Coord c;

	c.Mx = ((int)coord[0] - 97);
	c.My = ((int)coord[1] - 49);
	c.mx = ((int)coord[2] - 97);
	c.my = ((int)coord[3] - 49);

	return c;
}

int validMove(Coord c, Coord lc, Game g) {
	if (c.Mx < 0 || c.Mx > 2 ||
		c.My < 0 || c.My > 2 ||
		c.mx < 0 || c.mx > 2 ||
		c.my < 0 || c.my > 2) {
		return 0;
	}

	if (g.board[c.Mx][c.My].Minor[c.mx][c.my] == BOARDEMPTY) {
		if (c.Mx == lc.mx && c.My == lc.my) {
			return 1;
		} else if (majorScored(g.board[lc.mx][lc.my]) != BOARDEMPTY) {
			return 1;
		} else if (lc.Mx < 0 || lc.My < 0 || lc.mx < 0 || lc.my < 0) {
			return 1;
		}
	}

	return 0;
}

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
		if (minorx == 1) {
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
	printf("|______A______B______C____|\n");
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

	for (i = 0; i < 81 * 4; i++) {
		g->moverecord[i] = -1;
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

void recordMove(Coord c, Game *g, char t) {
	int recordindex = g->turn * 4;
	g->moverecord[recordindex] = c.Mx;
	g->moverecord[recordindex + 1] = c.My;
	g->moverecord[recordindex + 2] = c.mx;
	g->moverecord[recordindex + 3] = c.my;
	g->moverecord[0] = (int)t;
}
