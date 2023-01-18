/**
** header file for opponent.c
** catroidvania dec 27 22
*/

/**
** prevent recursive definition
*/
#ifndef OPPONENT_H
#define OPPONENT_H

#include "main.h"
#include "board.h"

/**
** function for computer related processes that only need to be called once
** currently it is only seeding the rand() function with srand() so calling
** it more than once likely wont have any major detrimental effects but it
** might cause inconsistancies with the random number generation though the
** numbers generated are usually at such a small range it probably wont matter
*/
void initCpu(void);

/**
** given a Major it finds any availible Minor coord that is a valid move
**
** Coord *c	- coordinate contianing the major to search with which has
**			  its mx and my members modified to a valid move if there is
**			  or is set to -1 if none can be found
** Game g	- Game containing the board used to check for validity against 
*/
void dummyMinor(Coord*, Game);

/**
** counts the amount of a character given by match
**
** Major m		- the major to search through
** char match	- the character to search for
**
** returns the number of characters found that match match or 0 if none
*/
int countChars(Major, char);

/**
** returns a pseudo randomly selected legal move
**
** Coord lm	- Coord struct contining the last move played, if that move scored
**			  all its members should be set to less than 0
** Game g	- Game struct contianing the board state to check move legality
**			  against
**
** returns a Coord struct contianing the move selected by the computer that
** will always be a legal move based on the game state given
*/
Coord randomStrat(Coord, Game);

/**
** returns a move attempting to send the player to the least filled Major or
** plays in the square that it has the most plays in if allowed to go anywhere
**
** Coord lm	- Coord struct contining the last move played, if that move scored
**			  all its members should be set to less than 0
** Game g	- Game struct contianing the board state to check move legality
**			  against
**
** returns a Coord struct contianing the move selected by the computer that
** will always be a legal move based on the game state given
*/
Coord stallStrat(Coord, Game);

/**
** returns a move based on prefering certain squares over others depending on
** the position where when being sent the computer tries playing edges then
** corners then the centre if it has to while when allowed to go anywhere it
** uses a reverse priority for the Major coord trying to play centre then
** corner then edges last
**
** Coord lm	- Coord struct contining the last move played, if that move scored
**			  all its members should be set to less than 0
** Game g	- Game struct contianing the board state to check move legality
**			  against
**
** returns a Coord struct contianing the move selected by the computer that
** will always be a legal move based on the game state given
*/
Coord loganStrat(Coord, Game);

/**
** unused - code for an mcts strategy that was never implemented due to time
** compile with the make variable DEBUG set to anything other than empty
*/
#ifdef DEBUG

/**
** unused - upper confidance bound bias
** i belive a value of 0 is only exploitation
** while a value of 1 is maximum exploration
*/
#ifndef BIAS
#define BIAS 1
#endif

/**
** unused - monte carlo tree search node
*/
struct Node {
	char Turn;

	double Wins;
	double Visits;
	
	Game State;
	
	Node *Parent;
	Node *Children[81];
};

typedef struct Node Node;

/**
** unused - initialises a node with empty values so we dont get undefined 
** shenanigans happening with pointers and such
**
** Node *n		- pointer to the node we are initialising created by malloc or
**				  otherwise as long as there is the memory for it
**				  has all of its values modified
** Node *p		- pointer to the parent node that can be null if it is the
**				  root node just make sure to not orphan any memory
**				  does not get modified
** int branch	- index of the nodes pointer inside the parent Children array
*/
void initNode(Node*, Node*, int);

/**
** unused - free all the memory allocated for nodes in a node tree
**
** Node *n		- root node of the tree to be freed but as it recursively
** 				  calls itself this can be used on any node to free it and
**				  its children and *n is modified such that its pointer
**				  from the parent node is removed and itself is freed if it
**				  has no children, otherwise its attempts to free each of its
**				  childrens first before freeing itself
** int branch	- location in the parent node of the child node being passed
**				  can be any number for the root as it checks for a NULL
**				  pointer to the parent node
*/
void freeTree(Node*, int);

/**
** unused - applies the results of a simulation to a node and its parents all
** the way up to the root using recursion
**
** Node *n	- pointer to the leaf node that was simulated which gets the next
**			  parameter wins added to the nodes member Wins as well as
**			  incrementing the Visits member and calls backpropagate on the
**			  nodes parent if it is not NULL
** int wins	- number to add to the Wins member of *n, usually 1 for a win and
**			  0 for a loss but can be higher to change the wieghts of a win
*/
void backpropagate(Node*, int);

/**
** unused - simulates a game between two computers playing random moves 
**
** and returns whether the result is a victory for the computer opponent
** Node *n 	- pointer to the leaf node contianing the game state to
**			  simulate from
**
** returns 1 if the computer sees a win for itself or a 0 if it sees a win
** for the player
*/
int simulate(Node*);

/**
** unused - search the node tree for a leaf node based on the uct formula
**
** Node *n	- pointer to the root node to search from which gets modified by
**			  adding a new child when selecting an unvisited branch
** Coord lm	- Coordinate of the last move used to find the range of branches
**			  to search when searching the entire board is not needed
**
** returns a pointer to a new leaf node created at the bottom of the branch
** selected by the uct formula
*/
Node* selection(Node*, Coord);

/**
** unused - uses a number in the range 0 - 80 corresponding with the
** index of a branch in a nodes Children array to find its coordinate
** equivilant on the three by three by three by three array
** int i	- branch index to derive the coordinate from
** returns a Coord struct containing the found board coordinate
** note that the order of the loops changes how the index corresponds with
** the coordinate and when calculating Coord to index the formula matters
*/
Coord numberToCoord(int);

/**
** unused - returns a move selected using a monte carlo tree search
** usually inside selection() a segfault happens for whatever reason causing
** the node tree and rootcopy to not have thier memory freed although when
** not interrupted the tree should deallocate successfully
** this behavior seems to be caused by running many successive iterations of
** the tree search which may mean its caused by an overflow or some kind of
** corruption but the problem has not been found so use this at your own risk
**
** Coord lm		- Coord struct contining the last move played, if that move
**				  scored all its members should be set to less than 0
** Game g		- Game struct contianing the board state to check move 
**				  legality against
** int limit	- the maximum amount of leaf nodes to be created and simulated
**				  per turn and the higher the limit the better the move should
**				  be although the simulation strategy should be changed from 
**				  random to attain more applicable simulation results as well
**				  as moving the tree into the outside the scope of smartStrat
**				  so the tree can persist between function calls
**
** returns a Coord struct contianing the move selected by the computer that
** will always be a legal move based on the game state given
*/
Coord smartStrat(Coord, Game, int);

#endif


#endif
