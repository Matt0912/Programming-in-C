/*Code that prints out first n number of primes */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int isPrime(int num);
void printPrimes(int n);
void test(void);

int main(void) {
  int n = 0;
  test();
  printf("Enter number of primes: ");
  scanf("%d", &n);

  printPrimes(n);

  return 0;
}

int isPrime(int num) {
  int factor = 2;
  if (num < 2) {
    return 0;
  }
  while (factor < num/2) {
    if (num % factor == 0) {
      return 0;
    }
    else {
      factor++;
    }
  }
  return 1;
}

void printPrimes(int n) {
  /* Start at 3 and increment in 2 (below) to eliminate all even numbers */
  int count = 1, num = 3;
  printf("%d: 2\n", count);
  while (count < n) {
    if (isPrime(num)) {
      count++;
      printf("%d: %d\n", count, num);
    }
    num += 2;
  }
}

void test(void) {
  assert(isPrime(53) == 1);
  assert(isPrime(100) == 0);
  assert(isPrime(1) == 0);
  assert(isPrime(-5) == 0);
}
