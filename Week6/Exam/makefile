CC=gcc
CFLAGS=-pedantic -ansi  -Wall -Wextra -Wfloat-equal -O2

all: part3
part3: part3.o part3.c
	$(CC) $(CFLAGS) part3.c -o part3

clean:
	rm -f part3 part3.o
run: part3
	./part3
