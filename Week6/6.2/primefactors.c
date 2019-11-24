#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define MAXFACTORS 100
#define MAXINPUT 20

enum bool {FALSE, TRUE};

void test(void);
int isValid(char input[MAXINPUT]);
int isPrime(int num);
int primeFactors(int input, int factors[MAXFACTORS]);
void printArray(int factors[MAXFACTORS], int size, int input);

int main(int argc, char **argv) {
  int size, factors[MAXFACTORS], userInput;
  test();

  if (argc == 2) {
    if (isValid(argv[1])) {
      userInput = atoi(argv[1]);
    }
    else {
      fprintf(stderr, "ERROR: USER INPUT NOT VALID\n");
      exit(1);
    }
  }
  else {
    fprintf(stderr, "INVALID USE - E.G ./primeFactors 210\n");
    exit(1);
  }

  size = primeFactors(userInput, factors);
  if (size == FALSE) {
    fprintf(stdout, " %d IS PRIME SO HAS NO PRIME FACTORS!\n", userInput);
  }
  else {
    printArray(factors, size, userInput);
  }
  return FALSE;
}

void test(void) {
  int factors[MAXFACTORS];

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

int isValid(char input[MAXINPUT]) {
  int i = 0, num;

  while (input[i]) {
    if (!isdigit(input[i])) {
      return FALSE;
    }
    i++;
  }

  num = atoi(input);
  if (num <= 0) {
    return FALSE;
  }

  return TRUE;
}

int isPrime(int num) {
  int factor = 2;
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

int primeFactors(int input, int factors[MAXFACTORS]) {
  int factor, i = 0;
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

void printArray(int factors[MAXFACTORS], int size, int input) {
  int i = 0;
  fprintf(stdout, "USER INPUT: %d\n", input);
  while (i < size - 1) {
    fprintf(stdout, "%d x ", factors[i]);
    i++;
  }
  fprintf(stdout, "%d = %d\n", factors[i], input);
}
