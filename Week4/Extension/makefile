CC=gcc
CFLAGS= -pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2 -lncurses -lm

all: WireworldCurses
WireworldCurses: WireworldCurses.o WireworldCurses.c
	$(CC) $(CFLAGS) neillncurses.c WireworldCurses.c -o WireworldCurses

clean:
	rm -f WireworldCurses WireworldCurses.o
run: WireworldCurses
	./WireworldCurses
