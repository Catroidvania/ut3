/*****************************************************************************
**
** main.c 
** created dec 27 2022
** by catroidvania
**
******************************************************************************
*/

#include <stdio.h>		/* for io operations								*/
#include <time.h>		/* for finding operation speed						*/

/*
** i dont need all the features in sqlite3 so i may make my own build of it
*/
#include "sqlite3.h"	/* for database operations							*/

#include "main.h"
#include "board.h"
#include "opponent.h"

/*
** main()
**
** main handles the input and output for the game as well as initialising and
** storing game information like the board and player / cpu moves
**
** overall program is not as optimised as it could be but i am a c newb rn lol
*/
int main() {
	/*
	** declare variables for storing game and menu info
	*/
	Game game;
	Coord move, cpu;

	char menu, strat, turn, winner, cslot;
	char input[4];
	
	int run = 1, state = 1, saved = 0;
	int i, slot;
	/*
	** both are called here so they only run once since there is no need for
	** to verify the existance of the save database multiple times and there
	** calling initCpu() more than once would change the seed for srand()
	*/
	initSaveFile();
	initCpu();
	
	/*
	** main game loop
	*/
	while (run) {
		/*
		** if the WINDOWS macro is given at compile time the escape chars
		** are replaced with some newlines to move the screen along
		*/
		#ifdef WINDOWS
		printf("\n\n\n\n");
		#else
		printf("%c[2J%c[;H\n", (char)27, (char)27);
		#endif

		/*
		** state one is the main menu and there is no state zero
		*/
		if (state == 1) {
			/*
			** welcome text that i forgot about lol
			*/
			printf("\nWelcome to Ultimate Tic Tac Toe! (Toefish)\n");

			/*
			** replacing printf()s without bindings with puts() might improve
			** optimisation although the compiler might already handle that
			** 
			** i could go read the assembly but im not sure optimisation is
			** that nessesary for this project
			*/
			printf("\nWhat would you like to do?:\n");
			printf("(p)lay a game\n");
			printf("(l)oad a save\n");
			printf("(d)isplay saves\n");
			//printf("(h)ow to play\n"); TODO
			printf("(q)uit the program\n");
			printf("Bracketed letter of option: ");
			/*
			** information about ffgets() is found below main()
			*/
			ffgets(&menu, 1, stdin);

			/*
			** menu for starting a new game
			*/
			if (menu == 'p') {
				printf("\nWho goes first?:\n");
				printf("(p)layer\n");
				printf("(c)omputer\n");
				printf("Bracketed letter of option: ");
				ffgets(&turn, 1, stdin);

				printf("\nWhat strategy should the computer use?:\n");
				printf("(r)andom - plays random moves\n");
				printf("(s)tall  - always sends to the least played major\n");
				printf("(l)ogan  - uses send priority!\n");
				printf("Bracketed letter of options: ");
				ffgets(&strat, 1, stdin);

				/*
				** there was originally going to be a brute force strategy but
				** it would have been very hard to balance as it would play
				** perfectly as well as not being human followable
				*/
				if (strat != 'r' && strat != 's' && strat != 'l') {
					printf("Invalid strategy! Defaulting to random...\n");
					strat = 'r';
					waitForInput();
				}
	
				initBoard(&game);
				
				emptyCoord(&move, 1);
				emptyCoord(&cpu, -1);

				/*
				** the turn counter is tracked as a char for convenience
				** note that this turn is not the same as game.turn
				*/
				if (turn == 'c') {
					turn = P2CHAR;
				} else {
					turn = P1CHAR;
				}

				state = 2;

			/*
			** menu for loading a save from the database which is still broken
			** on my windows seven machine and i cannot for the life of me
			** figure out why since sqlite3 seems to have a mind of its own
			** but knowing the age of the machine it could be a hardware issue
			*/
			} else if (menu == 'l') {
				printf("\nLoad which slot? (Slot number): ");
				ffgets(&cslot, 1, stdin);
				slot = (int)cslot - 48;

				if (slot < 1) {
					printf("Invalid slot!\n");
					waitForInput();
					continue;
				}

				/*
				** default initialisation so in the case loading a save fails
				** it will start a new game with the random strategy instead
				*/
				initBoard(&game);
				
				emptyCoord(&move, -1);
				emptyCoord(&cpu, -1);

				loadSave(slot, &game);

				playRecordToBoard(&game, &move, &cpu, P1CHAR, P2CHAR);

				strat = game.moverecord[0];

				if (strat != 'r' && strat != 's' && strat != 'l') {
					printf("Load error! Defaulting to random strategy...\n");
					strat = 'r';
					waitForInput();
				}

				/*
				** saved is checked even though no save is made since the game
				** is already saved to the database at this point and it makes
				** testing saving and loading less of a hassle
				*/
				saved = 1;
				state = 2;

			/*
			** displays all saves stored in the database
			*/
			} else if (menu == 'd') {
				displaySaves();
				waitForInput();
				continue;

			/*
			** quit program where the use of break invalidates the need for
			** the run variable although it could come in handy later
			*/
			} else if (menu == 'q') {
				break;

			/*
			** in case an invalid option is given
			** because you can never trust the user to play nicely
			*/
			} else {
				printf("Please input a valid option!\n");
				waitForInput();
			}

		/*
		** handles game states with two being game in progress and three being
		** game finished
		*/
		} else if (state > 1) {
			if (gameWon(game) != BOARDEMPTY && state == 2) {
				winner = gameWon(game);
				state = 3;

				/*
				** an extra move is appended to the move record as the program
				** is built around the idea that you can only save on the 
				** players turn and therefore must always end on a cpu move
				**
				** this move accounts for that but is ignored as it is both an
				** invalid move and the game would already be over
				*/
				if (winner == P1CHAR) {
					move = coordToBoardIndex("d4d4");
					recordMove(move, &game, strat);
				}

				/*
				** moves are cleared so no new moves can be played after the
				** game is over although im not really sure this is even needed
				** anymore since all relavant playToBoard()s cehck for state
				*/
				emptyCoord(&move, -1);
				emptyCoord(&cpu, -1);
			} else if (state == 2 && gameTied(game)) {
				state = 3;
				/*
				** this char represents a tie which i should really change to
				** something like the null char for possible customisation
				*/
				winner = 't';

				emptyCoord(&move, -1);
				emptyCoord(&cpu, -1);
			}

			/*
			** these if statements make sure certain things only run when the
			** game is ongoing as they would not make sense in a game over
			*/
			if (state == 2) {
				printf("Turn %d: ", game.turn);

				if (turn == P2CHAR) {
					printf("Computer turn! ");
				} else {
					printf("Player turn! ");
				}
			
				printf("Previous move: ");

				/*
				** somewhat verbose code to print the last move made to help
				** remember which majors are legal to play in
				*/
				if ((turn == P2CHAR) && (move.Mx >= 0 && move.My >= 0)) {
					if (majorFilled(game.board[cpu.mx][cpu.my])) {
						printf("Play anywhere!\n");
					} else {
						printf("%c%i%c%i\n", (char)move.Mx + 97, move.My + 1,
											 (char)move.mx + 97, move.my + 1);
					}
				} else if (cpu.Mx >= 0 && cpu.My >= 0) {
					if (majorFilled(game.board[cpu.mx][cpu.my])) {
						printf("Play anywhere!\n");
					} else {
						printf("%c%i%c%i\n", (char)cpu.Mx + 97, cpu.My + 1,
											 (char)cpu.mx + 97, cpu.my + 1);
					}
				} else {
					printf("Play anywhere!\n");
				}
			/*
			** these happen during state three which represents a game end
			*/
			} else if (state == 3) {
				if (winner == P1CHAR) {
					printf("You win! Way to go Ultimate Tic Tac Toer!\n");
				} else if (winner == P2CHAR) {
					printf("You lose! Better luck next time!\n");
				/*
				** wait i guess the tie character didnt actually matter lol
				*/
				} else {
					printf("Tie! Nobody wins!\n");
				}
			}
		
			/*
			** gets the cpu move which i feel like could be made less verbose
			** by using a function pointer defined during game initialisation
			*/
			if (turn == P2CHAR && state == 2) {
				if (strat == 'r') {
					cpu = randomStrat(move, game);
				} else if (strat == 's') {
					cpu = stallStrat(move, game);
				} else if (strat == 'l') {
					cpu = loganStrat(move, game);
				} else {
					printf("\nInvalid Strategy!\n");
					waitForInput();
					turn = P1CHAR;
					continue;
				}

				/*
				** in case last move scored it will be the centre of the
				** scored major or if the computer goes first
				*/
				if (move.mx < 0 || move.my < 0) {
					move.mx = game.moverecord[((turn - 1) * 4) + 1];
					move.my = game.moverecord[((turn - 1) * 4) + 2];
					move.Mx = game.moverecord[((turn - 1) * 4) + 3];
					move.My = game.moverecord[((turn - 1) * 4) + 4];
				}

				drawExplosion(cpu, move, game, P2CHAR);

				playToBoard(cpu, &game, P2CHAR);
				recordMove(cpu, &game, strat);

				/*
				** has to be called here so the text displays in the correct
				** order even though i find myself looking at the last move
				** most of the time instead of the below the board
				*/
				printf("Computer has played!\n");

				drawBoard(game);

				printf("\nThe computer played: ");
				printf("%c%i%c%i\n", (char)cpu.Mx + 97, cpu.My + 1,
									 (char)cpu.mx + 97, cpu.my + 1);
				
				waitForInput();

				if (fillScored(&game)) {
					emptyCoord(&cpu, -1);
				}

				game.turn++;
				turn = P1CHAR;
				saved = 0;
				continue;
			}

			drawBoard(game);

			/*
			** stuff for players turn
			*/
			printf("\nWhat would you like to do?:\n");
			/*
			** certain menu options are only displayed and are valid when the
			** game is still ongoing like playing moves
			*/
			if (state == 2) {
				printf("(p)lay a move\n");
			}
			printf("(s)ave game\n");
			printf("(q)uit game\n");
			printf("Bracketed letter of option");
			if (state == 2) {
				printf(" or input a valid move (eg. a3c2)");
			}
			printf(": ");
			ffgets(&input[0], 4, stdin);

			/*
			** explicit play move option in case putting the major and minor
			** coordinates is too hard for the user
			*/
			if (input[0] == 'p' && state == 2) {
				printf("Major coordinate? (eg. b2): ");
				ffgets(&input[0], 2, stdin);
				printf("Minor coordinate? (eg. c1): ");
				ffgets(&input[2], 2, stdin);

				move = coordToBoardIndex(input);

				if (!(validMove(move, cpu, game))) {
					printf("Invalid move!\n");
					waitForInput();
					continue;
				}

			/*
			** menu for saving the game
			*/
			} else if (input[0] == 's') {
				printf("\nSlot to save to (Slot number): ");
				ffgets(&cslot, 1, stdin);
				slot = (int)cslot - 48;

				if (slot < 1) {
					printf("Invalid slot!\n");
					waitForInput();
					continue;
				}

				printf("\nConfirm saving to slot %d?\n", slot);
				printf("(Any data in slot %d will be overwritten!)", slot);
				
				/*
				** even when this block does not show any sign of error the
				** database still does not get written to sometimes
				*/
				if (waitForConfirm()) {
					addSave(slot, game);
					printf("\nGame saved to slot %d!\n", slot);
					saved = 1;
				} else {
					printf("\nSave cancelled!\n");
				}

				waitForInput();
				continue;

			/*
			** option to return to the main menu without saving
			*/
			} else if (input[0] == 'q') {
				/*
				** reminder to the user and gives them one last chance to save
				*/
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

			/*
			** if the option given is a valid move then play it
			*/
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
				emptyCoord(&move, -1);
			}

			turn = P2CHAR;
			saved = 0;
			
		}
	}
	
	/*
	** end of main and also program exit point
	*/
	printf("Bye bye!\n");
	return 0;
}

