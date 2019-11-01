#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#define MAXHEIGHT 20
#define MAXWIDTH 20
/* Delay between maze prints (ms) */
#define DELAY 50
enum fileInputs {WALL = '#', SPACE = ' ', PATH = '.'};
#define TEMPWALL '/'
enum bool {FAIL, SUCCESS};

void test(void);
void readFile(char maze[MAXHEIGHT][MAXWIDTH], FILE *fp, int *mazeWidth, int *mazeHeight);
int checkValid(char c);
int findStart(char maze[MAXHEIGHT][MAXWIDTH], int *startx, int *starty);
int explore(int x, int y, char maze[MAXHEIGHT][MAXWIDTH], int *wC, int *mazeWidth, int *mazeHeight);
void deadEnd(int x, int y, char maze[MAXHEIGHT][MAXWIDTH], int *wC);
int isExit(int x, int y, char maze[MAXHEIGHT][MAXWIDTH]);
void printMaze(char maze[MAXHEIGHT][MAXWIDTH], int *mazeWidth, int *mazeHeight);
void delay(int seconds);

int main(int argc, char **argv) {
  FILE* fp;
  char maze[MAXHEIGHT][MAXWIDTH];
  int wallCount = 0, startx = 0, starty = 0, mazeWidth = 0, mazeHeight = 0;
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

  readFile(maze, fp, &mazeWidth, &mazeHeight);
  if (findStart(maze, &startx, &starty)) {
    explore(startx, starty, maze, &wallCount, &mazeWidth, &mazeHeight);
  }


  return 0;
}

void test(void) {
  /*char testInput[MAXHEIGHT][MAXWIDTH] = {{'#',' ','#','#'},
                                   {'#',' ',' ','#'},
                                   {'#','#',' ','#'}};
*/

  assert(checkValid(' ') == SUCCESS);
  assert(checkValid('#') == SUCCESS);
  assert(checkValid('.') == SUCCESS);
  assert(checkValid('\n') == SUCCESS);
  assert(checkValid('4') == FAIL);
  assert(checkValid('P') == FAIL);

}

void readFile(char maze[MAXHEIGHT][MAXWIDTH], FILE *fp, int *mazeWidth, int *mazeHeight) {
  int i, j=0, count = 0;
  char str[MAXWIDTH];

  while (fgets(str, MAXWIDTH+1, fp) != NULL) {
    count = 0;
    for (i=0;i<MAXWIDTH+1;i++) {
      if (checkValid(str[i])) {
        maze[i][j] = str[i];
        count++;
      }
      else {
        if (count > *mazeWidth) {
          *mazeWidth = count;
        }
      }
    }
    j++;
  }
  *mazeHeight = j;
}

int checkValid(char c) {
  if (c == WALL || c == SPACE || c == PATH || c == '\n') {
    return SUCCESS;
  }
  else {
    return FAIL;
  }
}

/* READ FILE SHOULD REMOVE ALL SPACE FROM LEFT SIDE TO START OF MAZE */

int findStart(char maze[MAXHEIGHT][MAXWIDTH], int *startx, int *starty) {
  int i;
  for (i = 0; i < MAXHEIGHT; i++) {
    if (maze[i][0] == SPACE) {
      *startx = i;
    }
  }
  for (i = 0; i < MAXWIDTH; i++) {
    if (maze[0][i] == SPACE) {
      *starty = i;
    }
  }
  return SUCCESS;
}

int explore(int x, int y, char maze[MAXHEIGHT][MAXWIDTH], int *wC, int *mazeWidth, int *mazeHeight) {
  if (x == *mazeWidth || y == *mazeHeight) {
    printMaze(maze, mazeWidth, mazeHeight);
    fprintf(stdout, "%d %d\n", *mazeWidth, *mazeHeight);
    return SUCCESS;
  }
  /* IF NEW POSITION IS SPACE, TURN IN TO PATH */
  if (maze[x][y] == SPACE) {
    (*wC) = 0;
    maze[x][y] = PATH;
    delay(DELAY);
    printMaze(maze, mazeWidth, mazeHeight);
  }
  /* IF NEW POSITION IS WALL, DON'T EXPLORE THIS DIRECTION */
  else if (maze[x][y] == WALL || maze[x][y] == TEMPWALL) {
    (*wC)++;
    return FAIL;
  }

  else if (maze[x][y] == PATH) {
    return FAIL;
  }
  /* IF WE CAN GO RIGHT */
  if (explore(x+1, y, maze, wC, mazeWidth, mazeHeight)) {
    return SUCCESS;
  }
  /* IF WE CAN GO UP */
  if (explore(x, y+1, maze, wC, mazeWidth, mazeHeight)) {
    return SUCCESS;
  }
  /* IF WE CAN GO LEFT */
  if (explore(x-1, y, maze, wC, mazeWidth, mazeHeight)) {
    return SUCCESS;
  }
  /* IF WE CAN GO DOWN */
  if (explore(x, y-1, maze, wC, mazeWidth, mazeHeight)) {
    return SUCCESS;
  }

  /* IF SURROUNDED BY WALLS, DEFINITELY DEADEND, SO REPLACE WITH TEMPORARY WALL */
  if ((*wC) > 2) {
    maze[x][y] = TEMPWALL;
  }
  return FAIL;
}

void printMaze(char maze[MAXHEIGHT][MAXWIDTH], int *mazeWidth, int *mazeHeight) {
  int x, y;
  for (x=0;x<(*mazeHeight);x++) {
    for (y=0;y<(*mazeWidth);y++) {
      if (maze[y][x] == TEMPWALL) {
        maze[y][x] = SPACE;
      }
      fprintf(stdout, "%c", maze[y][x]);
    }
  }
}

/* Function to delay each array print - include <time.h> */
void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
