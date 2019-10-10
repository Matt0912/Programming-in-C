#include <stdio.h>
#include <assert.h>

void test(int *cookingTimes);
int buttonPresses(int time, int *cookingTimes);

int main(void) {
  int cookingTimes[] = {600, 60, 10}, time = 0, presses;
  test(cookingTimes);
  printf("Type the time required: ");
  scanf("%d", &time);

  presses = buttonPresses(time, cookingTimes);
  printf("Presses needed: %d\n", presses);

  return 0;
}

int buttonPresses(int time, int *cookingTimes) {
  int i, remainder = time, presses = 0, n = 3; /* n = size of cooking times array */
  for (i=0; i < n; i++) {
    if (time >= cookingTimes[i]){
      remainder = time % cookingTimes[i];
      if (i == n-1) {
        if (time+cookingTimes[i]-remainder == cookingTimes[i-1]) {
          presses = 1;
          return presses;
        }
      }
      presses += (time-remainder)/cookingTimes[i];
      time = remainder;
    }
  }
  if (remainder > 0) {
    presses += 1;
  }
  return presses;
}

void test(int *cookingTimes) {
  assert(buttonPresses(25, cookingTimes) == 3);
  assert(buttonPresses(705, cookingTimes) == 7);
  assert(buttonPresses(5, cookingTimes) == 1);
  assert(buttonPresses(60, cookingTimes) == 1);
  assert(buttonPresses(55, cookingTimes) == 1);
}
