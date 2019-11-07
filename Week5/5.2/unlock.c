#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
/* PROGRAM PARAMETERS */
#define GRIDSIZE 3
#define PATHLENGTH 9
#define STARTX 0
#define STARTY 0

enum bool {FAIL, SUCCESS};
/* Moves named as 1, 2, 3, 4 */
enum moves {START, UP, RIGHT, DOWN, LEFT};

typedef struct validPaths {
  int path[PATHLENGTH];
  int pathNumber;
  struct validPaths *next;
} validPaths;

void test(void);
int explore(int x, int y, int grid[GRIDSIZE][GRIDSIZE], int num, int direction, validPaths *sp);
void printArray(int grid[GRIDSIZE][GRIDSIZE]);


int main(void) {
  validPaths head, *sp = &head;
  int i, startx = STARTX, starty = STARTY, complete = FAIL;
  int grid[GRIDSIZE][GRIDSIZE] = {{0}};
  head.next = NULL;

  while (complete = FAIL) {
    if (explore(startx, starty, grid, 0, START, sp)) {
      printArray(grid);
    }


  for (i=0; i < PATHLENGTH; i++) {
    printf("%d ", sp->path[i]);
  }

  return 0;
}

void test(void) {

}


int explore(int x, int y, int grid[GRIDSIZE][GRIDSIZE], int num, int direction, validPaths *sp) {
  if (x >= GRIDSIZE || y >= GRIDSIZE || x < 0 || y < 0) {
    return FAIL;
  }

  printf("%d \n", sp->path[num-1]);
  printf("_____________________\n");

  if (grid[y][x] == 0) {
    sp->path[num] = direction;
    num += 1;
    grid[y][x] = num;
  } else {
    return FAIL;
  }

  /* IF WE CAN GO RIGHT */
  if (explore(x+1, y, grid, num, RIGHT, sp)) {
    return SUCCESS;
  }
  /* IF WE CAN GO UP */
  if (explore(x, y-1, grid, num, UP, sp)) {
    return SUCCESS;
  }
  /* IF WE CAN GO LEFT */
  if (explore(x-1, y, grid, num, LEFT, sp)) {
    return SUCCESS;
  }
  /* IF WE CAN GO DOWN */
  if (explore(x, y+1, grid, num, DOWN, sp)) {
    return SUCCESS;
  }
  if (num == PATHLENGTH) {
    return SUCCESS;
  }

  return FAIL;
}

void printArray(int grid[GRIDSIZE][GRIDSIZE]) {
  int x, y;
  for (y=0;y<GRIDSIZE;y++) {
    for (x=0;x<GRIDSIZE;x++) {
      fprintf(stdout, "%d ", grid[y][x]);
    }
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
}
