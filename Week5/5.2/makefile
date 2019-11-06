CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: unlock
unlock: unlock.o unlock.c
	$(CC) $(CFLAGS) unlock.c -o unlock

clean:
	rm -f unlock unlock.o
run: unlock
	./unlock
