#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NDEBUG
#include <assert.h>
#define ACCURACY 0.0000001

void test(void);
int inCircle(float x, float y);

int main(void) {
  /* ha is the number of darts inside the square, hc is the number of darts in
  // the circle */
  int i = 0, ha = 0, hc = 0;
  float x, y, approximation=0.0;
  srand(time(NULL));
  while (fabs(approximation - 3.14159265) > ACCURACY) {
    i++;
    x = 2*(float)rand()/(float)(RAND_MAX)-1;
    y = 2*(float)rand()/(float)(RAND_MAX)-1;
    if (inCircle(x,y)) {
      hc++;
    }
    ha++;
    approximation = (float)4*(float)hc/(float)ha;
    printf("%d: Approximation to pi = %0.10f\n", i, approximation);
  }
  approximation = (float)4*(float)hc/(float)ha;
  printf("%d: Approximation to pi = %0.10f\n", i, approximation);
  return 0;
}

int inCircle(float x, float y) {
  int radius = 1;
  if (x*x + y*y <= radius*radius) {
    return 1;
  } else {
    return 0;
  }
}

void test(void) {
  assert(inCircle(50,40) == 0);
  assert(inCircle(0.67, 0.34) == 1);
  assert(inCircle(-0.67, -0.34) == 1);
}
