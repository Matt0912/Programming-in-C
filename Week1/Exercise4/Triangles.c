#include <stdio.h>
#include <stdlib.h>

int triangle(float a, float b, float c) {
    float temp = c;
    if (a > b && a > c) {
      c = a;
      a = temp;
    }
    else if (b > a && b > c) {
      c = b;
      b = temp;
    }
    if (a > b) {
      temp = b;
      b = a;
      a = temp;
    }
    printf("(%f, %f, %f)\n", a,b,c);
    if (c > a + b) printf("Impossible\n");
    else if (abs(c - (a + b)) < 0.005) printf("Flat\n");
    else if (abs(a - b) < 0.005 && abs(b - c) < 0.005) printf("Equilateral\n");
    else if (abs(a - b) < 0.005 || abs(b - c) < 0.005 || abs(c - a) < 0.005) printf("Isosceles\n");
    else if (a*a + b*b - c*c < 0.005) printf("Right\n");
    else if (abs(a*a + b*b - c*c) > 0.005 && abs(a - b) > 0.005 && abs(b - c) > 0.005 && abs(a - c) > 0.005) printf("Scalene\n");
    return 1;
}

int main(void) {
  float a, b, c;
  int error = 1;
  while (error == 1) {
    printf("Please input 3 values for your triangle: ");
    scanf("%f %f %f", &a, &b, &c);
    if (a < 0.000000005 || b < 0.000000005 || c < 0.000000005) {
      printf("INVALID INPUT - PLEASE TRY AGAIN\n");
    } else {
      error = 0;
      triangle(a, b, c);
    }
  }


  return 0;
}
