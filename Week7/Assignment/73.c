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
  struct node *parent;
  struct node *nextNode;
} Node;

void test(void);
int checkInput(int argc, char **argv);
int isValid(char userInput[]);
int isSolvable(char userInput[]);
int isComplete(char board[SIZE][SIZE]);
int isDuplicate(Node *endNode, char board[SIZE][SIZE]);
void initialiseBoard(char board[SIZE][SIZE], char userInput[]);
Node* newNode(char board[SIZE][SIZE], Node *parent, Node *nextNode);
void findSpace(char board[SIZE][SIZE], int *x, int *y);
int permuteBoard(Node *parentNode, Node *endNode);
void traceParents(Node *currentNode, int* moves);
int access(int x, int y);
void copyBoard(char newBoard[SIZE][SIZE], char currentBoard[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE]);
void swap(char *a, char *b);
void delay(int seconds);
void freeNodes(Node* endNode);

int main(int argc, char **argv) {
  char board[SIZE][SIZE], userInput[10];
  Node *startNode;

  test();

  if (checkInput(argc, argv)) {
    strcpy(userInput, argv[1]);
  }
  else {
    exit(1);
  }

  initialiseBoard(board, userInput);
  startNode = newNode(board, NULL, NULL);
  permuteBoard(startNode, startNode);

  freeNodes(startNode);
  return FALSE;
}

/* Difficult to test due to **argv - isValid and isSolvable thoroughly tested */
int checkInput(int argc, char **argv) {
  char userInput[10];
  if (argc == 2) {
    strcpy(userInput, argv[1]);
    if (isValid(userInput)) {
        if (isSolvable(userInput)) {
          return TRUE;
        }
        else {
          fprintf(stderr, "'%s' IS UNSOLVABLE! PLEASE TRY AGAIN \n", userInput);
          return FALSE;
        }
    }
    else if (!isValid(userInput)) {
      fprintf(stderr, "INVALID INPUT - SHOULD CONTAIN NUMBERS 0 - 8 "
      "AND A SPACE ONLY: E.G. '1234 5678'\n");
      return FALSE;
    }
  }
  else if (argc != 2) {
    fprintf(stderr,"ERROR: Incorrect usage: e.g ./8tile '12345678 '\n");
    return FALSE;
  }
  return FALSE;
}

/* TESTED */
int isValid(char userInput[]) {
  int length = strlen(userInput), i = 0, count[10] = {0}, val;
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
  for (i=0; i<length-1; i++) {
    if (userInput[i] == SPACE) {
      count[0]++;
    }
    else {
      c = userInput[i];
      val = c - '0';
      count[val]++;
    }
  }
  for (i=0; i<length; i++) {
    if (count[i] > 1) {
      return FALSE;
    }
  }
  return TRUE;
}

/* TESTED: Finds number of 'inversions' - if that number is odd, puzzle
// is impossible, if even, puzzle is solvable
// https://www.geeksforgeeks.org/check-instance-8-puzzle-solvable */
int isSolvable(char userInput[]) {
  int count = 0, i, j;
  for (i = 0; i < SIZE*SIZE - 1; i++) {
    for (j = i+1; j < SIZE*SIZE; j++) {
      if ((userInput[i] != SPACE) && (userInput[j] != SPACE)) {
        if (userInput[i] > userInput[j]) {
          count++;
        }
      }
    }
  }

  if (count % 2 == 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
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

/* TESTED: Loops through all previous boards to find duplicates */
int isDuplicate(Node *startNode, char board[SIZE][SIZE]) {
  int x, y, count;
  Node* tempNode;
  do {
    tempNode = startNode;
    count = 0;
    for (y=0; y<SIZE; y++) {
      for (x=0; x<SIZE; x++) {
        if (startNode->board[y][x] == board[y][x]) {
          count++;
        }
        else {
          if (startNode->nextNode != NULL) {
            startNode = startNode->nextNode;
          }
        }
      }
    }
    if (count == SIZE*SIZE) {
      return TRUE;
    }
  } while (tempNode->nextNode != NULL);

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

/* TESTED: Returns pointer to new node */
Node* newNode(char board[SIZE][SIZE], Node *parent, Node *nextNode) {
  int x, y;
  Node *current = (Node *)malloc(sizeof(Node));
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      current->board[y][x] = board[y][x];
    }
  }
  current->parent = parent;
  current->nextNode = nextNode;

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
int permuteBoard(Node *parentNode, Node *startNode) {
  int spaceX = -1, spaceY = -1, i;
  char newBoard[SIZE][SIZE];
  Node *currentNode = parentNode;
  while (!isComplete(currentNode->board)) {
    findSpace(parentNode->board, &spaceX, &spaceY);

    for (i = -1; i <= 1; i = i+2) {
      copyBoard(newBoard, parentNode->board);
      /*IF WE CAN SWAP TO THE LEFT OR RIGHT */
      if (access(spaceX + i, spaceY)) {
        swap(&newBoard[spaceY][spaceX+i], &newBoard[spaceY][spaceX]);
        if (!isDuplicate(startNode, newBoard)) {
          currentNode->nextNode = newNode(newBoard, parentNode, NULL);
          currentNode = currentNode->nextNode;
        }
      }

      copyBoard(newBoard, parentNode->board);

      /*IF WE CAN SWAP ABOVE OR BELOW THE SPACE*/
      if (access(spaceX, spaceY + i)) {
        swap(&newBoard[spaceY+i][spaceX], &newBoard[spaceY][spaceX]);
        if (!isDuplicate(startNode, newBoard)) {
          currentNode->nextNode = newNode(newBoard, parentNode, NULL);
          currentNode = currentNode->nextNode;
        }
      }
    }
    /* Move to next node in chain */
    parentNode = parentNode->nextNode;
  }

  if (isComplete(currentNode->board)) {
    fprintf(stdout, "SOLUTION FOUND! \n\n");
    i = 0;
    traceParents(currentNode, &i);
    fprintf(stdout, "MOVE %d: \n", i);
    printBoard(currentNode->board);
    delay(DELAY);
    return TRUE;
  }

  return FALSE;
}

/* Once solution is found, follows parents back to original input to find all
// the moves leading up to the final solution and prints in order */
void traceParents(Node *currentNode, int* moves) {
  if (currentNode->parent != NULL) {
    currentNode = currentNode->parent;
    traceParents(currentNode, moves);
    if (*moves == 0) {
      fprintf(stdout, "USER INPUT: \n");
    }
    else {
      fprintf(stdout, "MOVE %d: \n", *moves);
    }
    printBoard(currentNode->board);
    (*moves)++;
    delay(DELAY);
  }
}

/* TESTED: Copies content from current board on to new board */
void copyBoard(char newBoard[SIZE][SIZE], char currentBoard[SIZE][SIZE]) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      newBoard[y][x] = currentBoard[y][x];
    }
  }
}

