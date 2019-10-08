#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NDEBUG
#include <assert.h>

void test(void);
int inCircle(float x, float y);

int main(void) {
  /* ha is the number of darts inside the square, hc is the number of darts in
  // the circle */
  int i, ha = 0, hc = 0;
  float x, y;
  srand(time(NULL));

  for (i=0; i < 10000; i++) {
    x = (rand() % 2) - 1;
    y = (rand() % 2) - 1;
    printf("x is %f, y is %f\n", x, y);
    if (inCircle(x,y)) {
      hc++;
      ha++;
    } else {
      ha++;
    }
  }

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
