CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: 8tile
8tile: 8tile.o 8tile.c
	$(CC) $(CFLAGS) 8tile.c -o 8tile

clean:
	rm -f 8tile 8tile.o
run: 8tile
	./8tile
