#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void test(void);
char* pwdextend(char* a);

int main(void) {
  char *p3;
  p3 = pwdextend("HELLO there how are you");
  printf("%s\n", p3);

  test();

  return 0;
}

void test(void) {
  char *p2, *p1;
  p1 = pwdextend("password");
  p2 = pwdextend("hackable");
  assert(strcmp(p1, "password808") == 0);
  assert(strcmp(p2, "hackable808") == 0);
}

char* pwdextend(char* a) {
  int i = 0, length = strlen(a);
  char *b;
  b = (char *)malloc(sizeof(a) + 3);

  while (a[i]) {
    b[i] = a[i];
    i++;
  }

  b[length] = '8';
  b[length + 1] = '0';
  b[length + 2] = "8\n";

  return b;
}
