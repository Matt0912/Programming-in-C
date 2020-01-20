#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

/* Want to be able to have resizeable arrays of any data type */
#include "mvm.h"

#define MAXWORDLEN 100
#define MAXWORDS 50
#define FLOAT_INIT_VAL 1000000000000

enum ErrorCodes {StartERROR = 101, SyntaxERROR = 102, OverflowERROR = 103,
                 FunctionERROR = 104, VariableERROR = 105, FileERROR = 106,
                 CompERROR = 107, EndERROR = 108, Abort = 200};
enum CompStates {Greater = 1, Equal = 0, Less = -1, Invalid = -100};
enum check {FAIL, PASS};
typedef enum {FALSE, TRUE} bool;

typedef struct prog {
  char **words;
  int currWord;
  int totalWords;
  mvm *vars;
  int errorState;
} Program;

FILE* readFile(int argc, char** argv, int *maxWords);
void initProgram(Program *p, int maxWords);
void fillWords(Program *p, FILE *fp);

void prog(Program *p);
void instrs(Program *p);
void instruct(Program *p);
/* All possible instructions */
void file(Program *p, bool *checked);
void abortEX(Program *p, bool *checked);
void input(Program *p, bool *checked);
void ifcond(Program *p, bool *checked);
void inc(Program *p, bool *checked);
void set(Program *p, bool *checked);
void jump(Program *p, bool *checked);
void print(Program *p, bool *checked);
void rnd(Program *p, bool *checked);
/* All variables/constants */
void varcon(Program *p);
bool var(Program *p);
bool con(Program *p);
bool strvar(Program *p);
bool numvar(Program *p);
bool strcon(Program *p);
bool numcon(Program *p);

int checkCond(Program *p);
int compCons(char *strcon1, float numcon1, char *strcon2, float numcon2);

void printVarCon(Program *p);
char* formatStrcon(char *currentWord);
char *printVars(Program *p);

void ROTDecode(char *ROTString);
void nextWord(Program *p);
bool isInteger(char *number);
void printState(Program *p);
void freeProgram(Program *p, int maxWords);

void test(void);
void testVarCon(void);
void testGrammarFunc(void);

int main(int argc, char** argv) {
  Program p;
  int maxSize;
  FILE *fp;
  srand(time(NULL));
  test();

  fp = readFile(argc, argv, &maxSize);
  initProgram(&p, maxSize);
  fillWords(&p, fp);

  prog(&p);
  /*
  oldData = mvm_print(p.vars);
  printf("%s\n", oldData);*/

  printState(&p);

  mvm_free(&p.vars);
  freeProgram(&p, maxSize);
  fclose(fp);

  return 0;
}

