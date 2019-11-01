#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#define N 100
enum bool {FAIL, SUCCESS};

void test(void);
void printBoard(int board[N][N]);
void initialiseBoard(int board[N][N]);
void mutateBoard(int board[N][N]);
int access(int x, int y);
int isCloser(int x1, int y1, int x2, int y2);
void swap(int *a, int *b);


int main(void) {
  int board[N][N], i;
  srand(time(NULL));


  initialiseBoard(board);
  printBoard(board);

  for (i = 0; i < N*N*N*N; i++) {
    mutateBoard(board);
  }

  printBoard(board);

  return 0;
}

void mutateBoard(int board[N][N]) {
  int x1, y1, x2, y2;
  x1 = rand() % N;
  y1 = rand() % N;
  x2 = rand() % N;
  y2 = rand() % N;

  if (board[y1][x1] < board[y2][x2]) {
    if (isCloser(x1, y1, x2, y2)) {
      swap(&board[y1][x1], &board[y2][x2]);
    }
  }
}

/* If first co-ordinates are closer than second co-ordinates, returns 1 */
int isCloser(int x1, int y1, int x2, int y2) {
  int centre = N/2, sqdist1, sqdist2;
  sqdist1 = ((x1-centre)*(x1-centre)) + ((y1-centre)*(y1-centre));
  sqdist2 = ((x2-centre)*(x2-centre)) + ((y2-centre)*(y2-centre));

  if (sqdist1 >= sqdist2) {
    return 1;
  }
  return 0;
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int access(int x, int y) {
  if (x<0 || x>=N) {
    return FAIL;
  }
  if (y<0 || y>=N) {
    return FAIL;
  }
  return SUCCESS;
}

void printBoard(int board[N][N]) {
  int x, y;
  for (x=0; x<N; x++) {
    for (y=0; y<N ;y++) {
      fprintf(stdout, "%d", board[y][x]);
    }
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"\n");
}

void initialiseBoard(int board[N][N]) {
  int x, y;
  for (x=0; x<N; x++) {
    for (y=0; y<N ;y++) {
      board[y][x] = (rand() % 10);
    }
  }
}
