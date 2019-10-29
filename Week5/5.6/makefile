CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: maze
maze: maze.o maze.c
	$(CC) $(CFLAGS) maze.c -o maze

clean:
	rm -f maze maze.o
run: maze
	./maze