/* Read in file to program p - all ROT18 & strings stored as 1 word */
FILE* readFile(int argc, char** argv, int *maxWords) {
  FILE *fp;

  if (argc == 2) {
    if ((fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Unable to open %s - please try again\n", argv[1]);
      exit(1);
    }
  }
  else {
    fprintf(stderr, "Invalid usage, try: ./interp file.nal\n");
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  /* Size = # of bytes */
  *maxWords = ftell(fp);
  rewind(fp);

  return fp;
}

void fillWords(Program *p, FILE *fp) {
  int i = 0, length, currentSize = MAXWORDLEN;
  char currentWord[MAXWORDLEN];
  char *currentString = (char*)calloc(MAXWORDLEN,sizeof(char));
  bool string = FALSE, rot18 = FALSE;

  while (fscanf(fp, "%s", currentWord) != EOF) {
    /* If words are part of a string, add them to current string */
    if (string == TRUE || rot18 == TRUE) {
      length = strlen(currentString) + strlen(currentWord) + 1;
      if (length > currentSize/2) {
        currentSize *= 2;
        currentString = realloc(currentString, currentSize);
      }
      sprintf(currentString, "%s %s", currentString, currentWord);
      if (currentString[length-1] == '"') {
        string = FALSE;
        p->words[i] = realloc(p->words[i], currentSize);
        strcpy(p->words[i], currentString);
        i++;
      }
      if (currentString[length - 1] == '#') {
        rot18 = FALSE;
        p->words[i] = realloc(p->words[i], currentSize);
        strcpy(p->words[i], currentString);
        i++;
      }
    }
    /* If word is starting a string */
    else if (currentWord[0] == '"') {
      length = strlen(currentWord);
      if (currentWord[length-1] == '"') {
        strcpy(p->words[i], currentWord);
        i++;
      }
      else {
        string = TRUE;
        strcpy(currentString, currentWord);
      }
    }
    else if (currentWord[0] == '#') {
      length = strlen(currentWord);
      if (currentWord[length-1] == '#') {
        strcpy(p->words[i], currentWord);
        i++;
      }
      else {
        rot18 = TRUE;
        strcpy(currentString, currentWord);
      }
    }
    /* If word is not a string, simply copy it in */
    else {
      strcpy(p->words[i], currentWord);
      i++;
    }
  }
  p->totalWords = i;
  free(currentString);
}

void test(void) {
  Program testP;
  int i, size;
  char testString[MAXWORDLEN];
  char *ROTStrings[] = {"URYYB", "ABCDEFG", "JKLM", "hello",
                                  "While9", "123", "09812jkl"};
  char *decodedStrings[] = {"HELLO", "NOPQRST", "WXYZ", "uryyb",
                                      "Juvyr4", "678", "54367wxy"};

  /* Redirect stdout to a new test file */
  freopen("TESTOUTPUT.txt", "w", stdout);

  printf(" INITIALISING TESTS ... \n");
  /* TEST INITPROGRAM */
  initProgram(&testP, MAXWORDS);
  assert(testP.currWord == 0);
  assert(testP.words[10][0] == '\0');
  assert(testP.words[MAXWORDS-1][0] == '\0');
  assert(strcmp(testP.words[MAXWORDS-1], "") == 0);
  assert(strcmp(testP.words[0], "") == 0);
  assert(testP.errorState == PASS);

  /* TEST NEXT WORD FUNCTION */
  strcpy(testP.words[0], "FIRST CELL");
  strcpy(testP.words[1], "SECOND CELL");
  strcpy(testP.words[2], "THIRD CELL");
  testP.totalWords = 3;
  assert(strcmp(testP.words[testP.currWord], "FIRST CELL") == 0);
  nextWord(&testP);
  assert(strcmp(testP.words[testP.currWord], "SECOND CELL") == 0);
  nextWord(&testP);
  assert(strcmp(testP.words[testP.currWord], "THIRD CELL") == 0);
  nextWord(&testP);
  nextWord(&testP);
  assert(testP.errorState == EndERROR);
  freeProgram(&testP, MAXWORDS);

  /* TEST ROTDECODE FUNCTION */
  size = sizeof(ROTStrings)/sizeof(ROTStrings[0]);
  for (i = 0; i < size; i++) {
    strcpy(testString, ROTStrings[i]);
    ROTDecode(testString);
    assert(strcmp(testString, decodedStrings[i]) == 0);
  }

  /* TEST VARIABLE/CONSTANT FUNCTIONS */
  testVarCon();
  /* TEST FORMAL GRAMMAR FUNCTIONS */
  testGrammarFunc();

  printf(" TESTING COMPLETE \n");

  /* Once testing is done, restore stdout */
  freopen("/dev/tty","w",stdout);
}

void testVarCon(void) {
  /* TESTING VARIABLE/CONSTANT FUNCTIONS */
  /* All test cases are shown in the xxxxxData arrays to easily see if any
  // have been missed */
  Program testP;
  int i, j = 0, size1, size2, size3, size4;
  bool currentBool;

  /************************** STRVAR *********************************/
  char strvarData[][MAXWORDLEN] = {"$A", "$ABCD", "$ZYQ", "$HI=",
                                  "ABC", "$ZA1", "$", "$Hi", "$%A"};
  bool strvarBool[] = {TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
  int strvarError[] = {PASS, PASS, PASS, SyntaxERROR, PASS, SyntaxERROR,
                       PASS, SyntaxERROR, SyntaxERROR};
  /************************** NUMVAR *********************************/
  char numvarData[][MAXWORDLEN] = {"%E", "%JAZZ", "%1", "%", "%$E"};
  bool numvarBool[] = {TRUE, TRUE, FALSE, FALSE, FALSE};
  int numvarError[] = {PASS, PASS, SyntaxERROR, PASS, SyntaxERROR};
  /************************** STRCON *********************************/
  char strconData[][MAXWORDLEN] = {"\"HELLO.TXT\"","\"HELLO there\"",
                                  "\"HELLO.TXT", "'HELLO.TXT'", "#URYYB.GKG#",
                                  "#UR Y  YB  GKG#", "#URYYB.GKG", "URYYB.GKG",
                                  "#URYYB176.GKG#", "#URYYB176@/1#.GKG#",
                                   "\"", "##"};
  bool strconBool[] = {TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE,
                       TRUE, TRUE, FALSE, FALSE};
  int strconError[] = {PASS, PASS, SyntaxERROR, PASS, PASS, PASS, SyntaxERROR,
                       PASS, PASS, PASS, SyntaxERROR, SyntaxERROR};
  /************************** NUMCON *********************************/
  char numconData[][MAXWORDLEN] = {"14.103", "1", "140589400000000019909",
                                  "0.153201", "-6", "-17.8", "12.6.5",
                                  "0.15L21", "k", ".", "-"};
  bool numconBool[] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
                       FALSE, FALSE};
  int numconError[] = {PASS, PASS, PASS, PASS, PASS, PASS, SyntaxERROR, PASS,
                       PASS, SyntaxERROR, SyntaxERROR};

  size1 = sizeof(strvarData)/sizeof(strvarData[0]);
  size2 = sizeof(numvarData)/sizeof(numvarData[0]);
  size3 = sizeof(strconData)/sizeof(strconData[0]);
  size4 = sizeof(numconData)/sizeof(numconData[0]);
  /* TEST STRVAR */
  initProgram(&testP, MAXWORDS);
  testP.totalWords = size1;
  for (i = 0; i < size1; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i], strvarData[i]);
    assert(strvar(&testP) == strvarBool[i]);
    assert(testP.errorState == strvarError[i]);
    nextWord(&testP);
  }
  freeProgram(&testP, MAXWORDS);

  /* TEST NUMVAR */
  initProgram(&testP, MAXWORDS);
  testP.totalWords = size2;
  for (i = 0; i < size2; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i], numvarData[i]);
    assert(numvar(&testP) == numvarBool[i]);
    assert(testP.errorState == numvarError[i]);
    nextWord(&testP);
  }
  freeProgram(&testP, MAXWORDS);

  /* TEST STRCON */
  initProgram(&testP, MAXWORDS);
  testP.totalWords = size3;
  for (i = 0; i < size3; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i], strconData[i]);
    assert(strcon(&testP) == strconBool[i]);
    assert(testP.errorState == strconError[i]);
    nextWord(&testP);
  }
  freeProgram(&testP, MAXWORDS);

  /* TEST NUMCON */
  initProgram(&testP, MAXWORDS);
  testP.totalWords = size4;
  for (i = 0; i < size4; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i], numconData[i]);
    assert(numcon(&testP) == numconBool[i]);
    assert(testP.errorState == numconError[i]);
    nextWord(&testP);
  }
  freeProgram(&testP, MAXWORDS);

  /* TEST VAR  - All tests for strvar/numvar should pass */
  initProgram(&testP, MAXWORDS);
  testP.totalWords = size1 + size2 + size3 + size4;
  for (i = 0; i < size1; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i], strvarData[i]);
    assert(var(&testP) == strvarBool[i]);
    assert(testP.errorState == strvarError[i]);
    nextWord(&testP);
  }
  for (i = 0; i < size2; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i + size1], numvarData[i]);
    assert(var(&testP) == numvarBool[i]);
    assert(testP.errorState == numvarError[i]);
    nextWord(&testP);
  }

  /* TEST CON  - All tests for strcon/numcon should pass */
  for (i = 0; i < size3; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i + size1 + size2], strconData[i]);
    assert(con(&testP) == strconBool[i]);
    assert(testP.errorState == strconError[i]);
    nextWord(&testP);
  }
  for (i = 0; i < size4; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[i + size1 + size2 + size3], numconData[i]);
    assert(con(&testP) == numconBool[i]);
    assert(testP.errorState == numconError[i]);
    nextWord(&testP);
  }

  /* TEST VARCON - ALL WRONG INPUTS SHOULD NOW TRIGGER SYNTAX ERRORS */
  while (testP.errorState != EndERROR) {
    if (j < size1) {
      currentBool = strvarBool[i];
      i = j;
    }
    if (j >= size1 && j < size2 + size1) {
      i = j - size1;
      currentBool = numvarBool[i];
    }
    if (j >= size1 + size2 && j < size3 + size2 + size1) {
      i = j - (size1 + size2);
      currentBool = strconBool[i];
    }
    if (j >= size1 + size2 + size3 && j < testP.totalWords) {
      i = j - (size1 + size2 + size3);
      currentBool = numconBool[i];
    }
    testP.errorState = PASS;
    varcon(&testP);
    if (currentBool == TRUE) {
      assert(testP.errorState == PASS);
    }
    if (currentBool == FALSE) {
      assert(testP.errorState == SyntaxERROR);
    }
    nextWord(&testP);
    j++;
  }
  freeProgram(&testP, MAXWORDS);

}

