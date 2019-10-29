#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define HEIGHT 20
#define WIDTH 20
enum fileInputs {WALL = '#', SPACE = ' ', PATH = '.'};
enum bool {FAIL, SUCCESS};

void test(void);
void readFile(char maze[HEIGHT][WIDTH], FILE *fp);
int findStart(char maze[HEIGHT][WIDTH]);
int explore(int x, int y, char maze[HEIGHT][WIDTH]);
int isExit(int x, int y, char maze[HEIGHT][WIDTH]);
void printMaze(char maze[HEIGHT][WIDTH]);

int main(int argc, char **argv) {
  FILE* fp;
  char maze[HEIGHT][WIDTH];
  test();

  if(argc!=2) {
    argv[1] = "testmaze.txt";
    fprintf(stderr,"ERROR: Incorrect usage, defaulted to testmaze.txt\n");
  }

  fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "ERROR - Unable to open file %s\n", argv[1]);
    exit(1);
  }

  readFile(maze, fp);
  printMaze(maze);
  explore(0,1,maze);
  printMaze(maze);

  return 0;
}

void test(void) {
  char testInput[HEIGHT][WIDTH] = {{'#',' ','#','#'},
                                   {'#',' ',' ','#'},
                                   {'#','#',' ','#'}};

  assert(findStart(testInput) == 501);

}

void readFile(char maze[HEIGHT][WIDTH], FILE *fp) {
  int i, j=0;
  char str[WIDTH];

  while (fgets(str, WIDTH+1, fp) != NULL) {
    for (i=0;i<WIDTH;i++) {
      maze[i][j] = str[i];
    }
    j++;
  }
}

/* READ FILE SHOULD REMOVE ALL SPACE FROM LEFT SIDE TO START OF MAZE */

int findStart(char maze[HEIGHT][WIDTH]) {
  int i;
  for (i = 0; i < HEIGHT; i++) {
    if (maze[i][0] == SPACE) {
      return i;
    }
  }
  for (i = 0; i < WIDTH; i++) {
    if (maze[0][i] == SPACE) {
      return i + 500;
    }
  }
  return FAIL;
}

int explore(int x, int y, char maze[HEIGHT][WIDTH]) {
  if (x == WIDTH - 1 || y == HEIGHT - 1) {
    return SUCCESS;
    printMaze(maze);
  }
  if (maze[x][y] == SPACE) {
    maze[x][y] = PATH;
  }
  else if (maze[x][y] == WALL) {
    return FAIL;
  }
  /* IF WE CAN GO RIGHT */
  if (explore(x+1, y, maze)) {
    return SUCCESS;
  }
  /* IF WE CAN GO UP */
  if (explore(x, y+1, maze)) {
    return SUCCESS;
  }
  /* IF WE CAN GO LEFT */
  if (explore(x-1, y, maze)) {
    return SUCCESS;
  }
  /* IF WE CAN GO DOWN */
  if (explore(x, y-1, maze)) {
    return SUCCESS;
  }
  return FAIL;
}

void printMaze(char maze[HEIGHT][WIDTH]) {
  int x, y;
  for (x=0;x<WIDTH;x++) {
    for (y=0;y<HEIGHT;y++) {
      fprintf(stdout, "%c", maze[y][x]);
    }
  }
}
