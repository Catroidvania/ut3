/*
main file for catroidvanias ultimate tic tac toe program 
created 27 12 22
*/

#include <stdio.h>

/*
now that i think about it i shouldve just built sqlite3
myself i dont need all eight million constants lol
*/
#include "sqlite3.h"

#include "main.h"
#include "board.h"
#include "opponent.h"

int main() {
	Game game;
	Coord move, cpu;

	char menu, strat, turn, winner, cslot;
	char input[4];

	char P1CHAR = 'x';
	char P2CHAR = 'o';
	
	int run = 1, state = 1, saved = 0;
	int i, slot;
	
	initSaveFile();
	initCpuRandom();

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
				printf("\nLoad which slot?: ");
				ffgets(&cslot, 1, stdin);
				slot = (int)cslot - 48;

				if (slot < 1) {
					printf("Invalid slot!\n");
					waitForInput();
					continue;
				}

				initBoard(&game);
				
				emptyCoord(&move);
				emptyCoord(&cpu);

				loadSave(slot, &game);

				playRecordToBoard(&game, P1CHAR, P2CHAR);

				strat = game.moverecord[0];
				
				if (game.turn % 2) {
					turn = 'p';
					for (i = 0; i < 4; i++) {
						input[i] = game.moverecord[((game.turn-1)*4)+i-3];
					}
					cpu = coordToBoardIndex(input);
					if (majorScored(game.board[cpu.Mx][cpu.My])) {
						emptyCoord(&cpu);
					}

					for (i = 0; i < 4; i++) {
						input[i] = game.moverecord[((game.turn-1)*4)+i+1];
					}
					move = coordToBoardIndex(input);
					if (majorScored(game.board[move.Mx][move.My])) {
						emptyCoord(&move);
					}

				} else {
					turn = 'c';
					for (i = 0; i < 4; i++) {
						input[i] = game.moverecord[((game.turn-1)*4)+i-3];
					}
					move = coordToBoardIndex(input);
					if (majorScored(game.board[move.Mx][move.My])) {
						emptyCoord(&move);
					}

					for (i = 0; i < 4; i++) {
						input[i] = game.moverecord[((game.turn-1)*4)+i+1];
					}
					cpu = coordToBoardIndex(input);
					if (majorScored(game.board[cpu.Mx][cpu.My])) {
						emptyCoord(&cpu);
					}
				}

				saved = 1;
				state = 2;
			} else if (menu == 'q') {
				break;
			} else {
				printf("Please input a valid option!\n");
				waitForInput();
			}
		} else if (state > 1) {
			if (gameWon(game) != BOARDEMPTY && state == 2) {
				winner = gameWon(game);
				state = 3;

				if (winner == P1CHAR) {
					move = coordToBoardIndex("d4d4");
					recordMove(move, &game, strat);
				}

				emptyCoord(&move);
				emptyCoord(&cpu);
			}

			if (state == 2) {
				printf("Turn %d: ", game.turn);

				if (turn == P2CHAR) {
					printf("Computer turn! ");
				} else {
					printf("Player turn! ");
				}
			
				printf("Previous move: ");

				/*
				theres probably some logic combination that works better than this
				*/
				if ((turn == P2CHAR) && (move.Mx >= 0 && move.My >= 0)) {
					if (majorScored(game.board[move.mx][move.my]) != BOARDEMPTY) {
						printf("Play anywhere!\n");
					} else {
						printf("%c%i%c%i\n", (char)move.Mx + 97, move.My + 1,
											 (char)move.mx + 97, move.my + 1);
					}
				} else if (cpu.Mx >= 0 && cpu.My >= 0) {
					if (majorScored(game.board[cpu.mx][cpu.my]) != BOARDEMPTY) {
						printf("Play anywhere!\n");
					} else {
						printf("%c%i%c%i\n", (char)cpu.Mx + 97, cpu.My + 1,
											 (char)cpu.mx + 97, cpu.my + 1);
					}
				} else {
					printf("Play anywhere!\n");
				}
			} else if (state == 3) {
				if (winner == P1CHAR) {
					printf("You win! Way to go Ultimate Tic Tac Toer!\n");
				} else if (winner == P2CHAR) {
					printf("You lose! Better luck next time!\n");
				}
			}
		
			if (turn == P2CHAR && state == 2) {
				if (strat == 'r') {
					cpu = randomStrat(move, game);
				} else {
					printf("\nNot yet implemented!\n");
					waitForInput();
					game.turn = 0;
					continue;
				}

				playToBoard(cpu, &game, P2CHAR);
				recordMove(cpu, &game, strat);

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
				saved = 0;
				continue;
			}

			drawBoard(game);

			printf("\nWhat would you like to do?:\n");
			if (state == 2) {
				printf("(p)lay a move\n");
			}
			printf("(s)ave game\n");
			printf("(q)uit game\n");
			printf("Bracketed letter");
			if (state == 2) {
				printf(" or input a valid move (eg. a3c2)");
			}
			printf(": ");c2c3c2c3
			ffgets(&input[0], 4, stdin);

			if (input[0] == 'p' && state == 2) {
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
				printf("\nSlot to save to: ");
				ffgets(&cslot, 1, stdin);
				slot = (int)cslot - 48;

				if (slot < 1) {
					printf("Invalid slot!\n");
					waitForInput();
					continue;
				}

				printf("\nConfirm saving to slot %d?\n", slot);
				printf("(Any data in slot %d will be overwritten!)", slot);
				
				if (waitForConfirm()) {
					addSave(slot, game);
					printf("\nGame saved to slot %d!\n", slot);
					saved = 1;
				} else {
					printf("\nSave cancelled!\n");
				}

				waitForInput();
				continue;
			} else if (input[0] == 'q') {
				if (!saved) {
					printf("\nAre you sure you want to quit?\n");
					printf("(Any unsaved data will be lost!)");

					if (waitForConfirm()) {
						state = 1;
					}
				} else {
					state = 1;
				}

				continue;
			} else if (validMove(coordToBoardIndex(input), cpu, game)) {
				move = coordToBoardIndex(input);
			} else {
				printf("\nPlease input a valid option!\n");
				waitForInput();
				continue;
			}

			if (state == 2) {
				playToBoard(move, &game, P1CHAR);
				recordMove(move, &game, strat);
			}
			game.turn++;
	
			if (fillScored(&game)) {
				emptyCoord(&move);
			}

			turn = P2CHAR;
			saved = 0;
			
			/*
			TODO
			extra cpu starts
			explosions
			save / load doesnt set previous moves properly
			occasionally places the wrong tiles :// i htink
			need to account for dummy move in turn counter
			*/
		}
	}
	
	printf("Bye bye!\n");
	return 0;
}

