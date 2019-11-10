#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define SIZE 3
#define SPACE ' '
#define QUEUESIZE 25000

enum bool {FALSE, TRUE};
enum moves {START, UP, RIGHT, DOWN, LEFT};

typedef struct board {
  char board[SIZE][SIZE];
  int permNumber;
  int globalIndex;
  int parent;
} Board;

void test(void);
int isValid(char userInput[]);
int isComplete(char board[SIZE][SIZE]);
int isDuplicate(Board Queue[QUEUESIZE], char currentBoard[SIZE][SIZE]);
void initialiseBoard(char board[SIZE][SIZE], char userInput[], Board Queue[QUEUESIZE]);
void addToQueue(Board Queue[QUEUESIZE], char board[SIZE][SIZE], int permNumber, int index, int parent);
void findSpace(char board[SIZE][SIZE], int *x, int *y);
int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard, int *currentIndex);
int possiblePermutations(int x, int y);
int access(int x, int y);
void copyBoard(char currentBoard[SIZE][SIZE], char newBoard[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE]);
void swap(char *a, char *b);

int main(void) {
  test();


  return 0;
}

void test(void) {
  char userInput[] = "1 2753846";
  char board[SIZE][SIZE];
  static Board Queue[QUEUESIZE];
  int x = 0, y = 0, i, currentIndex;

  assert(isValid(userInput) == TRUE);
  assert(isValid("567123 48") == TRUE);
  assert(isValid("56712348 ") == TRUE);
  assert(isValid(" 56712348") == TRUE);
  assert(isValid(" 5671234") == FALSE);
  assert(isValid(" 56712!4p") == FALSE);
  assert(isValid(" 56710042") == FALSE);
  assert(isValid("123 5 6 7") == FALSE);

  initialiseBoard(board, userInput, Queue);
  assert(isComplete(board) == FALSE);
  initialiseBoard(board, "12345678 ", Queue);
  assert(isComplete(board) == TRUE);
  initialiseBoard(board, " 23456781", Queue);
  assert(isComplete(board) == FALSE);

  swap(&board[2][1], &board[2][2]);
  assert(isComplete(board) == FALSE);

  initialiseBoard(board, "4312 8657", Queue);
  findSpace(board, &x, &y);
  assert(x == 1 && y == 1);
  swap(&board[1][1], &board[1][2]);
  findSpace(board, &x, &y);
  assert(x == 2 && y == 1);

  assert(possiblePermutations(1,1) == 4);
  assert(possiblePermutations(1,2) == 3);
  assert(possiblePermutations(0,0) == 2);

  /*initialiseBoard(board, userInput, Queue);
  addToQueue(Queue, board, 0, 1, 0);
  assert(isDuplicate(Queue, board) == TRUE);*/

  initialiseBoard(board, userInput, Queue);
  i = 0;
  currentIndex = 0;
  while (i < QUEUESIZE/4) {
    printf("Queue position is: %d\n", i);
    if (permuteBoard(Queue, Queue[i], &currentIndex) == TRUE) {
      i = QUEUESIZE;
    }
    i++;
  }
/*
  printf("Queue 0 = \n");
  printBoard(Queue[0].board);
  printf("Queue 1 = \n");
  printBoard(Queue[1].board);
  printf("Queue 2 = \n");
  printBoard(Queue[2].board);
  printf("Queue 3 = \n");
  printBoard(Queue[3].board);
  printf("Queue 4 = \n");
  printBoard(Queue[4].board);

  printf("Permutation: %d\n", Queue[0].permNumber);
  printf("Index: %d\n", Queue[0].globalIndex);
  printf("Parent: %d\n \n", Queue[0].parent);
  printf("Queue 1 = \n");
  printBoard(Queue[1].board);
  printf("Queue 2 = \n");
  printBoard(Queue[2].board);
  printf("Permutation: %d\n", Queue[2].permNumber);
  printf("Index: %d\n", Queue[2].globalIndex);
  printf("Parent: %d\n \n", Queue[2].parent);
  printf("Queue 3 = \n");
  printBoard(Queue[3].board);
  printf("Queue 4 = \n");
  printBoard(Queue[4].board);
  printf("Permutation: %d\n", Queue[4].permNumber);
  printf("Index: %d\n", Queue[4].globalIndex);
  printf("Parent: %d\n \n", Queue[4].parent);
*/

}

