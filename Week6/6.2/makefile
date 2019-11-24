CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: primefactors
primefactors: primefactors.o primefactors.c
	$(CC) $(CFLAGS) primefactors.c -o primefactors

clean:
	rm -f primefactors primefactors.o
run: primefactors
	./primefactors
