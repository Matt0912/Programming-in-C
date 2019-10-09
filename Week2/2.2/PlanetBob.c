#include <stdio.h>
#include <assert.h>
#include <ctype.h>

int isVowel(int c);
int isPrime(int num);
void test(void);

int main(void) {
  char letter1 = 'a', letter2 = 'b';
  int i = 0, nameSum = 0;

  for (letter1 = 'a'; letter1 <= 'z'; letter1++) {
    for (letter2 = 'a'; letter2 <= 'z'; letter2++) {
      nameSum = 2*(letter1 - 'a' + 1) + (letter2 - 'a' + 1);
      if (isPrime(nameSum)){
        if (isVowel(letter1) == 0){
          if (isVowel(letter2) == 1){
              i++;
              printf("%4d: %c%c%c\n", i, letter1, letter2, letter1);
            }
          }
        else if (isVowel(letter1) == 1) {
          if (isVowel(letter2) == 0) {
            i++;
            printf("%4d: %c%c%c\n", i, letter1, letter2, letter1);
          }
        }
      }
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

int isPrime(int num) {
  int factor = 2;
  if (num < 2) {
    return 0;
  }
  while (factor <= num/2) {
    if (num % factor == 0) {
      return 0;
    }
    else {
      factor++;
    }
  }
  return 1;
}

void test(void) {
  assert(isPrime(53) == 1);
  assert(isPrime(100) == 0);
  assert(isPrime(1) == 0);
  assert(isPrime(-5) == 0);
  assert(isPrime(4) == 0);

  assert(isVowel('i') == 1);
  assert(isVowel('j') == 0);
  assert(isVowel('!') == 0);
}
