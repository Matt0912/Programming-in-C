#include <stdio.h>
#include <stdlib.h>

float maxValue(int max) {
  int i;
  float input, previous_input;
  printf("%d\n", max);
  scanf("Enter number: %f\n", &input);
  previous_input = input;

  for (i=0;i < max; i++) {
    printf("Enter number: ");
    scanf("%f", &input );
    if (input > previous_input) {
      previous_input = input;
    }
  }
  return previous_input;
}

int main()
{
  float max_input;
  int max = 3;
  printf("How many numbers do you wish to enter? ");
  scanf("%d", &max);

  max_input = maxValue(max);
  printf("The largest number was: %f\n", max_input);

  return 0;
}
