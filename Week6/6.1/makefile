CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: binarysearch
binarysearch: binarysearch.o binarysearch.c
	$(CC) $(CFLAGS) binarysearch.c -o binarysearch

clean:
	rm -f binarysearch binarysearch.o
run: binarysearch
	./binarysearch