void testGrammarFunc(void) {
  Program testP;
  int i, size;
  float num;
  char *testStr;

  char fileTests[][MAXWORDLEN] = {"\"WOWeee.txt\"", "\"file19n123.nal\"", "#WOW.TXT#", "##"};
  int fileErrorStates[] = {FileERROR, FileERROR, FileERROR, SyntaxERROR};

  /*
  char inputStrTests1[][MAXWORDLEN] = {"$C", "$STRVAR", "VAR1"};
  char inputStrTests2[][MAXWORDLEN] = {"$STRVAR", "$TEST", "VAR2"};
  int inputStrErrorStates[] = {PASS, PASS, SyntaxERROR};
  char inputNumTests[][MAXWORDLEN] = {"%VAR", "TEST"};
  int inputNumErrorStates[] = {PASS, SyntaxERROR};*/

  char ifcondTests1[][MAXWORDLEN] = {"\"Hello\"", "$ABC", "%D", "19.07", "#YUI#",
                                    "\"hey", "15.3", "\"NINE\""};
  char ifcondTests2[][MAXWORDLEN] = {"$ABC", "\"Hello\"", "19.07", "%D", "$ABC",
                                    "\"hey", "\"NINE\"", "15.3"};
  int ifcondErrorStates[] = {PASS, PASS, PASS, PASS, PASS, SyntaxERROR,
                             CompERROR, CompERROR};

  char incNums[][MAXWORDLEN] = {"15", "432.1", "-10", "67.0", "23", "0"};
  char incNumsResults[][MAXWORDLEN] = {"16", "433.10", "-9.00", "68.00", "24", "1"};
  char incTests[][MAXWORDLEN] = {"%A", "%C ", "%", "$ABC", "93", ".%K", "%R1"};
  int incErrorStates[] = {PASS, SyntaxERROR, SyntaxERROR, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR, SyntaxERROR};


  /* All 5 arrays should be same size */
  char setTestsSTR1[][MAXWORDLEN] = {"$A", "$STRVAR", "$D", "$D", "$ROT","$TEST", "$VAR", "$HI=", "$TEST"};
  char setTestsSTR2[][MAXWORDLEN] = {"\"hi\"", "$A", "\"A A a 8\"", "\"A2\"", "#URYYB#", "$VAR", "$VAR","$HI", "%ABC"};
  char setTestsSTR3[][MAXWORDLEN] = {"\"hi\"", "\"hi\"", "\"A A a 8\"", "\"A2\"", "#URYYB#", "$VAR", "$VAR","$HI", "%ABC"};
  char setTestsNUM1[][MAXWORDLEN] = {"%ABC", "%KP", "%D", "%D", "%NUM", "%NUM", "%TEST","%HI=", "%FIN"};
  char setTestsNUM2[][MAXWORDLEN] = {"17.3", "%ABC", "-100.5", "50", "%D", "%TEST", "%TEST", "1.3", "$TEST"};
  char setTestsNUM3[][MAXWORDLEN] = {"17.3", "17.3", "-100.5", "50", "50", "%TEST", "%TEST", "1.3", "$TEST"};
  int setErrorStates[] = {PASS, PASS, PASS, PASS, PASS, VariableERROR, VariableERROR, SyntaxERROR, SyntaxERROR};
  /* Use settests to test printvars */
  char printVarsOutputs1[][MAXWORDLEN] = {"hi", "hi", "A A a 8", "A2", "HELLO"};
  char printVarsOutputs2[][MAXWORDLEN] = {"17.3", "17.3", "-100.5", "50", "50"};

  char jumpTests[][MAXWORDLEN] = {"6", "-1", "-5.5", "10.2", "19.0",
                                  "51", "152"};
  int jumpErrorStates[] = {PASS, SyntaxERROR, SyntaxERROR,
                           SyntaxERROR, SyntaxERROR, OverflowERROR, OverflowERROR};

  char formatStrconTests[][MAXWORDLEN] = {"\"HELLO HOW is 3v3rYTh1nG @89\"", "#123#",
                                   "#URYYB.GKG#", "\"97hello32\"", "#HELLO.TXT#"};
  char formatStrconOutputs[][MAXWORDLEN] = {"HELLO HOW is 3v3rYTh1nG @89", "678",
                                           "HELLO.TXT", "97hello32", "URYYB.GKG"};

  char printTests[][MAXWORDLEN] = {"\"HELLO HOW is 3v3rYone /@89\"",
                                   "93075.459", "#URYYB.GKG#", "%", "$%ABC",
                                   "URYYB.GKG#", "97hello32"};
  char printTestOutputs[][MAXWORDLEN] = {"HELLO HOW is 3v3rYone /@89",
                                   "93075.459", "HELLO.TXT"};
  int printErrorStates[] = {PASS, PASS, PASS, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR, SyntaxERROR};

  char rndTests[][MAXWORDLEN] = {"(", "%E", ")",
                                 "(", "%NUMBERVAR", ")",
                                 "(", "%", ")",
                                 "(", "$P", ")",
                                 "(", "%E", "()",
                                 "[", "%HH", ")"};
  int rndErrorStates[] = {PASS, PASS, SyntaxERROR, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR};

  char functionErrorTests[][MAXWORDLEN] = {"jUMP", "PRINT ", "inc", "Set", "AB0RT"};

  /* PROG & INSTRS - test prog & opening/closing brackets */
  /* prog -> '{' -> instrs -> '}' */
  initProgram(&testP, MAXWORDS);
  /* Check empty array */
  prog(&testP);
  assert(testP.errorState == StartERROR);
  /* Check for no opening bracket */
  testP.errorState = PASS;
  strcpy(testP.words[0], "}");
  prog(&testP);
  assert(testP.errorState == StartERROR);
  /* Check for EndERROR - no closing bracket */
  testP.errorState = PASS;
  strcpy(testP.words[0], "{");
  prog(&testP);
  assert(testP.errorState == EndERROR);
  /* Check function works for { } */
  testP.errorState = PASS;
  strcpy(testP.words[1], "}");
  prog(&testP);
  assert(testP.errorState == PASS);
  freeProgram(&testP, MAXWORDS);

  /* INSTRUCT FUNCTIONS */
  /* prog -> '{' -> instrs -> INSTRUCT -> instrs -> */

  /* FILE FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "FILE");
  strcpy(testP.words[3], "}");
  size = sizeof(fileTests)/sizeof(fileTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[2], fileTests[i]);
    prog(&testP);
    assert(testP.errorState == fileErrorStates[i]);
  }
  freeProgram(&testP, MAXWORDS);

  /* ABORT FUNCTION TEST */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "ABORT");
  strcpy(testP.words[2], "}");
  prog(&testP);
  assert(testP.errorState == Abort);
  freeProgram(&testP, MAXWORDS);

  /* INPUT FUNCTION TESTS
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "IN2STR");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ",");
  strcpy(testP.words[6], ")");
  strcpy(testP.words[7], "}");
  size = sizeof(inputStrTests1)/sizeof(inputStrTests1[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], inputStrTests1[i]);
    strcpy(testP.words[5], inputStrTests2[i]);
    prog(&testP);
    assert(testP.errorState == inputStrErrorStates[i]);
  }

  strcpy(testP.words[1], "INNUM");
  strcpy(testP.words[4], ")");
  strcpy(testP.words[5], "}");
  size = sizeof(inputNumTests)/sizeof(inputNumTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], inputNumTests[i]);
    prog(&testP);
    assert(testP.errorState == inputNumErrorStates[i]);
  }
  freeProgram(&testP, MAXWORDS);*/

  /* IFCOND FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "IFEQUAL");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ",");
  strcpy(testP.words[6], ")");
  strcpy(testP.words[7], "{");
  strcpy(testP.words[8], "}");
  strcpy(testP.words[9], "}");
  mvm_insert(testP.vars, "$ABC", "\"testing\"");
  mvm_insert(testP.vars, "%D", "900.4");
  size = sizeof(ifcondTests1)/sizeof(ifcondTests1[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests1[i]);
    strcpy(testP.words[5], ifcondTests2[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
  }
  strcpy(testP.words[1], "IFGREATER");
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests1[i]);
    strcpy(testP.words[5], ifcondTests2[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
  }
  freeProgram(&testP, MAXWORDS);

  /* INC FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "INC");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ")");
  strcpy(testP.words[5], "}");
  size = sizeof(incNums)/sizeof(incNums[0]);
  strcpy(testP.words[3], "%A");
  for (i = 0; i < size; i++) {
    mvm_insert(testP.vars, "%A", incNums[i]);
    prog(&testP);
    assert(strcmp(mvm_search(testP.vars, "%A"), incNumsResults[i]) == 0);
  }
  size = sizeof(incTests)/sizeof(incTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], incTests[i]);
    prog(&testP);
    assert(testP.errorState == incErrorStates[i]);
  }
  freeProgram(&testP, MAXWORDS);

  /* SET FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[2], "=");
  strcpy(testP.words[4], "}");
  size = sizeof(setTestsSTR1)/sizeof(setTestsSTR1[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[1], setTestsSTR1[i]);
    strcpy(testP.words[3], setTestsSTR2[i]);
    prog(&testP);
    assert(testP.errorState == setErrorStates[i]);
    testP.errorState = PASS;
    strcpy(testP.words[1], setTestsNUM1[i]);
    strcpy(testP.words[3], setTestsNUM2[i]);
    prog(&testP);
    assert(testP.errorState == setErrorStates[i]);
    if (setErrorStates[i] == PASS) {
      testStr = mvm_search(testP.vars, setTestsSTR1[i]);
      assert(strcmp(testStr, setTestsSTR3[i]) == 0);
      testStr = mvm_search(testP.vars, setTestsNUM1[i]);
      assert(strcmp(testStr, setTestsNUM3[i]) == 0);
    }
  }
  strcpy(testP.words[2], "!=");
  strcpy(testP.words[1], setTestsSTR1[0]);
  strcpy(testP.words[3], setTestsSTR2[0]);
  prog(&testP);
  assert(testP.errorState == SyntaxERROR);
  freeProgram(&testP, MAXWORDS);


  /* JUMP FUNCTION TESTS*/
  initProgram(&testP, MAXWORDS);
  testP.totalWords = 6;
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "JUMP");
  strcpy(testP.words[3], "$A");
  strcpy(testP.words[4], "=");
  strcpy(testP.words[5], "#jump#");
  strcpy(testP.words[6], "}");
  size = sizeof(jumpTests)/sizeof(jumpTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[2], jumpTests[i]);
    prog(&testP);
    assert(testP.errorState == jumpErrorStates[i]);
  }
  freeProgram(&testP, MAXWORDS);

  /* PRINT FUNCTION TESTS*/
  fprintf(stdout, "===================\n");
  fprintf(stdout, "PRINT TESTS \n \n");
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "PRINT");
  strcpy(testP.words[3], "}");
  size = sizeof(printTests)/sizeof(printTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[2], printTests[i]);
    prog(&testP);
    assert(testP.errorState == printErrorStates[i]);
    if (printErrorStates[i] == PASS) {
      fprintf(stdout, "Expected output: %s\n \n", printTestOutputs[i]);
    }
  }
  assert(testP.errorState == SyntaxERROR);
  freeProgram(&testP, MAXWORDS);
  fprintf(stdout, "===================\n");

  /*FORMATSTRCON TESTS */
  size = sizeof(formatStrconTests)/sizeof(formatStrconTests[0]);
  for (i = 0; i < size; i++) {
    testStr = formatStrcon(formatStrconTests[i]);
    assert(strcmp(testStr, formatStrconOutputs[i]) == 0);
    free(testStr);
  }


  /*PRINTVARS TESTS */
  initProgram(&testP, MAXWORDS);
  size = sizeof(setTestsSTR1)/sizeof(setTestsSTR1[0]);
  for (i = 0; i < size; i++) {
    if (setErrorStates[i] == PASS) {
      /* String tests */
      mvm_insert(testP.vars, setTestsSTR1[i], setTestsSTR3[i]);
      strcpy(testP.words[i], setTestsSTR1[i]);
      testP.currWord = i;
      testStr = printVars(&testP);
      assert(strcmp(testStr, printVarsOutputs1[i]) == 0);
      free(testStr);


      /* Numvar Tests */
      mvm_insert(testP.vars, setTestsNUM1[i], setTestsNUM3[i]);
      strcpy(testP.words[i], setTestsNUM1[i]);
      testStr = printVars(&testP);
      assert(strcmp(testStr, printVarsOutputs2[i]) == 0);
    }
  }
  freeProgram(&testP, MAXWORDS);



  /* RND FUNCTION TESTS*/
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "RND");
  strcpy(testP.words[5], "}");
  size = sizeof(rndTests)/sizeof(rndTests[0]);
  for (i = 0; i < size; i = i + 3) {
    testP.errorState = PASS;
    strcpy(testP.words[2], rndTests[i]);
    strcpy(testP.words[3], rndTests[i+1]);
    strcpy(testP.words[4], rndTests[i+2]);
    prog(&testP);
    assert(testP.errorState == rndErrorStates[i/3]);
    if (testP.errorState == PASS) {
      num = strtod(mvm_search(testP.vars, rndTests[i+1]), NULL);
      assert(num >= 0 && num < 100);
    }
  }
  freeProgram(&testP, MAXWORDS);

  /* TEST FUNCTIONERROR */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[3], "}");
  size = sizeof(functionErrorTests)/sizeof(functionErrorTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[1], functionErrorTests[i]);
    strcpy(testP.words[2], jumpTests[i]);
    prog(&testP);
    assert(testP.errorState == FunctionERROR);
  }
  freeProgram(&testP, MAXWORDS);

}

