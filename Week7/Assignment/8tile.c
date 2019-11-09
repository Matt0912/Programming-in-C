#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define SIZE 3
#define SPACE ' '
#define QUEUESIZE 50

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
  char userInput[] = "43128657 ";
  char board[SIZE][SIZE];
  Board Queue[QUEUESIZE];
  int x = 0, y = 0;

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

  initialiseBoard(board, userInput, Queue);
  findSpace(board, &x, &y);
  assert(x == 1 && y == 1);
  swap(&board[1][1], &board[1][2]);
  findSpace(board, &x, &y);
  assert(x == 2 && y == 1);

  assert(possiblePermutations(1,1) == 4);
  assert(possiblePermutations(1,2) == 3);
  assert(possiblePermutations(0,0) == 2);

  initialiseBoard(board, userInput, Queue);
  addToQueue(Queue, board, 0, 1, 0);
  assert(isDuplicate(Queue, board) == TRUE);

  printBoard(Queue[0].board);
  printBoard(Queue[1].board);

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

int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard) {
  int possPerms, spaceX, spaceY;
  char newBoard[SIZE][SIZE];

  copyBoard(currentBoard.board, newBoard);

  if (isComplete(currentBoard)) {
    return TRUE;
  }

  findSpace(newBoard, spaceX, spaceY);
  possPerms = possiblePermutations(spaceX, spaceY);

  while (i < possPerms) {
    /*IF WE CAN SWAP TO THE RIGHT */
    if (access(spaceX + 1, spaceY)) {
      swap(newBoard[spaceY][spaceX+1], newBoard[spaceY][spaceX])
      if (!isDuplicate(Queue, newBoard)) {
        addToQueue(Queue, newBoard, );
      }
    }


    /*IF WE CAN SWAP ABOVE THE SPACE*/
    if (access(spaceX, spaceY - 1)) {
      swap(newBoard[spaceY-1][spaceX], newBoard[spaceY][spaceX])
      printBoard(newBoard);
    }

    /*IF WE CAN SWAP TO THE LEFT */
    if (access(spaceX-1, spaceY)) {
      swap(newBoard[spaceY][spaceX-1], newBoard[spaceY][spaceX])
      printBoard(newBoard);
    }

    /*IF WE CAN SWAP BELOW THE SPACE*/
    if (access(spaceX, spaceY + 1)) {
      swap(newBoard[spaceY+1][spaceX], newBoard[spaceY][spaceX])
      printBoard(newBoard);
    }
  }


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
    return FAIL;
  }
  if (y<0 || y>=SIZE) {
    return FAIL;
  }
  return PASS;
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
