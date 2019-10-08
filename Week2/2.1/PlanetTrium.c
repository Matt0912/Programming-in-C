#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char nextLetter(char letter);
int isVowel(int c);

int main(void) {
  char letter1 = 'b', letter2 = 'a', letter3 = 'b';
  int i = 0;

  for (letter3 = 'b'; letter3 <= 'z'; letter3++) {
    if (isVowel(letter3) == 0){
      i++;
      printf("%d: %c%c%c\n", i, letter1, letter2, letter3);
    }
    if (letter3 == 'z') {
      letter3 = 'a';
      if (letter2 == 'u'){
        if (letter1 == 'z') {
          letter3 = 'z';
        }
        letter1 = nextLetter(letter1);
      }
      letter2 = nextLetter(letter2);
    }
  }

  return 0;
}

char nextLetter(char oldLetter) {
  char newLetter;
  newLetter = oldLetter;
  if (isVowel(oldLetter)) {
    newLetter++;
    if (oldLetter == 'u') {
      newLetter = 'a';
    }
    while (isVowel(newLetter) == 0){
      newLetter++;
    }
  }
  else {
    if (oldLetter == 'z') {
      newLetter = 'a';
    }
    newLetter++;
    if (isVowel(newLetter)) {
      newLetter++;
    }
  }
  return newLetter;
}

int isVowel(int c) {
  if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') {
    return 1;
  } else {
    return 0;
  }
}