/* Initialise struct for storage of current program being parsed */
void initProgram(Program *p, int maxWords) {
  int i;
  p->currWord = 0;
  p->words = (char **)calloc(maxWords, sizeof(char*));
  for (i = 0; i < maxWords; i++) {
    p->words[i] = (char *)calloc(MAXWORDLEN, sizeof(char));
    p->words[i][0] = '\0';
  }
  p->vars = mvm_init();
  p->errorState = PASS;
}

void prog(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  p->currWord = 0;
  if (strcmp(p->words[p->currWord], "{") != 0) {
    p->errorState = StartERROR;
    return;
  }
  nextWord(p);
  instrs(p);
}

void instrs(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "}") == 0) {
    return;
  }
  instruct(p);
  nextWord(p);
  instrs(p);
}

void instruct(Program *p) {
  bool checked = FALSE;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  /* If instruct is called, every function is tested to see if it matches
  // Nothing is returned from each function, so no way of knowing if the
  // function has been called correctly - unless we check if the program
  // has moved on to the next word:
  // If it moves to the next word WITHOUT an errorState, function has been
  // called correctly and loop exits
  // If it moves to next word but has invalid future syntax, the functions
  // will catch it and change the error state, exiting the loop
  // If it goes through the whole while loop without going to the next word,
  // none of the functions have been called and a FunctionERROR is thrown */
  if (checked == FALSE) {
    file(p, &checked);
  }
  if (checked == FALSE) {
    abortEX(p, &checked);
  }
  if (checked == FALSE) {
    input(p, &checked);
  }
  if (checked == FALSE) {
    ifcond(p, &checked);
  }
  if (checked == FALSE) {
    inc(p, &checked);
  }
  if (checked == FALSE) {
    set(p, &checked);
  }
  if (checked == FALSE) {
    jump(p, &checked);
  }
  if (checked == FALSE) {
    print(p, &checked);
  }
  if (checked == FALSE) {
    rnd(p, &checked);
  }
  if (checked == FALSE && p->errorState == PASS) {
    p->errorState = FunctionERROR;
  }
}

