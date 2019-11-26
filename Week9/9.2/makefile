CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: compTrees
compTrees: compTrees.o compTrees.c
	$(CC) $(CFLAGS) compTrees.c -o compTrees

clean:
	rm -f compTrees compTrees.o
run: compTrees
	./compTrees
