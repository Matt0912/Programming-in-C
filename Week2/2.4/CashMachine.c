#include <stdio.h>
#define NDEBUG
#include <assert.h>

void test(void);
int findBounds(int input);
int checkInput(int input);

int main(void) {
  int input;
  test();
  printf("How much money would you like? ");
  scanf("%d", &input);
  while (checkInput(input) == 0) {
    input = findBounds(input);
  }
  return 0;
}

int findBounds(int input) {
  int upperBound, lowerBound, difference;
  difference = input % 20;
  lowerBound = input - difference;
  upperBound = input + (20 - difference);
  printf("I can give you £%d or £%d, please try again: ", lowerBound, upperBound);
  scanf("%d", &input);
  return input;
}

int checkInput(int input) {
  if (input < 0) {
    printf("Please enter valid input\n");
    return 1;
  } else {
    if (input % 20 == 0) {
      printf("Okay, dispensing £%d now\n", input);
      return 1;
    } else {
      return 0;
    }
  }
}

void test(void) {
  assert(checkInput(20) == 1);
  assert(checkInput(45) == 0);
}
