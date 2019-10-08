#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

  float input, result;
  printf("Input a value for x: ");
  scanf("%f", &input);

  result = sin(input)*sin(input) + cos(input)*cos(input);
  printf("Result = %f\n", result);

  return 0;
}
