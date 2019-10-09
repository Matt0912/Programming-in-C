#include <stdio.h>
#define TOTALNUMBER 100

int main(void) {
  int sum = 0, i, n, triangleNumber;
  for (i = 1; i <= TOTALNUMBER; i++) {
    sum += i;
    printf("Triangle number %d is: %d\n", i, sum);
  }
  printf("Please enter which triangle number you would like: ");
  scanf("%d", &n);
  triangleNumber = (n*(n+1))/2;
  printf("The %dth triangle number is: %d\n", n, triangleNumber);
  return 0;
}
