#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void test(void);
void passswp(char* a, char* b);
void reset(char* b);

int main(void) {

  test();

  return 0;
}

void test(void) {
  char b[100];
  passswp("Hello SsoOaA", b);
  assert(strcmp(b,"Hell0 5500@@") == 0);
  passswp("aaaa", b);
  assert(strcmp(b,"@@@@") == 0);
  passswp("ABCdefghijklmnopqrstuvwxyz",b);
  assert(strcmp(b, "@BCdefghijklmn0pqr5tuvwxyz") == 0);
  passswp("BCBHOLSaaaa",b);
  assert(strcmp(b, "BCBH0L5@@@@") == 0);
}

void passswp(char* a, char* b) {
  int i = 0, length = strlen(a);
  char c;
  while (i < length+1) {
    c = tolower(a[i]);
    if (c == 'a') {
      b[i] = '@';
    }
    else if (c == 'o') {
      b[i] = '0';
    }
    else if (c == 's') {
      b[i] = '5';
    }
    else {
      b[i] = a[i];
    }
    i++;
  }
}
