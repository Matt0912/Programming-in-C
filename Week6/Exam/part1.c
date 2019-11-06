#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void test(void);
int pwdok(char s[]);

int main(void) {

  test();


  return 0;
}

void test(void) {
  assert(pwdok("Ab1") == 1);
  assert(pwdok("Ab#") == 1);
  assert(pwdok("Ab#8901lll") == 1);
  assert(pwdok("19l}") == 0);
  assert(pwdok("Ab") == 0);
  assert(pwdok("#abjsdfksndf4h5323") == 0);
  assert(pwdok(" ") == 0);
  assert(pwdok("JJJJJJJJJJJJJ") == 0);
}

int pwdok(char s[]) {
  int i = 0, length = strlen(s);
  int checkUpper = 0, checkLower = 0, checkNonAlpha = 0;
  while (i < length) {
    if (islower(s[i])) {
      checkLower = 1;
    }
    if (isupper(s[i])) {
      checkUpper = 1;
    }
    if (isalpha(s[i]) == 0) {
      checkNonAlpha = 1;
    }
    i++;
  }
  if (checkUpper == 1 && checkLower == 1 && checkNonAlpha == 1) {
    return 1;
  }
  else {
    return 0;
  }
}
