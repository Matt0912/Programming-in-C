#include <stdio.h>

int main(void)
{
  int a, b, c, sum, total;
  printf("Input three integers: \n");
  scanf("%d\n", &a);
  scanf("%d\n", &b);
  scanf("%d", &c);

  sum = a + b + c;
  printf("%d\n",sum);
  total = 2*sum + 7;
  printf("%d\n", total);

  printf("Twice the sum of integers plus 7 is %d\n", total);

  return 0;
}
