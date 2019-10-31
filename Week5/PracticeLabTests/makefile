CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: anagrams
anagrams: anagrams.o anagrams.c
	$(CC) $(CFLAGS) anagrams.c -o anagrams

clean:
	rm -f anagrams anagrams.o
run: anagrams
	./anagrams
