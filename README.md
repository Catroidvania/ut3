# ut3 (ultimate tic tac toe)

_made by catroidvania_

hey i wonder if i can use this for school...

## dependancies

| name | link |
|---------|------|
| Sqlite3 | [Sqlite3](https://www.sqlite.org/index.html) |

## building

ensure you have all the listed dependancies installed or have all the 

.c and .h files placed in `/deps/`!

sqlite3 is already included in `/deps/` so dont worry about that one.

wait why and i telling you all this all the required dependancies are

packaged with this project lmao. hip hip hoorah for public domain code!

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

