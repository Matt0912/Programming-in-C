CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: Wireworld
Wireworld: Wireworld.o Wireworld.c
	$(CC) $(CFLAGS) Wireworld.c -o Wireworld

clean:
	rm -f Wireworld Wireworld.o
run: Wireworld
	./Wireworld