/* ALL INSTRUCTION FUNCTIONS */

void file(Program *p, bool *checked) {
  char *fileStr, *printstr;
  FILE *fp;
  int maxWords;
  Program newP;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "FILE") == 0) {
    nextWord(p);
    if (strcon(p)) {
      fileStr = formatStrcon(p->words[p->currWord]);
      if ((fp = fopen(fileStr, "r")) == NULL) {
        p->errorState = FileERROR;
        free(fileStr);
        return;
      }
      fseek(fp, 0, SEEK_END);
      maxWords = ftell(fp);
      rewind(fp);
      initProgram(&newP, maxWords);
      fillWords(&newP, fp);
      newP.vars->head = p->vars->head;
      printstr = mvm_print(newP.vars);
      fprintf(stderr, "%s\n", printstr);
      printstr = mvm_print(p->vars);
      fprintf(stderr, "%s\n", printstr);
      prog(&newP);
      freeProgram(&newP, maxWords);
      fclose(fp);
      free(fileStr);
      *checked = TRUE;
      return;
    }
    p->errorState = SyntaxERROR;
  }
}

void abortEX(Program *p, bool *checked) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "ABORT") == 0) {
    p->errorState = Abort;
    *checked = TRUE;
    return;
  }
}

void input(Program *p, bool *checked) {
  char key1[MAXWORDLEN], key2[MAXWORDLEN], userInput1[MAXWORDLEN], userInput2[MAXWORDLEN];
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "IN2STR") == 0) {
    nextWord(p);
    if (strcmp(p->words[p->currWord], "(") == 0) {
      nextWord(p);
      if (strvar(p)) {
        strcpy(key1, p->words[p->currWord]);
        nextWord(p);
        if (strcmp(p->words[p->currWord], ",") == 0) {
          nextWord(p);
          if (strvar(p)) {
            strcpy(key2, p->words[p->currWord]);
            nextWord(p);
            if (strcmp(p->words[p->currWord], ")") == 0) {
              /*
              if (fgets(userInput, MAXWORDLEN, stdin)) {
                    userInput[strcspn(name, "\n")] = 0;

                }
              */
              scanf("%99s", userInput1);
              scanf("%99s", userInput2);
              mvm_insert(p->vars, key1, userInput1);
              mvm_insert(p->vars, key2, userInput2);
              *checked = TRUE;
              return;
            }
          }
        }
      }
    }
    p->errorState = SyntaxERROR;
  }

  if (strcmp(p->words[p->currWord], "INNUM") == 0) {
    nextWord(p);
    if (strcmp(p->words[p->currWord], "(") == 0) {
      nextWord(p);
      if (numvar(p)) {
        strcpy(key1, p->words[p->currWord]);
        nextWord(p);
        if (strcmp(p->words[p->currWord], ")") == 0) {
          scanf("%99s", userInput1);
          mvm_insert(p->vars, key1, userInput1);
          *checked = TRUE;
          return;
        }
      }
    }
    p->errorState = SyntaxERROR;
  }
}

