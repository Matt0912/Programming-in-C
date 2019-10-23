#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#define ROWS 40
#define COLUMNS 40
/* Max COLUMNS */
#define MAXCHAR 100
#define GENERATIONS 1000
#define PASS 1
#define FAIL 0
/* Delay in MILLISECONDS */
#define DELAY 250
enum states {empty = ' ', head = 'H', tail = 't', copper = 'c'};

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
  /* If user doesn't enter 2 arguments, defaults to wirewcircuit1.txt */
  if(argc!=2) {
    argv[1] = "wirewcircuit1.txt";
    fprintf(stderr,"ERROR: Incorrect usage, defaulted to wirewcircuit1.txt\n");
  }

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "ERROR - Unable to open file %s\n", argv[1]);
    exit(1);
  }

  readToArray(array, fp);
  printArray(array);
  fclose(fp);

  while (i<GENERATIONS) {
    generateNewArray(array, newArray);
    printArray(newArray);
    swapArrays(array, newArray);
    delay(DELAY);
    i++;
  }

  return 0;
}

void test(void) {
  char testArray[ROWS][COLUMNS] = {{' ','c',' ',' '},
                                   {' ','t','H','c'},
                                   {' ','H',' ','H'}};

  assert(checkValid('u') == FAIL);
  assert(checkValid('7') == FAIL);
  assert(checkValid('!') == FAIL);
  assert(checkValid('c') == PASS);

  assert(newCell(empty, 2) == empty);
  assert(newCell(head, 1) == tail);
  assert(newCell(copper, 2) == head);
  assert(newCell(tail, 2) == copper);

  assert(countHeads(testArray, 0, 0) == 0);
  assert(countHeads(testArray, 1, 1) == 2);
  assert(countHeads(testArray, 2, 2) == 3);
  assert(countHeads(testArray, 10, 10) == 0);

  assert(access(-1,6) == FAIL);
  assert(access(1,45) == FAIL);
  assert(access(41,0) == FAIL);
  assert(access(20,39) == PASS);

}

/*Reads in line by line, and appends char by char - also tests for invalid chars */
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

/* Returns value of corresponding new cell in new array */
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

/* Counts number of heads in 8 surrounding cells */
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

/* If countHeads is trying to access area outside 40x40 array, return FAIL */
int access(int x, int y) {
  if (x<0 || x>=COLUMNS) {
    return FAIL;
  }
  if (y<0 || y>=ROWS) {
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

/* Copies newArray in to array */
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
