#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define SIZE 3
#define SPACE ' '

enum bool {FALSE, TRUE};

void test(void);
int checkValid(char userInput[]);
void initialiseBoard(char board[SIZE][SIZE], char userInput[]);
void printBoard(char board[SIZE][SIZE]);

int main(void) {
  test();


  return 0;
}

void test(void) {
  char userInput[] = "4312 8657";
  char board[SIZE][SIZE];

  assert(checkValid(userInput) == TRUE);
  assert(checkValid("567123 48") == TRUE);
  assert(checkValid("56712348 ") == TRUE);
  assert(checkValid(" 56712348") == TRUE);
  assert(checkValid(" 5671234") == FALSE);
  assert(checkValid(" 56712!4") == FALSE);
  assert(checkValid(" 56710042") == FALSE);
  initialiseBoard(board, userInput);
  printBoard(board);

}

int checkValid(char userInput[]) {
  int length = strlen(userInput), i = 0, count[9] = {0}, val;
  char c;
  if (length != SIZE*SIZE) {
    /* fprintf(stderr, "INVALID INPUT LENGTH - SHOULD CONTAIN NUMBERS 0 - 8 "
    "AND A SPACE: E.G. 1234 5678\n"); */
    return FALSE;
    exit(1);
  }
  while (userInput[i]) {
    if (isdigit(userInput[i]) || userInput[i] == SPACE) {
      if (userInput[i] == '9' || userInput[i] == '0') {
        return FALSE;
        exit(1);
      }
    else {
      i++;
    }
  }
    else {
      return FALSE;
      exit(1);
    }
  }
  for (i=0; i<length; i++) {
    if (userInput[i] == SPACE) {
      count[0]++;
    } else {
      c = userInput[i];
      val = atoi(&c);
      count[val]++;
    }
  }
  for (i=0; i<9; i++) {
    if (count[i] > 1) {
      return FALSE;
    }
  }
  return TRUE;
}

void initialiseBoard(char board[SIZE][SIZE], char userInput[]) {
  int i = 0, x, y;
  while (userInput[i]) {
    for (x=0; x<SIZE; x++) {
      for (y=0; y<SIZE; y++) {
        board[y][x] = userInput[i];
        i++;
      }
    }
  }
}

void printBoard(char board[SIZE][SIZE]) {
  int x, y;
  for (x=0; x<SIZE; x++) {
    for (y=0; y<SIZE; y++) {
      fprintf(stdout, "%c ", board[y][x]);
    }
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"\n");
}