void ifcond(Program *p, bool *checked) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }

  if (strcmp(p->words[p->currWord], "IFEQUAL") == 0) {
    nextWord(p);
    if (checkCond(p) == Equal) {
      nextWord(p);
      if (strcmp(p->words[p->currWord], "{") == 0) {
        nextWord(p);
        instrs(p);
        *checked = TRUE;
        return;
      }
      else if (strcmp(p->words[p->currWord], "{") != 0) {
        p->errorState = SyntaxERROR;
      }
    }
    else {
      while (strcmp(p->words[p->currWord], "}") != 0) {
        nextWord(p);
      }
      *checked = TRUE;
    }
  }

  if (strcmp(p->words[p->currWord], "IFGREATER") == 0) {
    nextWord(p);
    if (checkCond(p) == Greater) {
      nextWord(p);
      if (strcmp(p->words[p->currWord], "{") == 0) {
        nextWord(p);
        instrs(p);
        *checked = TRUE;
        return;
      }
      else if (strcmp(p->words[p->currWord], "{") != 0) {
        p->errorState = SyntaxERROR;
      }
    }
    else {
      while (strcmp(p->words[p->currWord], "}") != 0) {
        nextWord(p);
      }
      *checked = TRUE;
    }
  }
}

/* Change to bool, write function with cases for each state of p - str,
// num, var (var converts to either strcon or numcon) */
int checkCond(Program *p) {
  char *strcon1 = NULL, *strcon2 = NULL;
  float numcon1 = FLOAT_INIT_VAL, numcon2 = FLOAT_INIT_VAL;
  int result;
  if (p->errorState != PASS) {
    return Less;
  }
  if (strcmp(p->words[p->currWord], "(") == 0) {
    nextWord(p);
    varcon(p);
    if (strcon(p)) {
      strcon1 = formatStrcon(p->words[p->currWord]);
    }
    else if (numcon(p)) {
      numcon1 = strtod(p->words[p->currWord], NULL);
    }
    else if (strvar(p)) {
      strcon1 = printVars(p);
    }
    else if (numvar(p)) {
      numcon1 = strtod(printVars(p), NULL);
    }
    if (p->errorState == PASS) {
      nextWord(p);
      if (strcmp(p->words[p->currWord], ",") == 0) {
        nextWord(p);
        varcon(p);
        if (strcon(p)) {
          strcon2 = formatStrcon(p->words[p->currWord]);
        }
        else if (numcon(p)) {
          numcon2 = strtod(p->words[p->currWord], NULL);
        }
        else if (strvar(p)) {
          strcon2 = printVars(p);
        }
        else if (numvar(p)) {
          numcon2 = strtod(printVars(p), NULL);
        }
        if (p->errorState == PASS) {
          nextWord(p);
          if (strcmp(p->words[p->currWord], ")") == 0) {
            result = compCons(strcon1, numcon1, strcon2, numcon2);
            if (strcon1 != NULL) {
              free(strcon1);
            }
            if (strcon2 != NULL) {
              free(strcon2);
            }
            if (result == Invalid) {
              p->errorState = CompERROR;
            }
            return result;
          }
        }
      }
    }
  }
  p->errorState = SyntaxERROR;
  return Less;
}

/* Return 1 if first var is greater than, 0 if equal, -1 if first
// is smaller, -100 if invalid comparison */
int compCons(char *strcon1, float numcon1, char *strcon2, float numcon2) {
  if (strcon1 != NULL && strcon2 != NULL) {
    if (strcmp(strcon1, strcon2) > 0) {
      return Greater;
    }
    else if (strcmp(strcon1, strcon2) < 0) {
      return Less;
    }
    else if (strcmp(strcon1, strcon2) == 0) {
      return Equal;
    }
  }
  if (numcon1 < FLOAT_INIT_VAL && numcon2 < FLOAT_INIT_VAL) {
    if (numcon1 > numcon2) {
      return Greater;
    }
    else if (numcon1 < numcon2) {
      return Less;
    }
    else {
      return Equal;
    }
  }
  return Invalid;

}

