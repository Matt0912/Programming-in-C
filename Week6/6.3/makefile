CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2 -lm

all: sierpinski
sierpinski: sierpinski.o sierpinski.c
	$(CC) $(CFLAGS) sierpinski.c -o sierpinski

clean:
	rm -f sierpinski sierpinski.o
run: sierpinski
	./sierpinski
