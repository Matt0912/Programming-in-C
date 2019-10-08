#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int i, randVar, minus_cnt, plus_cnt, difference;
  int median = RAND_MAX/2;
  minus_cnt = 0;
  plus_cnt = 0;

  for (i=0; i<=1000; i++) {
    randVar = rand();
    /* printf("%d\n", randVar); */
    if (randVar < median) {
      minus_cnt++;
    }
    if (randVar > median) {
      plus_cnt++;
    }
    difference = plus_cnt - minus_cnt;
    printf("%d\n", difference);
  }
  printf("%d\n", RAND_MAX);
  return 0;
}
