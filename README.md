# ut3 (ultimate tic tac toe)

made by catroidvania!

## building

step one of course is downloading the source code

or you can just use the jank binaries i release sometimes 

though you will have to deal with windows defender and such


in the future i will probably try cmake or any other kind of more portable

build / compile system

### windows 32 bit

you will need mingw32 installed which can be found here: 

- <https://mingw.osdn.io>

make sure that is installs mingw32-make and gcc

then change into the `ut3-master` directory and run:

```
mingw32-make
```

if all goes well you can run the program created in the `build` directory:

```
cd build
ut3
```

### windows 64 bit

you will need mingw64 to build the project

the w64devkit is found here: 

- <https://www.mingw-w64.org/downloads/#w64devkit>

just run the same commands as for linux / macos through the devkit program

### linux / macos

building on linux / macos requires gnu `make` utility as well as `gcc` 

found here:

- <https://www.gnu.org/software/make/#download>
- <https://gcc.gnu.org/install/binaries.html>

move to the `ut3-master` directory and run:

```
make
```

and then you should find the `ut3` executable in the `build` directory.

run it with:

```
cd build
./ut3
```

note that i have not been able to test the program on any linux system so uh

have fun

## documentation

it is in the works at the moment but documentation for the code is done as

in file comments

this is neither a particularly complicated program nor one that i expect needs

over detailed documentation but i am availible to clarify certain parts of my

spaghetti code if you need it

## license

find out more about the MIT license here: 

- <https://snyk.io/learn/what-is-mit-license/>

## how to play

note that the game can be played with a pencil and paper so you dont actually

need this program to play the game

### understanding the coordinate system

if you know how to play tic tac toe you can play ultimate tic tac toe!

as opposed to the three by three tic tac toe board:


![A regular board](./img/boardregular.png "A normal tic tac toe board")


the ultimate tic tac toe board is nine by nine!

but dont let that scare you its really just a three by three grid of 

regular tic tac toe boards:


![Empty board](./img/board.png "An empty board")


this program uses what i call major-minor notation for its coordinates

with the major part refering to the position of the regular tic tac toe

board on the larger board and the minor part refering to the position of the

tile inside the regular board so the coords system looks something like this:


![Empty Board](./img/board0.png "An empty board")


as an example playing to the bottom left in the centre board would be:

```
b2a1
```

since its in the major `b2` (centre board) and on the minor `a1` (bottom left)

now that we have notation done onto the game itself!

### playing the game

the first player starts by playing anywhere on the board


![One move played](./img/board1.png "Board with one move played")


here they are represented as `x` and play the move b2a2 (centre middle left)

now the next player is not so lucky

they are `sent' (forced to play in) the major corresponding to the minor

coordinate of the last move

so the second player has to play in the major a2


![Two moves played](./img/board2.png "Board with two moves played")


thus a2b2 is played and so on and on

but the game doesnt go on forever like this

you win by first winning the smaller tic tac toe games and each small win

can be seen as one move one on the larger board which you must score on to

win the whole game itself

for the sake of explanation lets assume the game has played out to this point


![About scoring](./img/board3.png "A scored major")


now that the centre has been won by `x` it is no longer able to be played in

this program represents it like below but you can just draw a big `x` or `o`

over the whole thing on pencil and paper edition


![Centre is filled](./img/board4.png "Centre scored")


now that `x` has scored this means that the next move can be made anywhere

that isnt already filled

(note that this is a house rule and in most versions you still need to follow

the send restrictions after a score)

everntually the board starts to fill up and you may find yourself sending or

sent to an already won major


![Game progressed](./img/board5.png "Late game")


lets say `o` plays b1b2 (middle bottom centre) which has already been won and

so cannot be played in

in this case the next move can be played in anywhere that isnt already filled

this continues until one of the players wins enough majors that form a winning

position in a regular tic tac toe game


![Game over](./img/board6.png "The game is over")


now `x` has won in a diagonal line which in normal tic tac toe is a winning

position so `x` wins the game

this can happen for any valid tic tac toe position and the game can also tie

if all valid moves are exhausted and there is no winner

there can also be ties for each of the smaller boards themselves which means

nobody gets that major

### other stuff to note i guess

scoring on a major does not need to be done in alternating turns like in

tic tac toe so `x` could score the majors a1 then c1 then c2 without `o`

ever winning anything or vice versa

if you have trouble reading my instructions the wikipedia article may be of

use: 

- <https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe>

## finally

have fun ultimate tic tac toer!

catroidvania out!

