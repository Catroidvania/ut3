# ut3 (ultimate tic tac toe)

_made by catroidvania_

hey i wonder if i can use this for school...

## dependancies

| name | link |
|---------|------|
| Sqlite3 | [Sqlite3](https://www.sqlite.org/index.html) |

## building

sqlite3 is included with the project since its public domain code so

hip hip hurrah for that! if not then you will need to put the files:

- sqlite3.c
- sqlite3.h

into the `/deps/` folder.

### windows

to build on windows you will need either `minGW32` or `minGW-w64` (for

32bit and 64bit computers respectively) installed. 

then change directories into `/ut3/` and run:

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

valid c compiler (you will need to chang the `CC` variable in the Makefile).

change into the `/ut3/` directory and run:

```
make
```

and then you should find the `ut3` executable in the `/build/` directory.

run it with:

```
./ut3
```

## finally

have fun ultimate tic tac toer!

catroidvania out!

