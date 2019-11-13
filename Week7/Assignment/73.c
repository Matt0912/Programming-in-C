/* 8tile solver code
// This code takes an input from the user in string form e.g. "21435687 "
// which populates a 2D 3 x 3 grid, then solves it through calculating
// all possible permutations of the original grid until it is in the form
// "12345678 " - https://en.wikipedia.org/wiki/Sliding_puzzle */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

#define SIZE 3
#define SPACE ' '
#define DELAY 500

enum bool {FALSE, TRUE};

typedef struct node {
  char board[SIZE][SIZE];
  int index;
  struct node *parent;
  struct node *prevNode;
} Node;

void test(void);
int isValid(char userInput[]);
int isComplete(char board[SIZE][SIZE]);
int isDuplicate(Node currentNode);
void initialiseBoard(char board[SIZE][SIZE], char userInput[]);
Node* newNode(char board[SIZE][SIZE], int index, Node *parent, Node *prevNode);
void findSpace(char board[SIZE][SIZE], int *x, int *y);
/*int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard,
                 int *currentIndex, Board Solutions[MAXMOVES]);
void traceParents(Board Queue[QUEUESIZE], Board Solutions[MAXMOVES], int currentIndex); */
int access(int x, int y);
void copyBoard(char newBoard[SIZE][SIZE], char currentBoard[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE]);
void swap(char *a, char *b);
void delay(int seconds);

int main(int argc, char **argv) {
  char board[SIZE][SIZE], userInput[10] = "12345678 ";
  Node *point;

  test();
  /*
  if (argc == 2) {
    if (isValid(argv[1])) {
      strcpy(userInput, argv[1]);
    }
    else {
      fprintf(stderr, "INVALID INPUT - SHOULD ONLY CONTAIN NUMBERS 0 - 8 "
      "AND A SPACE: E.G. '1234 5678'\n");
      exit(1);
    }
  }
  else if (argc != 2) {
    fprintf(stderr,"ERROR: Incorrect usage: e.g ./8tile '12345678 '\n");
    exit(1);
  }
  */
  initialiseBoard(board, userInput);
  point = newNode(board, 0, NULL, NULL);
  printBoard(point->board);
  /*
  while (i < QUEUESIZE/3) {
    if (permuteBoard(Queue, Queue[i], &currentIndex, Solutions) == TRUE) {
      i = QUEUESIZE;
    }
    i++;
  }
  */
  return FALSE;
}

