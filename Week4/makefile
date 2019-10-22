CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: RomanNumerals
RomanNumerals: RomanNumerals.o RomanNumerals.c
	$(CC) $(CFLAGS) RomanNumerals.c -o RomanNumerals

clean:
	rm -f RomanNumerals RomanNumerals.o
run: RomanNumerals
	./RomanNumerals
