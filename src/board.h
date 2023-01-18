/**
** header file for board.c
** catroidvania dec 27 2022
*/

/**
** prevent multiple / recursive declarations
*/
#ifndef BOARD_H
#define BOARD_H

#include "main.h"

/**
** constant for the empty board square character
** can be defined at compile time but defaults to '.'
*/
#define BOARDEMPTY '.'

/**
** struct that holds all the parts of a ut3 coordinate
** all the members are expected to be integers from 0 to 2 as they refer to
** the board indexes not the coordinate string itself
**
** int Mx 	- integer representing the major x coordinate (board x location)
** int My 	- integer representing the major y coordinate (board y location)
** int mx 	- integer representing the minor x coordinate (x position inside board)
** int my 	- integer representing the minor y coordinate (y position inside board)
*/
struct Coord {
	int Mx, My, mx, my;
};

typedef struct Coord Coord;

/**
** struct representing a ut3 three by three board
**
** char Minor[3][3] 	- a three by three array of characters
*/
struct Major {
	char Minor[3][3];
};

typedef struct Major Major;

/**
** struct containing game related information
** in hindsight should have contained all of the game related information
** but that ship has salied sadly
**
** int turn 		- integer counting how many turns has passed
** char moverecord 	- a char array of 84 moves and one char for the strategy
** Major board		- a three by three array of Majors that stores the board
*/ 
struct Game {
	int turn;
	char moverecord[(81 * 4) + 1];
	Major board[3][3];
};

typedef struct Game Game;

/**
** converts a coordinate string to a Coord struct
**
** char coord[4]	- expected to be an arry with only the characters abc123
**
** returns a Coord struct containing the equivilant board index given by
** coord with all elements within the range 0 - 2
*/
Coord coordToBoardIndex(char[4]);

/**
** checks if a given move is valid or not
**
** Coord c	- the move attempting to be played as a Coord struct
** Coord lc	- the last move played as a Coord struct
** Game g	- the current state of the game as a Game struct
**
** returns 1 if the move if legal, 0 if not
*/
int validMove(Coord, Coord, Game);

/**
** checks if any Majors need to be filled and fills them
**
** Game *g 	- pointer to a Game struct that has any valid elements in board
**			  modified to be filled with the scorers character
**
** returns 1 if any Majors were filled, 0 if none were
*/
int fillScored(Game*);

/**
** checks for empty space in the board to tell if it is a tie
**
** Game g	- Game struct containing the game state to check
**
** returns 1 if the board is full, 0 if there is empty space
*/
int gameTied(Game);

/**
** checks if a major has been filled
**
** Major m	- a Major struct to check for empty space in
**
** returns 1 if it is full, 0 if any empty space is found
*/
int majorFilled(Major);

/**
** checks if the game has been won by either player
**
** Game g	- the game to check as a Game struct
**
** returns the char of the winner if there is one, returns BOARDEMPTY if not
*/
char gameWon(Game);

/**
** check if a major has been filled with only one character
** Major m	- a Major to check for uniformity
** returns the char of the scorer if there is one, return BOARDEMPTY if not
*/
char majorScored(Major);

/**
** checks if a given position is a valid tic tac toe win
**
** Major m	- a Major to check for scoring positions in
**
** returns the char of the scorer if there is one, returns BOARDEMPTY if not
*/
char scorePositions(Major);

/**
** draws the board with added decoration
** Game g	- Game struct containg the board to draw
*/
void drawBoard(Game);

/**
** draws an explosion effect from one point to another
**
** Coord c		- Coord of the location of the explosion destination
** Coord lc		- Coord of the start of the explosion effect
** Game g		- Game struct containing the board state to draw on
** char turn	- the char of the player being played - unused as the
** 				  animation only plays on the computers turn right now
*/
void drawExplosion(Coord, Coord, Game, char);

/**
** sleep function that changes depending on the system
**
** int msec	- miliseconds to suspend program execution for
**
** uses Sleep() on windows and nanosleep on everything else which may not be
** fully portable for windows 32 bit systems as i have yet to test on one of
** those although it relies on window.h so it should hopefully be fine
*/
void defsleep(int);

/**
** initialises default values for a given Game struct
**
** Game *g	- pointer to the Game struct to initialise
**
** fills all elements of board to BOARDEMPTY sets the turn to one
** sets the first element of moverecord to 'r' and the rest to BOARDEMPTY
*/
void initBoard(Game*);

/**
** plays a move to the board
**
** Coord c		- coord of the move to play
** Game *g		- pointer to the Game struct contiaining the game board to 
**				  be modified
** char tile	- character tile of the player playing the move
*/
void playToBoard(Coord, Game*, char);

/**
** initialises a Coord struct with a given value
**
** Coord *c		- pointer to the Coord struct to modify
** int replace	- integer to set all the values to
*/
void emptyCoord(Coord*, int);

/**
** replaces all elements in a Major with a given character
**
** Major *m	- pointer to the Major to be filled
** char c	- char to replace all elements with
*/
void fillMajor(Major*, char);

/**
** adds a move to a moverecord in its character form
**
** Coord c		- coordinate to add to the record
** Game *g		- pointer to the Game struct containing the moverecord
** char strat	- character corresponding to the strategy to update
*/
void recordMove(Coord, Game*, char);

/**
** recreates a game based on a given moverecord
**
** Game *g		- Game struct containing the moverecord and board to modify
** Coord *m1	- Coord struct that gets set to its player ones last move
** Coord *m2	- Coord struct that gets set to its player twos last move
** char p1		- Character representing player one
** char p2		- Character representing player two
**
** note that p1 and p2 are not P1CHAR and P2CHAR as it is not known which
** player actually went first so it must be clairified
** this also applies to the *m1 and *m2 so swap the arguments when needed
** 
** update im not sure the above text applies anymore - may change later
*/
void playRecordToBoard(Game*, Coord*, Coord*, char, char);

/**
** loads a save from the database
**
** int slot	- slot number to load relating to the id column of the database
** Game *g	- Game struct to write query to
**
** the function does not initialise the Game struct so that must be done
** before the function in the case no slot can be loaded
*/
void loadSave(int, Game*);

/**
** writes a save to the database
**
** int slot	- slot number to save to relating to the id column of the database
** Game g	- Game containing the moverecord that gets written
*/
void addSave(int, Game);

#endif


