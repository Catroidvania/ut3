/*
main file for catroidvanias ultimate tic tac toe program 
created 27 12 22
version 1.0
*/

#include <stdio.h>

#include "main.h"
#include "board.h"

int main() {
	char buffer[DISPLAYHEIGHT][DISPLAYWIDTH];
	
	int row, column;

	for (row = 0; row < DISPLAYHEIGHT; row++) {
		for (column = 0; column < DISPLAYWIDTH; column ++) {
			buffer[row][column] = ' ';
		}
	}

	printf("Testing testing...\n");
	drawBuffer(80, 24, buffer);

	return 0;
}
