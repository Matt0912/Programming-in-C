#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define INITSIZE 101
enum bool {FALSE, TRUE};

void test(void);
int isPrime(int num);
int hashGenerator(char* string);

int main(void) {
  test();


  return FALSE;
}

void test(void) {
  assert(isPrime(53) == 1);
  assert(isPrime(100) == 0);
  assert(isPrime(1) == 0);
  assert(isPrime(-5) == 0);
  assert(isPrime(INITSIZE) == 1);

  assert(hashGenerator("a") == 1);
  assert(hashGenerator("aa") == 27);

}

int hashGenerator(char* string) {
  int hash = 0, length = strlen(string), i=0, add;
  while (i < length) {
    printf("%d %d\n", length, i);
    add = string[i] -'a'+ 1;
    hash += pow(add,(length-i)) + pow(26, length-i-1);
    printf("%s : %d\n", string, hash);
    i++;
  }
  return hash;
}

int isPrime(int num) {
  int factor = 2;
  if (num < 2) {
    return FALSE;
  }
  while (factor < num/2) {
    if (num % factor == 0) {
      return FALSE;
    }
    factor++;
  }
  return TRUE;
}
