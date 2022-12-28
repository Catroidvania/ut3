/* 
file for board functions
catroidvania 27 12 22
version 1.0
*/

#include <stdio.h>

#include "board.h"
#include "main.h"

void drawBuffer(int width, int height, char contents[height][width]) {
	int row, column;

	for (row = 1; row <= height; row++) {
		for (column = 0; column < width; column++) {
			printf("%c", contents[row-1][column]);
		}
		printf("\n");
	}
}