/* TESTED */
int isValid(char userInput[]) {
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

/* TESTED: Checks if board is in final state of 123 456 78  */
int isComplete(char board[SIZE][SIZE]) {
  int x, y;
  char completeBoard[SIZE][SIZE] = {{"123"},
                                    {"456"},
                                    {"78 "}};
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      if (board[y][x] != completeBoard[y][x]) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/* NEED TO TEST */
int isDuplicate(Board Queue[QUEUESIZE], char currentBoard[SIZE][SIZE]) {
  int x, y, i = 0, count;
  while (i < QUEUESIZE) {
    count = 0;
    for (y=0; y<SIZE; y++) {
      for (x=0; x<SIZE; x++) {
        if (Queue[i].board[y][x] != currentBoard[y][x]) {
          i++;
        }
        else if (Queue[i].board[y][x] == currentBoard[y][x]) {
          count++;
        }
        if (count == SIZE*SIZE) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

/* Populates initial board with user input */
void initialiseBoard(char board[SIZE][SIZE], char userInput[], Board Queue[QUEUESIZE]) {
  int i = 0, x, y;
  while (userInput[i]) {
    for (y=0; y<SIZE; y++) {
      for (x=0; x<SIZE; x++) {
        board[y][x] = userInput[i];
        i++;
      }
    }
  }
  addToQueue(Queue, board, 0, 0, 0);
}

void addToQueue(Board Queue[QUEUESIZE], char board[SIZE][SIZE], int permNumber, int index, int parent) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      Queue[index].board[y][x] = board[y][x];
    }
  }
  printf("Index is: %d \n ", index);
  Queue[index].permNumber = permNumber;
  Queue[index].globalIndex = index;
  Queue[index].parent = parent;
}

void findSpace(char board[SIZE][SIZE], int *x, int *y) {
  int row, col;
  for (row=0; row<SIZE; row++) {
    for (col=0; col<SIZE; col++) {
      if (board[row][col] == SPACE) {
        *x = col;
        *y = row;
      }
    }
  }
}

int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard, int *currentIndex) {
  int spaceX = -1, spaceY = -1, count = 0, i;
  char newBoard[SIZE][SIZE];

  printf("PARENT BOARD IS: \n");
  printBoard(currentBoard.board);

  if (isComplete(currentBoard.board)) {
    fprintf(stdout, "SOLUTION FOUND! \n\n");
    /*while (currentBoard.globalIndex != 0) {

    }*/

    return TRUE;
  }

  findSpace(currentBoard.board, &spaceX, &spaceY);

    for (i = -1; i <= 1; i = i+2) {
      copyBoard(currentBoard.board, newBoard);
      /*IF WE CAN SWAP TO THE RIGHT OR LEFT */
      if (access(spaceX + i, spaceY)) {
        swap(&newBoard[spaceY][spaceX+i], &newBoard[spaceY][spaceX]);
        if (!isDuplicate(Queue, newBoard)) {
          count++;
          (*currentIndex)++;
          printf("Queue position is: %d\n", currentBoard.globalIndex);
          printf("Parent is: %d \n", currentBoard.parent);
          printBoard(newBoard);
          addToQueue(Queue, newBoard, count, *currentIndex, currentBoard.globalIndex);
        }
      }

      copyBoard(currentBoard.board, newBoard);

      /*IF WE CAN SWAP BELOW OR ABOVE THE SPACE*/
      if (access(spaceX, spaceY + i)) {
        swap(&newBoard[spaceY+i][spaceX], &newBoard[spaceY][spaceX]);
        if (!isDuplicate(Queue, newBoard)) {
          count++;
          (*currentIndex)++;
          printf("Queue position is: %d\n", currentBoard.globalIndex);
          printf("Parent is: %d \n", currentBoard.parent);
          printBoard(newBoard);
          addToQueue(Queue, newBoard, count, *currentIndex, currentBoard.globalIndex);
        }
      }
    }


  return FALSE;

}

void copyBoard(char currentBoard[SIZE][SIZE], char newBoard[SIZE][SIZE]) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      newBoard[y][x] = currentBoard[y][x];
    }
  }
}
/* If permuteBoard is trying to access area outside 3x3 array, return FAIL */
int access(int x, int y) {
  if (x<0 || x>=SIZE) {
    return FALSE;
  }
  if (y<0 || y>=SIZE) {
    return FALSE;
  }
  return TRUE;
}

/* Returns number of possible permutations depending on position of space */
int possiblePermutations(int x, int y) {
  int permutations[SIZE][SIZE] = {{2, 3, 2},
                                  {3, 4, 3},
                                  {2, 3, 2}};
  return permutations[y][x];
}

void printBoard(char board[SIZE][SIZE]) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      fprintf(stdout, "%c ", board[y][x]);
    }
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"\n");
}

void swap(char *a, char *b) {
  char temp;
  temp = *a;
  *a = *b;
  *b = temp;
}
