CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: Palindromes
Palindromes: Palindromes.o Palindromes.c
	$(CC) $(CFLAGS) Palindromes.c -o Palindromes

clean:
	rm -f Palindromes Palindromes.o
run: Palindromes
	./Palindromes
