/*
main file for catroidvanias ultimate tic tac toe program 
created 27 12 22
*/

#include <stdio.h>

#include "main.h"
#include "board.h"
#include "opponent.h"

int main() {
	Game game;
	Coord move, cpu;

	char menu, strat, turn, winner;
	char input[4];

	char P1CHAR = 'x';
	char P2CHAR = 'o';
	
	int run = 1, state = 1;
	int i;
	
	computerInit();

	while (run) {
		printf("%c[2J%c[;H", (char)27, (char)27);

		if (state == 1) {
			printf("\nWhat would you like to do?:\n");
			printf("(p)lay a game\n");
			printf("(l)oad a save\n");
			printf("(q)uit the program\n");
			printf("Bracketed letter of option: ");
			ffgets(&menu, 1, stdin);

			if (menu == 'p') {
				printf("\nWho goes first?:\n");
				printf("(p)layer\n");
				printf("(c)omputer\n");
				printf("Bracketed letter of option: ");
				ffgets(&turn, 1, stdin);

				printf("\nWhat strategy should the computer use?:\n");
				printf("(r)andom\n");
				printf("(s)tall - not yet implemented!\n");
				printf("(f)ocus - not yet implemented!\n");
				printf("(l)ogan - not yet implemented!\n");
				printf("Bracketed letter of options: ");
				/*
				other strats to add?? maybe??
				copycat - tries to copy your last move
				fool - always sends to your most controlled major
				brute force - ew i have to optimise
				hmm
				*/
				ffgets(&strat, 1, stdin);

				strat = 'r';  /* to remove */
	
				initBoard(&game);
				
				emptyCoord(&move);
				emptyCoord(&cpu);

				if (turn == 'c') {
					turn = P2CHAR;
				} else {
					turn = P1CHAR;
				}

				state = 2;

			} else if (menu == 'l') {
				continue;
			} else if (menu == 'q') {
				break;
			} else {
				printf("Please input a valid option!\n");
				waitForInput();
			}
		} else if (state == 2) {
			if (gameWon(game) != BOARDEMPTY) {
				winner = gameWon(game);
				if (winner == P1CHAR) {
					printf("\nPlayer wins! Congrats!\n");
				} else {
					printf("\nComputer wins! Better luck next time!\n");
				}
				
				for (i = 0; i < 81 * 4; i++) {
					printf("%d", game.moverecord[i]);
				}

				printf("\n");

				waitForInput();
				state = 1;
				continue;
			}

			printf("Turn %d: ", game.turn);

			if (turn == P2CHAR) {
				printf("Computer turn! ");
			} else {
				printf("Player turn! ");
			}
		
			printf("Previous move: ");

			if ((turn == P2CHAR) && (move.Mx >= 0 && move.My >= 0)) {	
				printf("%c%i%c%i\n", (char)move.Mx + 97, move.My + 1,
									 (char)move.mx + 97, move.my + 1);
			} else if (cpu.Mx >= 0 && cpu.My >= 0) {
				printf("%c%i%c%i\n", (char)cpu.Mx + 97, cpu.My + 1,
									 (char)cpu.mx + 97, cpu.my + 1);
			} else {
				printf("Play anywhere!\n");
			}
		
			if (turn == P2CHAR) {
				if (strat == 'r') {
					cpu = randomStrat(move, game);
				} else {
					printf("\nNot yet implemented!\n");
					waitForInput();
					game.turn = 0;
					continue;
				}

				playToBoard(cpu, &game, P2CHAR);
				recordMove(cpu, &game, P2CHAR);

				drawBoard(game);

				printf("\nThe computer played: ");
				printf("%c%i%c%i\n", (char)cpu.Mx + 97, cpu.My + 1,
									 (char)cpu.mx + 97, cpu.my + 1);
				
				waitForInput();

				if (fillScored(&game)) {
					emptyCoord(&cpu);
				}

				game.turn++;
				turn = P1CHAR;
				continue;
			}

			drawBoard(game);

			printf("\nWhat would you like to do?:\n");
			printf("(p)lay a move\n");
			printf("(s)ave game\n");
			printf("(q)uit game\n");
			printf("Bracketed letter or input a valid move (eg. a3c2): ");
			ffgets(&input[0], 4, stdin);

			if (input[0] == 'p') {
				printf("Major coordinate?: ");
				ffgets(&input[0], 2, stdin);
				printf("Minor coordinate?: ");
				ffgets(&input[2], 2, stdin);

				move = coordToBoardIndex(input);

				if (!(validMove(move, cpu, game))) {
					printf("Invalid move!\n");
					waitForInput();
					continue;
				}
			} else if (input[0] == 's') {
				continue;
			} else if (input[0] == 'q') {
				state = 1;
				continue;
			} else if (validMove(coordToBoardIndex(input), cpu, game)) {
				move = coordToBoardIndex(input);
			} else {
				printf("\nPlease input a valid option!\n");
				waitForInput();
				continue;
			}

			playToBoard(move, &game, P1CHAR);
			recordMove(move, &game, P1CHAR);

			game.turn++;
	
			if (fillScored(&game)) {
				emptyCoord(&move);
			}

			turn = P2CHAR;
			
			/*
			TODO
			win checking
			previous move incorrectly displays scored sometimes
			save / load
			extra cpu starts
			explosions
			fix valid move checking; sometimes gives freedom sometimes doesnt?
			also lets the opponent play into a scored square because it doesnt
			fill for some reason?? might be a logic issue idk
			*/
		}
	}
	
	printf("Bye bye!\n");
	return 0;
}

void waitForInput() {
	char confirm;
	printf("\nPress enter to continue...\n");
	ffgets(&confirm, 1, stdin);
}

void ffgets(char *buf, int bufsize, FILE *stream) {
	/*
	flushed file get string
	reads into bufsize amount of character into buf,
	then reads and ignores all trailing characters in the line
	unsure if this is even safe \\: but hey it works lmao
	*/
	int x = 0;
	char c;
	
	while (1) {
		c = fgetc(stream);
		
		if (c == '\0' || c == '\n') {
			break;
		} else if (x < bufsize) {
			buf[x++] = c;
		}
	}
}
