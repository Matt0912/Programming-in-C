#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#define MAXSTRING 75

void test(void);
int isPalindrome(char *input, int strlength);

int main(void) {
  /*char input[MAXSTRING];*/
  test();

  return 0;
}

int isPalindrome(char *input, int strlength) {
  int i;
  for (i=0;i<strlength;i++) {
    if (input[i] != input[strlength-(i+1)]) {
      return 0;
    }
  }
  return 1;
}

void test(void) {
  assert(isPalindrome("kayak", 5) == 1);
  assert(isPalindrome("A man, a plan, a canal: Panama!", 31) == 1);
  assert(isPalindrome("Madam, in Eden I’m Adam,", 24) == 1);
  assert(isPalindrome("Level, madam, level!", 20) == 1);
}
