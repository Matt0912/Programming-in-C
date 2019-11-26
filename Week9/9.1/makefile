CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: treeDepth
treeDepth: treeDepth.o treeDepth.c
	$(CC) $(CFLAGS) treeDepth.c -o treeDepth

clean:
	rm -f treeDepth treeDepth.o
run: treeDepth
	./treeDepth
