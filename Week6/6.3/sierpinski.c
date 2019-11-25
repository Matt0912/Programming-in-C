#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define SIZE 27
#define DELAY 500

enum bool {FALSE, TRUE};

void test(void);
void initialiseBoard(char board[SIZE][SIZE]);
void iterateBoard(char board[SIZE][SIZE], int level);
void drawToBoard(char board[SIZE][SIZE], int boardsize, int i, int j);
void printBoard(char board[SIZE][SIZE]);
void delay(int seconds);

int main(void) {
  test();



  return FALSE;
}

void test(void) {
  char board[SIZE][SIZE];
  initialiseBoard(board);
  iterateBoard(board, 1);

}

void iterateBoard(char board[SIZE][SIZE], int level) {
  int boardsize = SIZE/pow(3, level), i, j;

  delay(DELAY);
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      drawToBoard(board, boardsize, i, j);
    }
  }
  printBoard(board);
  iterateBoard(board, level+1);
}

void drawToBoard(char board[SIZE][SIZE], int boardsize, int i, int j) {
  int startx = boardsize/3 + i*boardsize;
  int starty = boardsize/3 + j*boardsize;
  int endx = 2*boardsize/3 + i*boardsize;
  int endy = 2*boardsize/3 + j*boardsize;

  while(startx < endx) {
    starty = boardsize/3 + j*boardsize;
    while (starty < endy) {
      board[starty][startx] = '#';
      starty++;
    }
    startx++;
  }
}

void initialiseBoard(char board[SIZE][SIZE]) {
  int start = SIZE/3, end = 2*SIZE/3, x, y;
  for (y=0; y<SIZE; y++) {
    for (x=0; x<SIZE; x++) {
      board[y][x] = '.';
    }
  }
  for (y=start; y<end; y++) {
    for (x=start; x<end; x++) {
      board[y][x] = '#';
    }
  }
  printBoard(board);
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

void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