int waitForConfirm() {
	char confirm;

	printf("\n(y/n): ");
	ffgets(&confirm, 1, stdin);

	if (confirm == 'y') {
		return 1;
	}

	return 0;
}

int slotFull(int slot) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int rc, exists;

	char *sql = "SELECT id FROM saves WHERE id = ?;";

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		return 1;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, slot);

	while (rc != SQLITE_DONE) {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			exists = sqlite3_column_int(stmt, 0);
		}
	}

	if (exists) {
		exists = 1;
	} else {
		exists = 0;
	}

	sqlite3_finalize(stmt);

	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
	sqlite3_close(db);

	return exists;
}

void initSaveFile() {
	sqlite3 *db;
	sqlite3_stmt *stmt;
	
	FILE *fp;

	char *sql = 
		"CREATE TABLE saves ("\
		"id INTEGER NOT NULL PRIMARY KEY,"\
		"record BLOB NOT NULL);";

	int rc;

	fp = fopen(DBNAME, "r");

	if (fp) {
		fclose(fp);
		return;
	}

	fp = fopen(DBNAME, "w");
	fclose(fp);

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	
	while (rc != SQLITE_DONE) {
		rc = sqlite3_step(stmt);
	}

	sqlite3_finalize(stmt);

	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	sqlite3_close(db);
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
		
		if (c == '\n' || c == EOF) {
			break;
		} else if (x < bufsize) {
			buf[x++] = c;
		}
	}
}
