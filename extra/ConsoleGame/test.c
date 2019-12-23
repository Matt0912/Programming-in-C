#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "neillncurses.h"

#define WIDTH 150
#define HEIGHT 50

enum vars {grass = ' ', path = '.', water = '-',
           mountain = '#', player = '*', tree = '^'};

void initialiseMap(char map[HEIGHT][WIDTH]);

int main(void) {
  NCURS_Simplewin sw;
  char map[HEIGHT][WIDTH];

  initialiseMap(map);

  Neill_NCURS_Init(&sw);
  Neill_NCURS_CharStyle(&sw, " ", COLOR_GREEN, COLOR_GREEN, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "-", COLOR_BLUE, COLOR_BLUE, A_NORMAL);
  Neill_NCURS_CharStyle(&sw, "#", COLOR_WHITE, COLOR_WHITE, A_BOLD);

  Neill_NCURS_PrintArray(&map[0][0], WIDTH, HEIGHT, &sw);
  do{
     Neill_NCURS_Delay(100);
     Neill_NCURS_Events(&sw);
  }while(!sw.finished);

  atexit(Neill_NCURS_Done);
  exit(EXIT_SUCCESS);

  return 0;

}

void initialiseMap(char map[HEIGHT][WIDTH]) {
  int riverStart = WIDTH*2/3, i, j;

  /* fill the array with grass */
  for(j=0; j<HEIGHT; j++){
     for(i=0; i<WIDTH; i++){
        map[j][i] = grass;
     }
  }

  /* Draw the river */
  for (j = 0; j < HEIGHT; j++) {
    for (i = riverStart; i < riverStart + 5; i++) {
      map[j][i] = water;
    }
  }



}
