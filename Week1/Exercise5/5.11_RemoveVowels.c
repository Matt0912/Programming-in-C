#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) {

  int c;
  while ((c = getchar()) != EOF) {
    if (isupper(c)) {
      c = tolower(c);
      if (isVowel(c) == 0) {
        putchar(toupper(c));
      }
    }
    else if (islower(c)) {
      if (isVowel(c) == 0) {
        putchar(c);
      }
    }
    else if (isspace(c)) {
      putchar(c);
    }
  }
  return 0;
}

int isVowel(int c) {
  if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') {
    return 1;
  } else {
    return 0;
  }
}
