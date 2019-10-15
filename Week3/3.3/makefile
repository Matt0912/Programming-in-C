CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: Rule110
Rule110: Rule110.o Rule110.c
	$(CC) $(CFLAGS) Rule110.c -o Rule110

clean:
	rm -f Rule110 Rule110.o
run: Rule110
	./Rule110
