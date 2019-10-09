#include <stdio.h>
#include <assert.h>

void test(void);
float nextNumber(int i);

int main(void) {
  int i, n = 10000000;
  float approximation = 4.0;
  test();
  for (i=1;i<n;++i) {
    approximation += nextNumber(i);
    printf("pi is equal to: %0.8f\n", approximation);
  }

  return 0;
}

float nextNumber(int i) {
  float nextNumber = 0, denominator = i*2 + 1;
  if (i%2==0) {
    nextNumber = 4/denominator;
  } else {
    nextNumber = -4/denominator;
  }
  return nextNumber;
}

void test(void) {
  assert(nextNumber(1) + 1.3333 < 0.005);
  assert(nextNumber(3) + 0.5714 < 0.005);
}
