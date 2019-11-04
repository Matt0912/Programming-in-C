CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: SelfAnagram
SelfAnagram: SelfAnagram.o SelfAnagram.c
	$(CC) $(CFLAGS) SelfAnagram.c -o SelfAnagram

clean:
	rm -f SelfAnagram SelfAnagram.o
run: SelfAnagram
	./SelfAnagram
