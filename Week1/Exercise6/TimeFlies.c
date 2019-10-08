#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

  int hours1, minutes1, hours2, minutes2, time1, time2, difference, error = 1;
  int hoursResult, minutesResult;
  while (error == 1) {
    printf("Enter two times (hh:mm): ");
    scanf("%d:%d" "%d:%d", &hours1, &minutes1, &hours2, &minutes2);
    if (minutes1 > 60 || minutes2 > 60 || hours1 > 24 || hours2 > 24) {
      printf("PLEASE ENTER VALID TIMES\n");
    } else {
      error = 0;
    }
  }
  time1 = 60*hours1 + minutes1;
  time2 = 60*hours2 + minutes2;

  if (time2 <= time1) {
    /*Add a day so that the second time is always in the future*/
    time2 += 24*60;
  }
  difference = time2-time1;
  hoursResult = difference/60;
  minutesResult = difference%60;
  printf("Difference is: %d:%d\n", hoursResult, minutesResult);
  return 0;
}