void inc(Program *p, bool *checked) {
  double i;
  char newData[MAXWORDLEN], *oldData;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "INC") == 0) {
    nextWord(p);
    if (strcmp(p->words[p->currWord], "(") == 0) {
      nextWord(p);
      if (numvar(p)) {
        if (mvm_size(p->vars) > 0) {
          if (mvm_search(p->vars, p->words[p->currWord]) != NULL) {
            oldData = mvm_search(p->vars, p->words[p->currWord]);
            i = strtod(oldData, NULL);
            i = i + 1;
            if (isInteger(oldData)) {
              sprintf(newData, "%.0f", i);
            }
            else {
              sprintf(newData, "%.2f", i);
            }
            mvm_delete(p->vars, p->words[p->currWord]);
            mvm_insert(p->vars, p->words[p->currWord], newData);
            nextWord(p);
            if (strcmp(p->words[p->currWord], ")") == 0) {
              *checked = TRUE;
              return;
            }
          }
          else {
            p->errorState = VariableERROR;
            return;
          }
        }
      }
    }
    p->errorState = SyntaxERROR;
  }
}

/* Use mvm to store variables */
void set(Program *p, bool *checked) {
  char key[MAXWORDLEN], *result;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strvar(p)) {
    strcpy(key, p->words[p->currWord]);
    nextWord(p);
    if (strcmp(p->words[p->currWord], "=") == 0) {
      nextWord(p);
      if (strcon(p)) {
        mvm_insert(p->vars, key, p->words[p->currWord]);
        *checked = TRUE;
        return;
      }
      /* Can make one var equal another var if it already exists */
      else if (strvar(p)) {
        if (mvm_size(p->vars) > 0) {
          result = mvm_search(p->vars, p->words[p->currWord]);
          if (result != NULL) {
            mvm_insert(p->vars, key, result);
            *checked = TRUE;
            return;
          }
        }
        p->errorState = VariableERROR;
        return;
      }
    }
    p->errorState = SyntaxERROR;
    return;
  }

  if (numvar(p)) {
    strcpy(key, p->words[p->currWord]);
    nextWord(p);
    if (strcmp(p->words[p->currWord], "=") == 0) {
      nextWord(p);
      if (numcon(p)) {
        mvm_insert(p->vars, key, p->words[p->currWord]);
        *checked = TRUE;
        return;
      }
      /* Can make one var equal another var if it already exists */
      else if (numvar(p)) {
        if (mvm_size(p->vars) > 0) {
          result = mvm_search(p->vars, p->words[p->currWord]);
          if (result != NULL) {
            mvm_insert(p->vars, key, result);
            *checked = TRUE;
            return;
          }
        }
        p->errorState = VariableERROR;
        return;
      }
    }
    p->errorState = SyntaxERROR;
    return;
  }
}

/* Numcon must be integer & > 0  && numcon must be < totalWords*/
void jump(Program *p, bool *checked) {
  int i;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "JUMP") == 0) {
    nextWord(p);
    if (numcon(p)) {
      if (!isInteger(p->words[p->currWord])) {
        p->errorState = SyntaxERROR;
        return;
      }
      i = strtol(p->words[p->currWord], NULL, 10);
      if (i > p->totalWords) {
        p->errorState = OverflowERROR;
        return;
      }
      p->currWord = i-1;
      *checked = TRUE;
    }
    else {
      p->errorState = SyntaxERROR;
      return;
    }
  }
}

void print(Program *p, bool *checked) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "PRINT") == 0) {
    nextWord(p);
    varcon(p);
    /* Special case only for PRINT (Not PRINTN) */
    if (strcmp(p->words[p->currWord], "\"\"") == 0) {
      p->errorState = PASS;
      fprintf(stdout, "\n");
      *checked = TRUE;
      return;
    }
    printVarCon(p);
    fprintf(stdout, "\n");
    *checked = TRUE;
  }

  if (strcmp(p->words[p->currWord], "PRINTN") == 0) {
    nextWord(p);
    varcon(p);
    printVarCon(p);
    *checked = TRUE;
  }
}

void printVarCon(Program *p) {
  char *output;
  if (strcon(p)) {
    output = formatStrcon(p->words[p->currWord]);
    fprintf(stdout, "%s", output);
    free(output);
  }
  else if (numcon(p)) {
    fprintf(stdout,"%s", p->words[p->currWord]);
  }
  else if (var(p)) {
    if (mvm_size(p->vars) > 0) {
      output = printVars(p);
      if (output == NULL) {
        p->errorState = VariableERROR;
        return;
      }
      fprintf(stdout,"%s", output);
      if (strvar(p)) {
        free(output);
      }
    }
    else {
      p->errorState = VariableERROR;
    }
  }
}

char* formatStrcon(char *currentWord) {
  int length = strlen(currentWord);
  char *ptr, *outputString = (char *)calloc(1,length);

  ptr = currentWord;
  ptr++;
  memcpy(outputString, ptr, length - 1);
  length = length - 2;
  if (outputString[length] == '#') {
    ROTDecode(outputString);
  }
  outputString[length] = '\0';

  return outputString;
}

/* Search through mvm vars until it finds a constant */
char *printVars(Program *p) {
  char *data;
  if (strvar(p)) {
    if (mvm_size(p->vars) > 0) {
      data = mvm_search(p->vars, p->words[p->currWord]);
      if (data != NULL) {
        return formatStrcon(data);
      }
    }
  }
  if (numvar(p)) {
    if (mvm_size(p->vars) > 0) {
      data = mvm_search(p->vars, p->words[p->currWord]);
    }
  }
  return data;
}

void rnd(Program *p, bool *checked) {
  char newData[MAXWORDLEN], key[MAXWORDLEN];
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "RND") == 0) {
    nextWord(p);
    if (strcmp(p->words[p->currWord], "(") == 0) {
      nextWord(p);
      if (numvar(p)) {
        strcpy(key, p->words[p->currWord]);
        nextWord(p);
        if (strcmp(p->words[p->currWord], ")") == 0) {
          sprintf(newData, "%d", rand() % 100);
          mvm_insert(p->vars, key, newData);
          *checked = TRUE;
          return;
        }
      }
    }
    p->errorState = SyntaxERROR;
  }
}