/*
** function that gets a yes no response from the user and returns and integer
** that represents true or false
*/
int waitForConfirm() {
	char confirm;

	printf("\nConfirm yes or no (y or n): ");
	ffgets(&confirm, 1, stdin);

	if (confirm == 'y') {
		return 1;
	}

	return 0;
}

/*
** function that check is a given slot is full so we dont try and insert when
** an update is needed
*/
int slotFull(int slot) {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int rc;
	int exists = 0;

	char *sql = "SELECT id FROM saves WHERE id = ?;";

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		return 1;
	}

	/*
	** doesnt save to database unless i explicitly start and end a transaction
	** for some reason but doesnt seem to be the issue on the windows version
	*/
	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, slot);

	while (rc != SQLITE_DONE) {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			exists = sqlite3_column_int(stmt, 0);
		}
	}

	if (exists > 0) {
		exists = 1;
	} else {
		exists = 0;
	}

	sqlite3_finalize(stmt);

	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
	sqlite3_close(db);

	return exists;
}

/*
** checks if SAVES.db exists and creates it if not as well as creating the
** saves table in the database
*/
void initSaveFile() {
	sqlite3 *db;
	sqlite3_stmt *stmt;
	
	FILE *fp;

	char *sql = 
		"CREATE TABLE saves ("\
		"id INTEGER NOT NULL PRIMARY KEY,"\
		"record BLOB NOT NULL);";

	int rc;

	/*
	** read mode so we dont overwrite an existing file
	** does not exist if it returns a null pointer
	*/
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

/*
** simple select operation to list all the saves in the database
*/
void displaySaves() {
	sqlite3 *db;
	sqlite3_stmt *stmt;

	int i, rc;
	
	char *record;
	char *sql = "SELECT * FROM saves;";

	sqlite3_open(DBNAME, &db);

	if (db == NULL) {
		printf("\nFailed to open saves database!\n");
		waitForInput();
		return;
	}

	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	
	/*
	** we need to print per step as the column pointer changes even without an
	** sqlite_row happening
	*/
	while (rc != SQLITE_DONE) {
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			record = (char*)sqlite3_column_blob(stmt, 1);
			printf("SLOT: %d | STRAT: %c | MOVES: ",
					sqlite3_column_int(stmt, 0), record[0]);

			for (i = 1; i <= (81 * 4) + 1; i++) {
				if (record[i] != BOARDEMPTY) {
					printf("%c", record[i]);
				}

				if (i != 1 && i % 4 == 1) {
					printf(" ");
				}
			}
			printf("\n");
		}
	}

	sqlite3_finalize(stmt);

	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	sqlite3_close(db);
}

/*
** reads and discards the input buffer when a newline is given
*/
void waitForInput() {
	char confirm;
	printf("\nPress enter to continue...\n");
	ffgets(&confirm, 1, stdin);
}

/*
** python esque input function
*/
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
		
		/*
		** might need to include NULL in this list
		*/
		if (c == '\n' || c == EOF) {
			break;
		} else if (x < bufsize) {
			buf[x++] = c;
		}
	}
}
