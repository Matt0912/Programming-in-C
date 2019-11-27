CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: huffman
huffman: huffman.o huffman.c
	$(CC) $(CFLAGS) huffman.c -o huffman

clean:
	rm -f huffman huffman.o
run: huffman
	./huffman
