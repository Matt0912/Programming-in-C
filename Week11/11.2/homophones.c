#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mvm.h"

enum bool {FALSE, TRUE};

int hashIndex(char* string);
void test(void);

int main(void) {
  test();

  return FALSE;
}

void test(void) {
  char *testStr1 = "STANO#S T AA1 N OW0";
  char *testStr2 = "FURGERSON#F ER1 G ER0 S AH0 N";
  char *testStr3 = "STEELED#S T IY1 L D";
  char *testStr4 = "STEELEDS T IY1 L D";

  assert(hashIndex(testStr1) == 5);
  assert(hashIndex(testStr2) == 9);
  assert(hashIndex(testStr3) == 7);
  assert(hashIndex(testStr4) == FALSE);


}

/* Return the index of the '#' in a string*/
int hashIndex(char* string) {
  int i = 0;
  while (string[i]) {
    if (string[i] == '#') {
      return i;
    }
    i++;
  }
  return FALSE;
}

void returnPhonemes(char* string, int hashIndex, char** phonemes) {
  
}
