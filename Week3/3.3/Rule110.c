#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#define ARRAYWIDTH 75
#define ARRAYLENGTH 40

void nextLine(int *currentArray, int *newArray);
int numberTests(int first, int second, int third);
void initialiseArray(int *currentArray, int userInput);
void printLine(int *currentArray);
void swapArrays(int *currentArray, int *newArray);
void test(void);

int main(void) {
  int i=0, currentArray[ARRAYWIDTH] = {0}, newArray[ARRAYWIDTH], userInput = 0;
  srand(time(NULL));
  test();

  initialiseArray(currentArray, userInput);
  printLine(currentArray);

  while(i < ARRAYLENGTH) {
    i++;
    nextLine(currentArray, newArray);
    swapArrays(currentArray, newArray);
    printLine(currentArray);
  }

  return 0;
}

void swapArrays(int *currentArray, int *newArray) {
  int i;
  for (i=0;i<ARRAYWIDTH;i++) {
    currentArray[i] = newArray[i];
  }
}

void nextLine(int *currentArray, int *newArray) {
  int i;
  for (i=1;i<ARRAYWIDTH-1;i++) {
    newArray[i] = numberTests(currentArray[i-1], currentArray[i], currentArray[i+1]);
  }
}

int numberTests(int first, int second, int third) {
  if (first == 1 && second == 1 && third == 1) {
    return 0;
  }
  else if (first == 1 && second == 0 && third == 0) {
    return 0;
  }
  else if (first == 0 && second == 0 && third == 0) {
    return 0;
  }
  else if ((first != 0 && first != 1) || (third != 0 && third != 1)) {
    return 0;
  }
  else {
    return 1;
  }
}

void printLine(int *currentArray) {
  int i;
  for (i=1;i<ARRAYWIDTH-1;i++){
    printf("%d", currentArray[i]);
  }
  printf("\n");
}

void initialiseArray(int *currentArray, int userInput) {
  int i;
  /*Input 0 means single 1 in the middle*/
  if (userInput == 0) {
    currentArray[ARRAYWIDTH/2] = 1;
  }
  /*Input 1 means randomly select zeros or ones for the first line */
  else if (userInput == 1) {
    for (i=1;i<ARRAYWIDTH-1;i++) {
      if (rand()%10 < 5) {
        currentArray[i] = 1;
      }
    }
  }
  else {
    printf("INVALID INPUT");
  }
}

void test(void) {
  assert(numberTests(1,1,1) == 0);
  assert(numberTests(1,0,0) == 0);
  assert(numberTests(0,0,0) == 0);
  assert(numberTests(1,1,0) == 1);
  assert(numberTests(0,1,1) == 1);

}
