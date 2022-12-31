# ut3 (ultimate tic tac toe)

_made by catroidvania_

## dependancies

| none so | far! |
|---------|------|
|         |      |

## building

ensure you have all the listed dependancies installed or placed in `/deps/`

### windows

to build on windows you will need either `minGW32` or `minGW-w64` (for

32bit and 64bit computers respectively) installed. 

then change directories into `/ut3/` and run

```
mingw32-make
```

if all works then the executable will be in the `/build/` directory

and you can run it with

```
ut3
```

### linux / macos

building on unix requires gnu `make` utility as well as `gcc` or another

valid c compiler (you will need to chang the `CC` variable in the Makefile).

change into the `/ut3/` directory and run

```
make
```

and then you should find the `ut3` executable in the `build` directory.

run it with

```
./ut3
```

### ew object files

you can also `make` the `clean` target to remove the object files

which would look soemthing like this

```
mingw32-make clean
```

or for unix

```
make clean
```

## finally

have fun ultimate tic tac toer!

catroidvania out!

