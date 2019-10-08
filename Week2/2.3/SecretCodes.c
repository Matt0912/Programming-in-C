#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

char convertLetter(char letter);
void test(void);

int main(void) {
  int input;
  test();
  while ((input = getchar()) != EOF) {
    putchar(convertLetter(input));
  }
  return 0;
}

char convertLetter(char letter) {
  char newLetter;
  if (letter >= 'a' && letter <= 'z') {
    newLetter = 'z' - letter + 'a';
    return newLetter;
  }
  else if (letter >= 'A' && letter <= 'Z'){
    newLetter = 'Z' - letter + 'A';
    return newLetter;
  }
  else {
    return letter;
  }
}

void test(void) {
  assert(convertLetter('a') == 'z');
  assert(convertLetter('m') == 'n');
  assert(convertLetter('A') == 'Z');
}