/* TESTED */
int isValid(char userInput[]) {
  int length = strlen(userInput), i = 0, count[9] = {0}, val;
  char c;
  if (length != SIZE*SIZE) {
    return FALSE;
  }
  while (userInput[i]) {
    if (isdigit(userInput[i]) || userInput[i] == SPACE) {
      if (userInput[i] == '9' || userInput[i] == '0') {
        return FALSE;
      }
    else {
      i++;
    }
  }
    else {
      return FALSE;
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

/* TESTED: Loops through all previous boards in the queue to find duplicates */
int isDuplicate(Node currentNode) {
  int x, y, i = 0, count;
  Node *parent;
  while (parent != NULL) {
    parent = currentNode->parent;
    count = 0;
    for (y=0; y<SIZE; y++) {
      for (x=0; x<SIZE; x++) {
        if (currentNode->board[y][x] != prevNode->board[y][x]) {
          currentNode = &prevNode;
        }
        else if (currentNode->board[y][x] != prevNode->board[y][x]) {
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

/* TESTED: Populates initial board with user input */
void initialiseBoard(char board[SIZE][SIZE], char userInput[]) {
  int i = 0, x, y;
  while (userInput[i]) {
    for (y=0; y<SIZE; y++) {
      for (x=0; x<SIZE; x++) {
        board[y][x] = userInput[i];
        i++;
      }
    }
  }
}

/* TESTED: Adds current board to end of queue (position = index) */
Node* newNode(char board[SIZE][SIZE], int index, Node *parent, Node *prevNode) {
  int x, y;
  Node *current = (Node *)malloc(sizeof(Node));
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      current->board[y][x] = board[y][x];
    }
  }
  current->index = index;
  current->parent = parent;
  current->prevNode = prevNode;

  return current;
}

/* TESTED: Finds the co-ordinates of the space in the grid */
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

/* TESTED: Finds all unique permutations of 'currentBoard' and
// adds them to the end of the queue.
// Also checks if the board is complete and calls function to trace parents */
int permuteBoard(Node *current, int *currentIndex) {
  int spaceX = -1, spaceY = -1, i;
  char newBoard[SIZE][SIZE];

  if (isComplete(current->board)) {
    /*traceParents(Queue, Solutions, currentBoard.index);*/
    return TRUE;
  }

  findSpace(current->board, &spaceX, &spaceY);

  for (i = -1; i <= 1; i = i+2) {
    copyBoard(newBoard, current->board);
    /*IF WE CAN SWAP TO THE LEFT OR RIGHT */
    if (access(spaceX + i, spaceY)) {
      swap(&newBoard[spaceY][spaceX+i], &newBoard[spaceY][spaceX]);
      if (!isDuplicate(current)) {
        (*currentIndex)++;
        newNode(newBoard, *currentIndex, current, current);
      }
    }

    copyBoard(newBoard, currentBoard.board);

    /*IF WE CAN SWAP ABOVE OR BELOW THE SPACE*/
    if (access(spaceX, spaceY + i)) {
      swap(&newBoard[spaceY+i][spaceX], &newBoard[spaceY][spaceX]);
      if (!isDuplicate(current)) {
        (*currentIndex)++;
        newNode(Queue, newBoard, *currentIndex, currentBoard.index);
      }
    }
  }

  return FALSE;

}

/* Once solution is found, follows parents back to original input to find all
// the moves leading up to the final solution and prints in order
void traceParents(Board Queue[QUEUESIZE], Board Solutions[MAXMOVES], int currentIndex) {
  int moves = 0, i;
  fprintf(stdout, "SOLUTION FOUND! \n\n");

  while (Queue[currentIndex].index != 0) {
    newNode(Solutions, Queue[currentIndex].board, moves, 0);
    currentIndex = Queue[currentIndex].parent;
    moves++;
  }

  newNode(Solutions, Queue[0].board, moves, 0);

  for (i = 0; i <= moves; i++) {
    if (i == 0) {
      fprintf(stdout, "USER INPUT: \n");
    }
    else {
      fprintf(stdout, "MOVE %d: \n", i);
    }
    printBoard(Solutions[moves - i].board);
    delay(DELAY);
  }
}

/ TESTED: Copies content from current board on to new board */
void copyBoard(char newBoard[SIZE][SIZE], char currentBoard[SIZE][SIZE]) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      newBoard[y][x] = currentBoard[y][x];
    }
  }
}

/* TESTED: If permuteBoard is trying to access area outside 3x3 array, return FAIL */
int access(int x, int y) {
  if (x<0 || x>=SIZE) {
    return FALSE;
  }
  if (y<0 || y>=SIZE) {
    return FALSE;
  }
  return TRUE;
}

/* TESTED (VISUALLY) */
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

/* TESTED */
void swap(char *a, char *b) {
  char temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

/* TESTED: Function to delay each array print - include <time.h> */
void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

void test(void) {
  char userInput[] = "4312 8657";
  int x = 0, y = 0;
  char board[SIZE][SIZE], newBoard[SIZE][SIZE];

  /* Test isValid */
  assert(isValid(userInput) == TRUE);
  assert(isValid("567123 48") == TRUE);
  assert(isValid("56712348 ") == TRUE);
  assert(isValid(" 56712348") == TRUE);
  assert(isValid(" 56711348") == FALSE);
  assert(isValid(" 5671234") == FALSE);
  assert(isValid(" 56712!4p") == FALSE);
  assert(isValid(" 56710042") == FALSE);
  assert(isValid("123 5 6 7") == FALSE);
  assert(isValid("1235678 \n") == FALSE);

  /* Test initialiseBoard */
  initialiseBoard(board, userInput);
  assert(board[0][1] == userInput[1]);
  assert(board[1][1] == userInput[4]);
  assert(board[2][2] == userInput[8]);
  initialiseBoard(board, "123456 78");
  assert(board[0][0] == '1');
  assert(board[1][2] == '6');
  assert(board[2][0] == ' ');

  /* Test isComplete */
  initialiseBoard(board, userInput);
  assert(isComplete(board) == FALSE);
  initialiseBoard(board, "12345678 ");
  assert(isComplete(board) == TRUE);
  initialiseBoard(board, " 23456781");
  /*assert(isComplete(Queue[0].board) == FALSE); */

  /* Test swap */
  initialiseBoard(board, "1234567 8");
  assert(board[2][1] == ' ');
  assert(board[2][2] == '8');
  swap(&board[2][1], &board[2][2]);
  assert(board[2][2] == ' ');
  assert(board[2][1] == '8');
  swap(&board[1][2], &board[2][2]);
  assert(board[1][2] == ' ');
  assert(board[2][2] == '6');

  /* Test newNode *
  initialiseBoard(board, "1234567 8");
  newNode(Queue, board, 1, 0);
  assert(Queue[0].board[0][0] == Queue[1].board[0][0]);
  assert(Queue[0].board[1][2] == Queue[1].board[1][2]);
  assert(Queue[0].board[2][0] == Queue[1].board[2][0]);
  assert(Queue[1].parent == Queue[0].index);
  assert(Queue[1].index == 1);
  swap(&board[2][2], &board[1][2]);
  newNode(Queue, board, 2, 0);
  assert(Queue[0].board[0][0] == Queue[2].board[0][0]);
  assert(Queue[0].board[1][2] == Queue[2].board[2][2]);
  assert(Queue[0].board[2][2] == Queue[2].board[1][2]);
  assert(Queue[1].parent == Queue[0].index);
  assert(Queue[2].index == 2);

   Test isDuplicate *
  initialiseBoard(board, "87654321 ");
  assert(isDuplicate(Queue, board) == TRUE);
  swap(&board[2][2], &board[1][2]);
  assert(isDuplicate(Queue, board) == FALSE);
  swap(&board[2][2], &board[1][2]);
  assert(isDuplicate(Queue, board) == TRUE);

  / Test findSpace */
  initialiseBoard(board, "4312 8657");
  findSpace(board, &x, &y);
  assert(x == 1 && y == 1);
  swap(&board[1][1], &board[1][2]);
  findSpace(board, &x, &y);
  assert(x == 2 && y == 1);
  initialiseBoard(board, " 12345678");
  findSpace(board, &x, &y);
  assert(x == 0 && y == 0);

  /*Test copyBoard */
  initialiseBoard(board, "12345678 ");
  initialiseBoard(newBoard, " 87654321");
  assert(board[0][1] != newBoard[0][1]);
  assert(board[2][2] != newBoard[2][2]);
  assert(board[1][2] != newBoard[1][2]);
  copyBoard(newBoard, board);
  assert(board[0][1] == newBoard[0][1]);
  assert(board[2][2] == newBoard[2][2]);
  assert(board[1][2] == newBoard[1][2]);

  /* Test access */
  assert(access(-1,0) == FALSE);
  assert(access(3,0) == FALSE);
  assert(access(0,10) == FALSE);
  assert(access(1,-6) == FALSE);
  assert(access(1,1) == TRUE);
  assert(access(1,2) == TRUE);
  assert(access(0,0) == TRUE);

  /* Test permuteBoard *
  memset(Queue, 0x00, 3*sizeof(Queue[0])); / Reset Queue *
  initialiseBoard(board, "1 2345678");
  assert(permuteBoard(Queue, Queue[0], &currentIndex, Solutions) == FALSE);
  assert(Queue[1].index == 1);
  assert(Queue[3].index == 3);
  /These will fail if order of permutations changes *
  assert(Queue[0].board[2][2] == Queue[1].board[2][2]);
  assert(Queue[2].board[2][2] == Queue[3].board[2][2]);
  assert(Queue[0].board[0][0] == Queue[1].board[0][1]);
  assert(Queue[1].board[0][1] == Queue[3].board[0][0]);
  assert(Queue[0].board[0][2] == Queue[2].board[0][1]);
  assert(Queue[1].board[0][2] == Queue[2].board[0][1]);
  assert(Queue[0].board[1][1] == Queue[3].board[0][1]);
  assert(Queue[2].board[1][1] == Queue[3].board[0][1]);
  assert(permuteBoard(Queue, Queue[1], &currentIndex, Solutions) == FALSE);
  assert(Queue[4].index == 4);

  initialiseBoard(board, "1 2345678");
  assert(Queue[4].parent == 1);
  currentIndex = 0;

  / Test traceParents /
  / ALL OF THE BELOW WAS COMMENTED OUT AFTER TESTING TO STOP PRINTING *
  / ******** TO TEST TRACEPARENTS, UNCOMMENT BELOW CODE ***********
  initialiseBoard(board, "12345678 ", Queue);
  assert(permuteBoard(Queue, Queue[0], &currentIndex, Solutions) == TRUE);


  initialiseBoard(board, "1 2345678", Queue);
  traceParents(Queue, Solutions, 4);
  assert(Solutions[2].board[0][0] == Queue[0].board[0][0]);
  assert(Solutions[1].board[0][0] == Queue[1].board[0][0]);
  assert(Solutions[0].board[0][0] == Queue[4].board[0][0]);
  assert(Solutions[0].index == Solutions[1].parent);
  assert(Solutions[1].index == Solutions[2].parent);
  */

}
