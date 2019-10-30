/* IF SURROUNDED BY WALLS, CALL DEADEND FUNCTION */
if ((*wC) > 2) {
  deadEnd(x, y, maze, wC);
}

/* CHECKS SURROUNDING AREA FOR PATH, MOVES BACK ALONG UNTIL FINDS ALTERNATE ROUTE */
void deadEnd(int x, int y, char maze[HEIGHT][WIDTH], int *wC) {
  if (maze[x-1][y] == PATH) {
    maze[x][y] = WALL;
    maze[x-1][y] = SPACE;
    (*wC) = 0;
    explore(x-1, y, maze, wC);
  }
  if (maze[x+1][y] == PATH) {
    maze[x][y] = WALL;
    maze[x+1][y] = SPACE;
    (*wC) = 0;
    explore(x+1, y, maze, wC);
  }
  if (maze[x][y-1] == PATH) {
    maze[x][y] = WALL;
    maze[x][y-1] = SPACE;
    (*wC) = 0;
    explore(x, y-1, maze, wC);
  }
  if (maze[x][y+1] == PATH) {
    maze[x][y] = WALL;
    maze[x][y+1] = SPACE;
    (*wC) = 0;
    explore(x, y+1, maze, wC);
  }
}
