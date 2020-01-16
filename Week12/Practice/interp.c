#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* SET TEST = 1 TO RUN TESTS, SET TEST = 0 TO DISABLE TESTS */
#define TEST 1
#include <assert.h>

#define MAXWORDS 100
#define MAXWORDLEN 50
#define FILENAME "01.no"

enum ErrorCodes {BeginERROR = 101, OneZeroERROR = 102, EndERROR = 103};
enum bool {FAIL, PASS};

typedef struct prog {
  char words[MAXWORDS][MAXWORDLEN];
  int currWord;
  int totalWords;
  int errorState;
} Program;

void initProgram(Program *p);
void prog(Program *p);
void code(Program *p);
void statement(Program *p);
void printState(Program *p);
void test(void);

int main(int argc, char** argv) {
  int i = 0, runTests = TEST;
  FILE *fp;
  Program p;
  if (runTests) {
    test();
  }

  if (argc == 2) {
    if ((fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Unable to open %s - please try again\n", argv[1]);
      exit(1);
    }
  }
  else {
    if ((fp = fopen(FILENAME, "r")) == NULL) {
      fprintf(stderr, "Unable to open %s - please try again\n", FILENAME);
      exit(1);
    }
  }

  initProgram(&p);
  while (fscanf(fp, "%s", p.words[i]) != EOF) {
    i++;
  }
  p.totalWords = i;

  prog(&p);
  printf("\n");
  printState(&p);

  return PASS;
}

void initProgram(Program *p) {
  int i;
  p->currWord = 0;
  for (i = 0; i < MAXWORDS; i++) {
    p->words[i][0] = '\0';
  }
  p->errorState = PASS;
}

void prog(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "BEGIN") != 0) {
    p->errorState = BeginERROR;
    return;
  }
  (p->currWord)++;
  code(p);
}

void code(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "END") == 0) {
    fprintf(stdout, "\n");
    return;
  }
  statement(p);
  (p->currWord)++;
  code(p);
}

void statement(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "ONE") == 0) {
    fprintf(stdout, " 1 ");
    return;
  }
  if (strcmp(p->words[p->currWord], "NOUGHT") == 0) {
    fprintf(stdout, " 0 ");
    return;
  }
  if (p->currWord >= p->totalWords) {
    p->errorState = EndERROR;
    return;
  }
  p->errorState = OneZeroERROR;
  return;
}

void printState(Program *p) {
  switch (p->errorState) {
    case PASS:
      fprintf(stderr, "Parsed OK\n");
      fprintf(stderr, "Interpreted OK\n");
      break;
    case BeginERROR:
      fprintf(stderr, "\nERROR - Expected a BEGIN statement at word %d\n", p->currWord);
      break;
    case OneZeroERROR:
      fprintf(stderr, "\nERROR - Expected a ONE or ZERO at word %d\n", p->currWord);
      break;
    case EndERROR:
      fprintf(stderr, "\nERROR - Expected an END statement at word %d\n", p->currWord);
      break;
  }
}

void test(void) {
  Program testP;
  int i = 0, numElements;
  char testProg1[MAXWORDS][MAXWORDLEN] = {"BEGIN", "END"};
  char testProg2[MAXWORDS][MAXWORDLEN] = {"BEGIN", "ONE", "END"};
  char testProg3[MAXWORDS][MAXWORDLEN] = {"BEGIN", "ONE"};
  char testProg4[MAXWORDS][MAXWORDLEN] = {"BEGIN", "ONE", "NOUGHT", "NOUGHT", "END"};
  char testProg5[MAXWORDS][MAXWORDLEN] = {"BEGIN", "ONE", "NOUGHT", "ONE", "END"};
  char testProg6[MAXWORDS][MAXWORDLEN] = {"BEGIN", "ONE", "NOUGHT", "TWO", "END"};
  char testProg7[MAXWORDS][MAXWORDLEN] = {"ONE", "NOUGHT", "TWO", "END"};

  freopen("TESTOUTPUT.txt", "w", stdout);

  fprintf(stderr, "\nINITIALISING TESTS...\n");



  /* Test initProgram */
  initProgram(&testP);
  assert(testP.currWord == 0);
  assert(testP.words[10][0] == '\0');
  assert(testP.words[MAXWORDS-1][0] == '\0');
  assert(strcmp(testP.words[MAXWORDS-1], "") == 0);
  assert(strcmp(testP.words[0], "") == 0);
  assert(testP.errorState == PASS);

  /* Test testProg1 string */
  numElements = 2;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg1[i]);
  }
  assert(strcmp(testP.words[0], "BEGIN") == 0);
  assert(strcmp(testP.words[1], "END") == 0);
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == PASS);

  /* Test testProg2 string */
  initProgram(&testP);
  numElements = 3;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg2[i]);
  }
  assert(strcmp(testP.words[0], "BEGIN") == 0);
  assert(strcmp(testP.words[1], "ONE") == 0);
  assert(strcmp(testP.words[2], "END") == 0);
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == PASS);

  /* Test testProg3 string */
  initProgram(&testP);
  numElements = 2;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg3[i]);
  }
  assert(strcmp(testP.words[0], "BEGIN") == 0);
  assert(strcmp(testP.words[1], "ONE") == 0);
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == EndERROR);

  /* Test testProg4 string */
  initProgram(&testP);
  numElements = 5;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg4[i]);
  }
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == PASS);

  /* Test testProg5 string */
  initProgram(&testP);
  numElements = 5;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg5[i]);
  }
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == PASS);

  /* Test testProg6 string */
  initProgram(&testP);
  numElements = 5;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg6[i]);
  }
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == OneZeroERROR);

  /* Test testProg7 string */
  initProgram(&testP);
  numElements = 4;
  testP.totalWords = numElements;
  for (i = 0; i < numElements; i++) {
    strcpy(testP.words[i], testProg7[i]);
  }
  assert(testP.errorState == PASS);
  prog(&testP);
  assert(testP.errorState == BeginERROR);

  fprintf(stderr, "\nALL TESTS PASSED\n\n");

  freopen("/dev/tty","w",stdout);

}
