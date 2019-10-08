#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int isVowel(int c) {
  if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') {
    return 1;
  } else {
    return 0;
  }
}


int main(void) {

  int c;
  while ((c = getchar()) != EOF) {
    if (isupper(c)){
      c = tolower(c);
      if (isVowel(c)) {
        putchar(toupper(c));
      }
      else {
        putchar(c);
      }
    }
    else if (islower(c)) {
      if (isVowel(c)) {
        putchar(toupper(c));
      }
      else {
        putchar(c);
      }
    }
  }
  return 0;
}
