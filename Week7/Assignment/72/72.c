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
#include "neillsdl2.h"

#define SIZE 3
#define SPACE ' '
#define QUEUESIZE 500000
#define MAXMOVES 150
#define TESTSIZE 20
#define RECTSIZE 200
#define MILLISECONDDELAY 300

enum bool {FALSE, TRUE};

typedef struct board {
  char board[SIZE][SIZE];
  int globalIndex;
  int parent;
} Board;

void test(void);
int checkInput(int argc, char **argv);
int isValid(char userInput[]);
int isSolvable(char userInput[]);
int isComplete(char board[SIZE][SIZE]);
int isDuplicate(Board Queue[QUEUESIZE], char currentBoard[SIZE][SIZE],
                int currentIndex);
void initialiseBoard(char board[SIZE][SIZE], char userInput[], Board Queue[QUEUESIZE]);
void addToQueue(Board Queue[QUEUESIZE], char board[SIZE][SIZE], int index, int parent);
void findSpace(char board[SIZE][SIZE], int *x, int *y);
int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard,
                 int *currentIndex, Board Solutions[MAXMOVES]);
void traceParents(Board Queue[QUEUESIZE], Board Solutions[MAXMOVES], int currentIndex);
void runSDL(Board* Solutions, int moves);
int access(int x, int y);
void copyBoard(char newBoard[SIZE][SIZE], char currentBoard[SIZE][SIZE]);
void swap(char *a, char *b);

int main(int argc, char **argv) {
  int currentIndex = 0, i = 0;
  char board[SIZE][SIZE], userInput[10];
  static Board Queue[QUEUESIZE];
  static Board Solutions[MAXMOVES];

  test();

  if (checkInput(argc, argv)) {
    strcpy(userInput, argv[1]);
  }
  else {
    exit(1);
  }

  /* i = Queue position of board which is being permuted
  // currentIndex = Current number of boards in the queue (i.e. the current
  // length of the queue) */
  initialiseBoard(board, userInput, Queue);
  while (i < QUEUESIZE/3) {
    if (permuteBoard(Queue, Queue[i], &currentIndex, Solutions) == TRUE) {
      i = QUEUESIZE;
    }
    i++;
  }

  return FALSE;
}

/* Initialise SDL window and outputs boards from solutions array */
void runSDL(Board* Solutions, int moves) {
  int x, y, boardNumber = moves;
  SDL_Simplewin sw;
  SDL_Rect rectangle;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  char currentChar;
  char movesString[15];

  Neill_SDL_ReadFont(fontdata, "mode7.fnt");

  rectangle.w = RECTSIZE;
  rectangle.h = RECTSIZE;

  Neill_SDL_Init(&sw);

  do {

    /* Reset grid to black */
    Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    for (y = 0; y < SIZE; y++) {
      for (x = 0; x < SIZE; x++) {
        rectangle.x = RECTSIZE * x;
        rectangle.y = RECTSIZE * y;
        SDL_RenderFillRect(sw.renderer, &rectangle);
      }
    }

    if (boardNumber > 0) {
     Neill_SDL_SetDrawColour(&sw, 192,192,192);
    }
    else {
     Neill_SDL_SetDrawColour(&sw, 50, 205, 50);
    }

    /* Draw white grid */
    for (y = 0; y < SIZE; y++) {
      for (x = 0; x < SIZE; x++) {
        if (Solutions[boardNumber].board[y][x] != SPACE) {
          rectangle.x = RECTSIZE * x;
          rectangle.y = RECTSIZE * y;
          SDL_RenderDrawRect(sw.renderer, &rectangle);
        }
      }
    }

    /* Use 2 seperate loops otherwise window bugs out */
    /* Draw board characters */
    for (y = 0; y < SIZE; y++) {
      for (x = 0; x < SIZE; x++) {
        currentChar = Solutions[boardNumber].board[y][x];
        Neill_SDL_DrawChar(&sw, fontdata, currentChar,
          RECTSIZE/2 + RECTSIZE * x, RECTSIZE/2 + RECTSIZE * y);
      }
    }

    Neill_SDL_DrawString(&sw, fontdata, "MOVES:",
    (RECTSIZE * SIZE) + RECTSIZE/5, RECTSIZE/5);
    sprintf(movesString, "%d", moves-boardNumber);
    Neill_SDL_DrawString(&sw, fontdata, movesString,
      (RECTSIZE * SIZE) + RECTSIZE/5, 2*RECTSIZE/5);

    if (boardNumber > 0) {
      boardNumber--;
    }
    else {
      Neill_SDL_DrawString(&sw, fontdata, "SOLUTION",
      (RECTSIZE * SIZE) + RECTSIZE/10, 2*RECTSIZE);
      Neill_SDL_DrawString(&sw, fontdata, "FOUND!",
      (RECTSIZE * SIZE) + RECTSIZE/5, 2*RECTSIZE + FNTHEIGHT + 10);
    }

    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_UpdateScreen(&sw);
    /* Check for ESC event - mouse click etc. */
    Neill_SDL_Events(&sw);

  }while(!sw.finished);

  SDL_Quit();
  atexit(SDL_Quit);
}

