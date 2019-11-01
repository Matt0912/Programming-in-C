#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#define N 20
enum bool {FAIL, SUCCESS};

void test(void);
void printBoard(int board[N][N]);
void initialiseBoard(int board[N][N]);
void mutateBoard(int board[N][N]);
int access(int x, int y);
void swap(int *a, int *b);


int main(void) {
  int board[N][N], i;
  srand(time(NULL));


  initialiseBoard(board);
  printBoard(board);

  for (i = 0; i < N*N*N; i++) {
    mutateBoard(board);
  }

  printBoard(board);

  return 0;
}

void mutateBoard(int board[N][N]) {
  int x, y;
  x = rand() % N;
  y = rand() % N;

  if (access(x-1, y)) {
    if (board[y][x-1] > board[y][x]) {
      swap(&board[y][x-1], &board[y][x]);
    }
  }
  else if (access(x+1, y)) {
    if (board[y][x+1] < board[y][x]) {
      swap(&board[y][x+1], &board[y][x]);
    }
  }

  x = rand() % N;
  y = rand() % N;

  if (access(x, y-1)) {
    if (board[y-1][x] > board[y][x]) {
      swap(&board[y-1][x], &board[y][x]);
    }
  }
  else if (access(x, y+1)) {
    if (board[y+1][x] < board[y][x]) {
      swap(&board[y+1][x], &board[y][x]);
    }
  }



}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

/* If countHeads is trying to access area outside 40x40 array, return FAIL */
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
