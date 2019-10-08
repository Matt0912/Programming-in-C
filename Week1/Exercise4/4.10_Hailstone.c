#include <stdio.h>
#include <stdlib.h>

int isEven(int n) {
  if (n % 2 == 0) {
    return 1;
  } else {
    return 0;
  }
}

int main(void) {
  int i, n = 0;
  printf("Enter an integer: ");
  scanf("%d", &n);
  for (i=0; i<30; i++) {
    if (isEven(n)) {
      n = n/2;
    } else if (isEven(n) == 0) {
      n = 3*n + 1;
    }
    printf("%d\n", n);
  }



  return 0;
}
