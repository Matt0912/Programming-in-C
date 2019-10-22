#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#define ROWS 40
#define COLUMNS 40
#define MAXCHAR 100
#define GENERATIONS 1000
#define PASS 1
#define FAIL 0
/* Delay in MILLISECONDS */
#define DELAY 250
enum states {empty = ' ', head = 'H', tail = 't', copper = 'c'};
typedef enum states states;

void test(void);
void readToArray(char array[ROWS][COLUMNS], FILE *fp);
int checkValid(char c);
int countHeads(char array[ROWS][COLUMNS], int x, int y);
int access(int x, int y);
int newCell(int currentCell, int numHeads);
void printArray(char array[ROWS][COLUMNS]);
void swapArrays(char array[ROWS][COLUMNS], char newArray[ROWS][COLUMNS]);
void generateNewArray(char array[ROWS][COLUMNS], char newArray[ROWS][COLUMNS]);
void delay(int seconds);

int main(int argc, char **argv) {
  FILE* fp;
  char array[ROWS][COLUMNS], newArray[ROWS][COLUMNS];
  int i=0;

  test();

  if(argc==2) {
    fp = fopen(argv[1], "r");
  }
  else {
    printf("ERROR: Incorrect usage, defaulted to wirewcircuit1.txt\n");
    fp = fopen("wirewcircuit1.txt", "r");
  }

  if (fp == NULL) {
    fprintf(stderr, "ERROR - Unable to open file %s\n", argv[1]);
    exit(1);
  }

  readToArray(array, fp);
  printArray(array);

  while (i<GENERATIONS) {
    generateNewArray(array, newArray);
    printArray(newArray);
    swapArrays(array, newArray);
    delay(250);
    i++;
  }

  fclose(fp);
  return 0;
}

void test(void) {
  assert(newCell(empty, 2) == empty);
  assert(newCell(head, 1) == tail);
  assert(newCell(copper, 2) == head);
  assert(newCell(tail, 2) == copper);

  assert(access(-1,6) == FAIL);
  assert(access(1,45) == FAIL);
  assert(access(41,0) == FAIL);
  assert(access(20,40) == PASS);

  assert(checkValid('u') == FAIL);
  assert(checkValid('7') == FAIL);
  assert(checkValid('!') == FAIL);
  assert(checkValid('c') == PASS);
}

void readToArray(char array[ROWS][COLUMNS], FILE *fp) {
  int i, j=0;
  char str[MAXCHAR];

  while (fgets(str, MAXCHAR, fp) != NULL) {
    for (i=0;i<ROWS;i++) {
      if (checkValid(str[i])) {
        array[j][i] = str[i];
      }
      else {
        fprintf(stderr, "FILE CONTAINS INVALID INPUT!\n");
        exit(1);
      }
    }
    j++;
  }
}

int checkValid(char c) {
  if (c == empty || c==head || c==tail || c==copper) {
    return PASS;
  }
  else {
    return FAIL;
  }
}

int newCell(int currentCell, int numHeads) {
  if (currentCell == empty) {
    return empty;
  }
  if (currentCell == head) {
    return tail;
  }
  if (currentCell == tail) {
    return copper;
  }
  if (currentCell == copper) {
    if (numHeads == 1 || numHeads == 2) {
      return head;
    }
    else {
      return copper;
    }
  }
  else {
    return FAIL;
  }
}

int countHeads(char array[ROWS][COLUMNS], int x, int y) {
  int i, j, numHeads = 0;
  for (i=-1; i<=1;i++) {
    for (j=-1; j<=1; j++) {
      if (array[y+i][x+j] == head && access(y+i, x+j)) {
        numHeads++;
      }
    }
  }
  return numHeads;
}

int access(int x, int y) {
  if (x<0 || x>COLUMNS) {
    return FAIL;
  }
  if (y<0 || y>ROWS) {
    return FAIL;
  }
  return PASS;
}

void generateNewArray(char array[ROWS][COLUMNS], char newArray[ROWS][COLUMNS]) {
  int x, y, numHeads=0;
  for (x=0;x<COLUMNS;x++) {
    for (y=0;y<ROWS;y++) {
      numHeads = countHeads(array, x, y);
      newArray[y][x] = newCell(array[y][x], numHeads);
    }
  }
}

/* Copies new array in to old array */
void swapArrays(char array[ROWS][COLUMNS], char newArray[ROWS][COLUMNS]) {
  int x, y;
  for (x=0;x<COLUMNS;x++) {
    for (y=0;y<ROWS;y++) {
      array[y][x] = newArray[y][x];
    }
  }
}

void printArray(char array[ROWS][COLUMNS]) {
  int x, y;
  for (x=0;x<COLUMNS;x++) {
    for (y=0;y<ROWS;y++) {
      fprintf(stdout, "%c", array[x][y]);
    }
    fprintf(stdout, "\n");
  }
}

/* Function to delay each array print */
void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
