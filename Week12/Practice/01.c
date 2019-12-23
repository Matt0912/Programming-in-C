#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAXWORDS 100
#define MAXWORDLEN 50
#define FILENAME "01.no"

enum ErrorCodes {};

typedef struct prog {
  char words[MAXWORDS][MAXWORDLEN];
  int currWord;
  int totalWords;
} Program;

void initProgram(Program *p);
int prog(Program *p);
int code(Program *p);
int statement(Program *p);
void test(void);

int main(void) {
  /*
  int i = 0;
  FILE *fp;
  Program p;*/
  test();

  /*
  if ((fp = fopen(FILENAME, "r") == NULL) {
    fprintf(stderr, "Unable to open %s - please try again\n", FILENAME);
    exit(1);
  }

  while (fscanf(fp, "%s", p.words[i]) != NULL && i < MAXWORDS) {
    i++;
  }
  */
}

void initProgram(Program *p) {
  int i;
  p.currWord = 0;
  for (i = 0; i < MAXWORDS; i++) {
    p.words[i][0] = '\0';
  }
}

void test(void) {
  Program testP;
  testProg1 = {"BEGIN", "ONE", "END"};
  testProg2 = {"BEGIN", "END"};
  testProg3 = {"BEGIN", "ONE", "END"};

  initProgram(&testP);
  assert(testP.currWord == 0);
  assert(testP.words[10] == NULL);
  assert(testP.words[MAXWORDS-1] == NULL);






}
