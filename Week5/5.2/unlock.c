#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define GRIDSIZE 3
#define PATHLENGTH 9

enum bool {FAIL, SUCCESS};

void test(void);
int explore(int x, int y, int grid[GRIDSIZE][GRIDSIZE], int num);
void printArray(int grid[GRIDSIZE][GRIDSIZE]);


int main(void) {
  int startx = 0, starty = 0;
  int grid[GRIDSIZE][GRIDSIZE] = {{0}};

  if (explore(startx, starty, grid, 1)) {
    printArray(grid);
  }
  else {
    printf("Not a valid route\n");
  }


  return 0;
}

void test(void) {

}


int explore(int x, int y, int grid[GRIDSIZE][GRIDSIZE], int num) {

  if (x >= GRIDSIZE || y >= GRIDSIZE || x < 0 || y < 0) {
    return FAIL;
  }

  if (grid[y][x] == 0) {
    grid[y][x] = num;
  } else {
    return FAIL;
  }

  /* IF WE CAN GO RIGHT */
  if (explore(x+1, y, grid, num+1)) {
    return SUCCESS;
  }
  /* IF WE CAN GO UP */
  if (explore(x, y+1, grid, num+1)) {
    return SUCCESS;
  }
  /* IF WE CAN GO LEFT */
  if (explore(x-1, y, grid, num+1)) {
    return SUCCESS;
  }
  /* IF WE CAN GO DOWN */
  if (explore(x, y-1, grid, num+1)) {
    return SUCCESS;
  }
  if (num == PATHLENGTH) {
    return SUCCESS;
  } else {
    return FAIL;
  }
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