/* ALL VARIABLE/CONSTANT FUNCTIONS */

void varcon(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (var(p) == con(p)) {
    p->errorState = SyntaxERROR;
  }
}

bool var(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  /* Use != as XOR */
  if (strvar(p) != numvar(p)) {
    return TRUE;
  }
  return FALSE;
}

bool con(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  /* Use != as XOR */
  if (strcon(p) != numcon(p)) {
    return TRUE;
  }
  return FALSE;
}

bool strvar(Program *p) {
  int wordLength = strlen(p->words[p->currWord]), i = 1;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  if (wordLength > 1) {
    if (p->words[p->currWord][0] == '$') {
      while (i < wordLength) {
        if (!isupper(p->words[p->currWord][i])) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
        i++;
      }
      return TRUE;
    }
  }
  return FALSE;
}

bool numvar(Program *p) {
  int wordLength = strlen(p->words[p->currWord]), i = 1;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  if (wordLength > 1) {
    if (p->words[p->currWord][0] == '%') {
      while (i < wordLength) {
        if (!isupper(p->words[p->currWord][i])) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
        i++;
      }
      return TRUE;
    }
  }
  return FALSE;
}

bool strcon(Program *p) {
  int length = strlen(p->words[p->currWord]);
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }

  /* Check if normal string */
  if (p->words[p->currWord][0] == '"') {
    if (p->words[p->currWord][length - 1] == '"') {
      if (length > 2) {
        return TRUE;
      }
    }
    p->errorState = SyntaxERROR;
    return FALSE;
  }

  /* Check if ROT18 encoded string */
  if (p->words[p->currWord][0] == '#') {
    if (p->words[p->currWord][length - 1] == '#') {
      if (length > 2) {
        return TRUE;
      }
    }
    p->errorState = SyntaxERROR;
  }
  return FALSE;
}

/* Can I make it so the default for the function isn't TRUE? */
bool numcon(Program *p) {
  int i = 0, length = strlen(p->words[p->currWord]), count1 = 0, count2 = 0;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  while (i < length) {
    if (isdigit(p->words[p->currWord][i]) || p->words[p->currWord][i] == '.'
        || p->words[p->currWord][i] == '-') {
      if (p->words[p->currWord][i] == '.') {
        count1++;
        if (count1 > 1 || length == 1) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
      }
      if (p->words[p->currWord][i] == '-') {
        count2++;
        if (count2 > 1 || length == 1) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
      }
      i++;
    }
    else {
      return FALSE;
    }
  }
  return TRUE;
}


/* Function to move to next word */
void nextWord(Program *p) {
  (p->currWord)++;/*
  printf("%s\n", p->words[p->currWord]); */
  if (p->words[p->currWord][0] == '\0') {
    p->errorState = EndERROR;
  }
}

bool isInteger(char *number) {
  int length = strlen(number), i;
  for (i = 0; i < length; i++) {
    if (number[i] == '.' || number[i] == '-') {
      return FALSE;
    }
  }
  return TRUE;
}

void ROTDecode(char *ROTString) {
  int i, length = strlen(ROTString);
  for (i = 0; i < length; i ++) {
    if (isupper(ROTString[i])) {
      if (ROTString[i] <= 'M') {
        ROTString[i] = ROTString[i] + 13;
      }
      else if (ROTString[i] > 'M') {
        ROTString[i] = ROTString[i] - 13;
      }
    }
    else if (islower(ROTString[i])) {
      if (ROTString[i] <= 'm') {
        ROTString[i] = ROTString[i] + 13;
      }
      else if (ROTString[i] > 'm') {
        ROTString[i] = ROTString[i] - 13;
      }
    }
    else if (isdigit(ROTString[i])) {
      if (ROTString[i] <= '4') {
        ROTString[i] = ROTString[i] + 5;
      }
      else if (ROTString[i] > '4') {
        ROTString[i] = ROTString[i] - 5;
      }
    }
  }
}

void printState(Program *p) {
  switch (p->errorState) {
    case PASS:
      fprintf(stderr, "Interpreted OK\n");
      break;
    case StartERROR:
      fprintf(stderr, "\nERROR - Expected a '{' at word %d: '%s'\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case SyntaxERROR:
      fprintf(stderr, "\nERROR - Invalid syntax at word %d: '%s', e.g. lowercase"
              " letters / missing quotation marks / wrong variable type \n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case OverflowERROR:
      fprintf(stderr, "\nERROR - Accessing code out of bounds at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case FunctionERROR:
      fprintf(stderr, "\nERROR - No valid function call at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case VariableERROR:
      fprintf(stderr, "\nERROR - Problem with variable at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case FileERROR:
      fprintf(stderr, "\nERROR - Invalid filename at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case CompERROR:
      fprintf(stderr, "\nERROR - Invalid variable comparison at word %d: %s\n",
            p->currWord, p->words[p->currWord - 1]);
      break;
    case EndERROR:
      fprintf(stderr, "\nERROR - Expected a '}' at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);
      break;
    case Abort:/*
      fprintf(stderr, "\nUser specified ABORT at word %d: %s\n",
              p->currWord, p->words[p->currWord - 1]);*/
      break;
  }
}

void freeProgram(Program *p, int maxWords) {
  int i;
  for (i = 0; i < maxWords; i++) {
    free(p->words[i]);
  }
  free(p->words);
}

/* StartERROR = No opening bracket, could be empty array
// SyntaxERROR = Invalid syntax - e.g. used lowercase letters/ missing quotation marks/ not using correct grammar rules
// OverflowERROR = Accessing something out of bounds
// FunctionERROR = No valid function call - more specific than syntax error
// VariableERROR = Error when assigning variables with set
// EndERROR = No valid closing bracket found at end of words list
// Abort = User specified abort */
