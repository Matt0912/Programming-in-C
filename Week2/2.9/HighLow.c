#include <stdio.h>
#define NDEBUG
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void test(void);
int checkGuess(int guess, int random);

int main(void) {
  int guess, random, tries = 9;
  srand(time(NULL));
  test();
  random = rand() % 1000;
  printf("Guess the number I'm thinking of between 1 and 1000: ");
  scanf("%d", &guess);
  while (tries) {
    if (checkGuess(guess, random) == 0) {
      scanf("%d", &guess);
      tries--;
      printf("You have %d tries left!\n", tries);
    }
    else {
      tries = 0;
    }
  }
  return 0;
}

int checkGuess(int guess, int random) {
  if (guess > random) {
    printf("Too high! Try again: ");
    return 0;
  }
  else if (guess < random) {
    printf("Too low! Try again: ");
    return 0;
  }
  else if (guess == random) {
    printf("Congratulations! %d was the right answer!\n", guess);
    return 1;
  }
  else {
    printf("NOT A VALID GUESS, TRY AGAIN: ");
    return 0;
  }
}

void test(void) {
  assert(checkGuess(10, 10) == 1);
  assert(checkGuess(10, 9) == 0);
}
