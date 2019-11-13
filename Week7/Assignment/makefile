CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: 73
73: 73.o 73.c
	$(CC) $(CFLAGS) 73.c -o 73

clean:
	rm -f 73 73.o
run: 73
	./73
