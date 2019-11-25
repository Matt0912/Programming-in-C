#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define MAXFACTORS 200
#define MAXINPUT 40

enum bool {FALSE, TRUE};

typedef unsigned long int u32;


void test(void);
u32 checkInput(int argc, char **argv);
int isValid(char input[MAXINPUT]);
int isPrime(u32 num);
int primeFactors(u32 input, u32 factors[MAXFACTORS]);
void printArray(u32 factors[MAXFACTORS], int size, u32 input);

int main(int argc, char **argv) {
  int size;
  u32 userInput, factors[MAXFACTORS];
  test();

  if ((userInput = checkInput(argc, argv)) == FALSE) {
    exit(1);
  }

  if ((size = primeFactors(userInput, factors)) == FALSE) {
    fprintf(stdout, " %lu IS PRIME SO HAS NO PRIME FACTORS!\n", userInput);
  }
  else {
    printArray(factors, size, userInput);
  }
  return FALSE;
}

void test(void) {
  u32 factors[MAXFACTORS];

  assert(isValid("123") == TRUE);
  assert(isValid("123345710") == TRUE);
  assert(isValid("999999999") == TRUE);
  assert(isValid("1") == TRUE);
  assert(isValid("-1") == FALSE);
  assert(isValid("j123") == FALSE);
  assert(isValid("1987h76") == FALSE);
  assert(isValid("[675]") == FALSE);
  assert(isValid(".9021") == FALSE);
  assert(isValid("-9021") == FALSE);

  assert(isPrime(53) == 1);
  assert(isPrime(154321) == 1);
  assert(isPrime(100) == 0);
  assert(isPrime(1) == 0);
  assert(isPrime(-5) == 0);
  assert(isPrime(4) == 0);

  assert(primeFactors(5, factors) == FALSE);
  assert(factors[0] == 5);
  assert(primeFactors(6, factors) == 2);
  assert(factors[0] == 2);
  assert(factors[1] == 3);
  assert(primeFactors(210, factors) == 4);
  assert(factors[0] == 2);
  assert(factors[1] == 3);
  assert(factors[2] == 5);
  assert(factors[3] == 7);

}

u32 checkInput(int argc, char **argv) {
  u32 userInput;
  char* pointer;
  if (argc == 2) {
    if (isValid(argv[1])) {
      userInput = strtoul(argv[1], &pointer, 10);
      return userInput;
    }
    else {
      fprintf(stderr, "ERROR: USER INPUT NOT VALID\n");
      return FALSE;
    }
  }
  else {
    fprintf(stderr, "INVALID USE - E.G ./primefactors 210\n");
    return FALSE;
  }
}

int isValid(char input[MAXINPUT]) {
  u32 num;
  char *pointer;
  int i = 0;

  while (input[i]) {
    if (!isdigit(input[i])) {
      return FALSE;
    }
    i++;
  }

  num = strtoul(input, &pointer, 10);
  if (num <= 0) {
    return FALSE;
  }

  return TRUE;
}

int isPrime(u32 num) {
  u32 factor = 2;
  if (num < 2) {
    return FALSE;
  }
  while (factor <= num/2) {
    if (num % factor == 0) {
      return FALSE;
    }
    else {
      factor++;
    }
  }
  return TRUE;
}

int primeFactors(u32 input,
                               u32 factors[MAXFACTORS]) {
  u32 factor;
  int i = 0;
  if (isPrime(input) || input < 2) {
    factors[0] = input;
    return FALSE;
  }
  while (!isPrime(input)) {
    factor = 2;
    while (factor < input) {
      if (input%factor == 0) {
        factors[i] = factor;
        i++;
        input = input/factor;
        factor = 1;
      }
      factor++;
    }
  }
  factors[i] = input;
  return i + 1;

}

void printArray(u32 factors[MAXFACTORS], int size, u32 input) {
  int i = 0, j, count;
  fprintf(stdout, "USER INPUT: %lu\n 1", input);
  while (i < size - 1) {
    j = i + 1;
    count = 1;
    while (j < size) {
      if (factors[i] == factors[j]) {
        count++;
      }
      j++;
    }
    if (count > 1) {
      fprintf(stdout, " x %lu^%d", factors[i], count);
    }
    else {
      fprintf(stdout, " x %lu", factors[i]);
    }
    i += count;
  }
  if (i == size - 1) {
    fprintf(stdout, " %lu = %lu\n", factors[i], input);
  }
  else {
    fprintf(stdout, " = %lu\n", input);
  }
}