/* Difficult to test - isValid and isSolvable thoroughly tested */
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
      val = c - '0';
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

/* TESTED: Finds number of 'inversions' - if that number is odd, puzzle
// is impossible, if even, puzzle is solvable */
/* https://www.geeksforgeeks.org/check-instance-8-puzzle-solvable */
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

/* TESTED: Loops through all previous boards in the queue to find duplicates */
int isDuplicate(Board Queue[QUEUESIZE], char currentBoard[SIZE][SIZE],
                int currentIndex) {
  int x, y, i = 0, count;
  while (i < currentIndex) {
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

/* TESTED: Populates initial board with user input and adds to first queue position */
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
  addToQueue(Queue, board, 0, 0);
}

/* TESTED: Adds current board to end of queue (position = index) */
void addToQueue(Board Queue[QUEUESIZE], char board[SIZE][SIZE], int index, int parent) {
  int x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      Queue[index].board[y][x] = board[y][x];
    }
  }
  Queue[index].globalIndex = index;
  Queue[index].parent = parent;
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
int permuteBoard(Board Queue[QUEUESIZE], Board currentBoard,
                 int *currentIndex, Board Solutions[MAXMOVES]) {
  int spaceX = -1, spaceY = -1, i;
  char newBoard[SIZE][SIZE];

  if (isComplete(currentBoard.board)) {
    traceParents(Queue, Solutions, currentBoard.globalIndex);
    return TRUE;
  }

  findSpace(currentBoard.board, &spaceX, &spaceY);

  for (i = -1; i <= 1; i = i+2) {
    copyBoard(newBoard, currentBoard.board);
    /*IF WE CAN SWAP TO THE LEFT OR RIGHT*/
    if (access(spaceX + i, spaceY)) {
      swap(&newBoard[spaceY][spaceX+i], &newBoard[spaceY][spaceX]);
      if (!isDuplicate(Queue, newBoard, *currentIndex)) {
        (*currentIndex)++;
        addToQueue(Queue, newBoard, *currentIndex, currentBoard.globalIndex);
      }
    }

    copyBoard(newBoard, currentBoard.board);

    /*IF WE CAN SWAP ABOVE OR BELOW THE SPACE*/
    if (access(spaceX, spaceY + i)) {
      swap(&newBoard[spaceY+i][spaceX], &newBoard[spaceY][spaceX]);
      if (!isDuplicate(Queue, newBoard, *currentIndex)) {
        (*currentIndex)++;
        addToQueue(Queue, newBoard, *currentIndex, currentBoard.globalIndex);
      }
    }
  }

  return FALSE;

}

