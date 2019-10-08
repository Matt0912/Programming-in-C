#include <stdio.h>
#include <stdlib.h>

int maxValue(int max) {
  int i, input, previous_input;

  scanf("Enter integer: %d\n", &input);
  if (input % 2 == 0) {
    previous_input = input;
  } else{
    printf("No odd numbers!\n");
    previous_input = 0;
    max += 1;
  }

  for (i=0;i < max; i++) {
    printf("Enter integer: ");
    scanf("%d", &input );
    if (input > previous_input) {
      if (input % 2 == 0)
        previous_input = input;
      else {
        printf("No odd numbers!\n");
        i--;
      }
    }
  }
  return previous_input;
}

int main()
{
  int max, max_input;
  printf("How many numbers do you wish to enter? ");
  scanf("%d", &max);

  max_input = maxValue(max);
  printf("The largest number was: %d\n", max_input);

  return 0;
}
