CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: 71
71: 71.o 71.c
	$(CC) $(CFLAGS) 71.c -o 71

clean:
	rm -f 71 71.o
run: 71
	./71
