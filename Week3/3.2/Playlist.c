#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void populateArray(int listSize, int *list);
void randomiseArray(int listSize, int *list);

int main(void) {
  int *list, listSize, i;
  srand(time(NULL));
  list = (int *) malloc(100);
  printf("How many songs required? ");
  scanf("%d", &listSize);
  populateArray(listSize, list);
  randomiseArray(listSize, list);

  for (i=0;i<listSize;i++) {
    printf("%d ", list[i]);
  }
  printf("\n");

  return 0;
}

void populateArray(int listSize, int *list) {
  int i;
  for (i=0;i<listSize;i++) {
    list[i] = i+1;
    printf("%d ", list[i]);
  }
  printf("\n");
}

void randomiseArray(int listSize, int *list) {
  int position, n = listSize, temp;
  while (n > 1) {
    position = rand() % (n-1);
    temp = list[position];
    list[position] = list[n-1];
    list[n-1] = temp;
    n--;
  }
}