/* TESTED: If permuteBoard is trying to access area outside
// 3x3 array, return FALSE */
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

/* TESTED (Ran valgrind/-fsanitize & tried to print boards after freeing) */
void freeNodes(Node* startNode) {
  Node* temp;

  while (startNode->nextNode != NULL) {
    temp = startNode;
    startNode = startNode->nextNode;
    free(temp);
  }
  free(startNode);
}

void test(void) {
  char userInput[] = "4312 8657";
  int x = 0, y = 0;
  char board[SIZE][SIZE], newBoard[SIZE][SIZE];
  Node *testNode, *testNode2;
  /* UNCOMMENT TO TEST PERMUTEBOARD & TRACEPARENTS */
  /*
  Node *testNode3, *testNode4;
  int moves;
  */

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
  assert(isComplete(board) == FALSE);
  initialiseBoard(board, "1234567 8");
  assert(isComplete(board) == FALSE);

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

  /* Test newNode */
  initialiseBoard(board, "1234567 8");
  testNode = newNode(board, NULL, NULL);
  initialiseBoard(board, "12345678 ");
  testNode2 = newNode(board, testNode, NULL);
  assert(testNode->board[0][0] == '1');
  assert(testNode->board[0][1] == '2');
  assert(testNode->board[2][1] == SPACE);
  assert(testNode2->board[0][0] == '1');
  assert(testNode2->board[2][1] == '8');
  assert(testNode2->parent == testNode);
  assert(testNode->parent == NULL);
  testNode->nextNode = testNode2;
  assert(testNode->nextNode == testNode2);
  freeNodes(testNode);

  /* Test isDuplicate */
  initialiseBoard(board, "12345678 ");
  testNode = newNode(board, NULL, NULL);
  initialiseBoard(board, "12345678 ");
  assert(isDuplicate(testNode, board) == TRUE);
  initialiseBoard(board, "1234567 8");
  assert(isDuplicate(testNode, board) == FALSE);
  testNode2 = newNode(board, testNode, NULL);
  testNode->nextNode = testNode2;
  assert(isDuplicate(testNode, board) == TRUE);
  initialiseBoard(board, "123456 78");
  assert(isDuplicate(testNode, board) == FALSE);
  freeNodes(testNode);

  /* Test findSpace */
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

  /* Test permuteBoard */
  /* ALL OF THE BELOW WAS COMMENTED OUT AFTER TESTING TO STOP PRINTING
  / *** TO TEST PERMUTEBOARD AND TRACEPARENTS, UNCOMMENT BELOW CODE ***/
  /* ALSO UNCOMMENT TESTNODE3, TESTNODE4 & MOVES */
  /*
  initialiseBoard(board, "1 2345678");
  testNode = newNode(board, NULL, NULL);
  assert(permuteBoard(testNode, testNode) == TRUE);
  testNode2 = testNode->nextNode;
  testNode3 = testNode2->nextNode;
  testNode4 = testNode3->nextNode;
  */
  /*These will fail if order of permutations changes */
  /*
  assert(testNode->board[2][2] == testNode2->board[2][2]);
  assert(testNode3->board[2][2] == testNode4->board[2][2]);
  assert(testNode->board[0][0] == testNode2->board[0][1]);
  assert(testNode2->board[0][1] == testNode4->board[0][0]);
  assert(testNode->board[0][2] == testNode3->board[0][1]);
  assert(testNode2->board[0][2] == testNode3->board[0][1]);
  assert(testNode->board[1][1] == testNode4->board[0][1]);
  assert(testNode3->board[1][1] == testNode4->board[0][1]);
  */
  /* This part also tests that freeNodes works:
  // RUN WITH valgrind ./73 TO CHECK FOR LEAKS */
  /*freeNodes(testNode);*/


  /* Test traceParents - mainly tested visually */
  /*
  initialiseBoard(board, "1234567 8");
  testNode = newNode(board, NULL, NULL);
  swap(&board[2][1], &board[2][2]);
  assert(isComplete(board) == TRUE);
  testNode2 = newNode(board, testNode, NULL);
  testNode->nextNode = testNode2;
  moves = 0;
  traceParents(testNode2, &moves);
  assert(moves == 1);

  freeNodes(testNode);
  */
}