/* Once solution is found, follows parents back to original input to find all
// the moves leading up to the final solution and prints in order */
void traceParents(Board Queue[QUEUESIZE], Board Solutions[MAXMOVES], int currentIndex) {
  int moves = 0;
  fprintf(stdout, "SOLUTION FOUND! \n\n");

  while (Queue[currentIndex].globalIndex != 0) {
    addToQueue(Solutions, Queue[currentIndex].board, moves, 0);
    currentIndex = Queue[currentIndex].parent;
    moves++;
  }

  addToQueue(Solutions, Queue[0].board, moves, 0);

  /* Print out solutions to SDL window */
  runSDL(Solutions, moves);
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

/* TESTED */
void swap(char *a, char *b) {
  char temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void test(void) {
  char userInput[] = "4312 8567";
  int x = 0, y = 0, currentIndex = 0;
  char board[SIZE][SIZE], newBoard[SIZE][SIZE];
  static Board Queue[TESTSIZE];
  static Board Solutions[TESTSIZE];

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

  /* Test isSolvable */
  assert(isSolvable("12345678 ") == TRUE);
  assert(isSolvable(userInput) == TRUE);
  assert(isSolvable("12345 678") == TRUE);
  assert(isSolvable("182 43765") == TRUE);
  assert(isSolvable("528417 36") == TRUE);
  assert(isSolvable("812 43765") == FALSE);
  assert(isSolvable("7125 4836") == FALSE);
  assert(isSolvable("7125348 6") == FALSE);

  /* Test initialiseBoard */
  initialiseBoard(board, userInput, Queue);
  assert(board[0][1] == userInput[1]);
  assert(board[1][1] == userInput[4]);
  assert(board[2][2] == userInput[8]);
  initialiseBoard(board, "123456 78", Queue);
  assert(board[0][0] == '1');
  assert(board[1][2] == '6');
  assert(board[2][0] == ' ');

  /* Test isComplete */
  initialiseBoard(board, userInput, Queue);
  assert(isComplete(board) == FALSE);
  initialiseBoard(board, "12345678 ", Queue);
  assert(isComplete(board) == TRUE);
  initialiseBoard(board, " 23456781", Queue);
  assert(isComplete(Queue[0].board) == FALSE);

  /* Test swap */
  initialiseBoard(board, "1234567 8", Queue);
  assert(board[2][1] == ' ');
  assert(board[2][2] == '8');
  swap(&board[2][1], &board[2][2]);
  assert(board[2][2] == ' ');
  assert(board[2][1] == '8');
  swap(&board[1][2], &board[2][2]);
  assert(board[1][2] == ' ');
  assert(board[2][2] == '6');

  /* Test addToQueue */
  initialiseBoard(board, "1234567 8", Queue);
  addToQueue(Queue, board, 1, 0);
  assert(Queue[0].board[0][0] == Queue[1].board[0][0]);
  assert(Queue[0].board[1][2] == Queue[1].board[1][2]);
  assert(Queue[0].board[2][0] == Queue[1].board[2][0]);
  assert(Queue[1].parent == Queue[0].globalIndex);
  assert(Queue[1].globalIndex == 1);
  swap(&board[2][2], &board[1][2]);
  addToQueue(Queue, board, 2, 0);
  assert(Queue[0].board[0][0] == Queue[2].board[0][0]);
  assert(Queue[0].board[1][2] == Queue[2].board[2][2]);
  assert(Queue[0].board[2][2] == Queue[2].board[1][2]);
  assert(Queue[1].parent == Queue[0].globalIndex);
  assert(Queue[2].globalIndex == 2);

  /* Test isDuplicate */
  initialiseBoard(board, "87654321 ", Queue);
  currentIndex++;
  assert(isDuplicate(Queue, board, currentIndex) == TRUE);
  swap(&board[2][2], &board[1][2]);
  assert(isDuplicate(Queue, board, currentIndex) == FALSE);
  swap(&board[2][2], &board[1][2]);
  assert(isDuplicate(Queue, board, currentIndex) == TRUE);
  currentIndex=0;

  /* Test findSpace */
  initialiseBoard(board, "4312 8657", Queue);
  findSpace(board, &x, &y);
  assert(x == 1 && y == 1);
  swap(&board[1][1], &board[1][2]);
  findSpace(board, &x, &y);
  assert(x == 2 && y == 1);
  initialiseBoard(board, " 12345678", Queue);
  findSpace(board, &x, &y);
  assert(x == 0 && y == 0);

  /*Test copyBoard */
  initialiseBoard(board, "12345678 ", Queue);
  initialiseBoard(newBoard, " 87654321", Queue);
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
  memset(Queue, 0x00, 3*sizeof(Queue[0])); /* Reset Queue */
  initialiseBoard(board, "1 2345678", Queue);
  assert(permuteBoard(Queue, Queue[0], &currentIndex, Solutions) == FALSE);
  assert(Queue[1].globalIndex == 1);
  assert(Queue[3].globalIndex == 3);
  /*These will fail if order of permutations changes */
  assert(Queue[0].board[2][2] == Queue[1].board[2][2]);
  assert(Queue[2].board[2][2] == Queue[3].board[2][2]);
  assert(Queue[0].board[0][0] == Queue[1].board[0][1]);
  assert(Queue[1].board[0][1] == Queue[3].board[0][0]);
  assert(Queue[0].board[0][2] == Queue[2].board[0][1]);
  assert(Queue[1].board[0][2] == Queue[2].board[0][1]);
  assert(Queue[0].board[1][1] == Queue[3].board[0][1]);
  assert(Queue[2].board[1][1] == Queue[3].board[0][1]);
  assert(permuteBoard(Queue, Queue[1], &currentIndex, Solutions) == FALSE);
  assert(Queue[4].globalIndex == 4);initialiseBoard(board, "1 2345678", Queue);
  assert(Queue[4].parent == 1);
  currentIndex = 0;

  /* Test traceParents */
  /* ALL OF THE BELOW WAS COMMENTED OUT AFTER TESTING TO STOP PRINTING */
  /*
  initialiseBoard(board, "12345678 ", Queue);
  assert(permuteBoard(Queue, Queue[0], &currentIndex, Solutions) == TRUE);
  */
  /* ******** TO TEST TRACEPARENTS, UNCOMMENT BELOW CODE ************/
  /*
  initialiseBoard(board, "1 2345678", Queue);
  traceParents(Queue, Solutions, 4);
  assert(Solutions[2].board[0][0] == Queue[0].board[0][0]);
  assert(Solutions[1].board[0][0] == Queue[1].board[0][0]);
  assert(Solutions[0].board[0][0] == Queue[4].board[0][0]);
  assert(Solutions[0].globalIndex == Solutions[1].parent);
  assert(Solutions[1].globalIndex == Solutions[2].parent);
  */
}
