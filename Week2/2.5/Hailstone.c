#include <stdio.h>
#include <assert.h>

void test(void);
int isEven(int n);

int main(void) {
  int i, count, max = 50000, num, maxCount = 0, longestSequence = 0;
  int maxNum = 0, largestSequence = 0;

  for (i=2; i<max; i++) {
    count = 0;
    num = i;
    while (num != 1) {
      count++;
      if (count > maxCount) {
        maxCount = count;
        longestSequence = i;
      }
      if (num > maxNum) {
        maxNum = num;
        largestSequence = i;
      }
      if (isEven(num)) {
        num = num/2;
      } else {
        num = 3*num + 1;
      }
    }
  }
  printf("The number which produced the largest sequence of %d was %d\n",
  maxCount, longestSequence);
  printf("The largest number in any sequence is %d, in %d's sequence\n",
  maxNum, largestSequence);

  return 0;
}

int isEven(int n) {
  if (n % 2 == 0) {
    return 1;
  } else {
    return 0;
  }
}

void test(void) {
  assert(isEven(16) == 1);
  assert(isEven(7) == 0);
}
