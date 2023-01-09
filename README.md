# ut3 (ultimate tic tac toe)

_made by catroidvania_

## dependancies

| name | link |
|---------|------|
| Sqlite3 | [Sqlite3](https://www.sqlite.org/index.html) |
| MinGW | [MinGW-w64](https://www.mingw-w64.org) |

sqlite3 is included with the project since its public domain code so

hip hip hurrah for that! if not then you will need to put the files:

- sqlite3.c
- sqlite3.h

into the `/deps/` folder.

if you are on windows you will need a version of mingw to build the project

the link is included in the table above in the MinGW section just 

navigate to the downloads section and install the version for you system

you can check that it is installed correctly by running the command:

```
gcc --version
```

as long as the command is recognised it should work

## building

### windows

make sure you have either `minGW32` or `minGW-w64` (for 32bit and 64bit

computers respectively) installed. then change directories into `/ut3/` 

and run:

```
mingw32-make
```

if all works then the executable will be in the `/build/` directory

and you can run it with:

```
ut3
```

### linux / macos

building on unix requires gnu `make` utility as well as `gcc` or another

valid c compiler (you will need to change the `CC` variable in the Makefile

if youre using another compiler). change into the `/ut3/` directory and run:

```
make
```

and then you should find the `ut3` executable in the `/build/` directory.

run it with:

```
./ut3
```

## how to play

couldawouldashoulda at the moment

## finally

have fun ultimate tic tac toer!

catroidvania out!

