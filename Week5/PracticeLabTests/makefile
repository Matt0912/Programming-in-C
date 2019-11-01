CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: mutate2
mutate2: mutate2.o mutate2.c
	$(CC) $(CFLAGS) mutate2.c -o mutate2

clean:
	rm -f mutate2 mutate2.o
run: mutate2
	./mutate2
