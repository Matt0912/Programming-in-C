#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

/* Want to be able to have resizeable arrays of any data type */
#include "mvm.h"

#define MAXWORDLEN 75

enum ErrorCodes {StartERROR = 101, SyntaxERROR = 102, EndERROR = 103};
enum check {FAIL, PASS};
typedef enum {FALSE, TRUE} bool;

typedef struct prog {
  mvm *words;
  mvmcell *currWord;
  int errorState;
} Program;

void initProgram(Program *p);
void prog(Program *p);
void instrs(Program *p);
void instruct(Program *p);
/* All possible instructions */
void file(Program *p);
void abortEX(Program *p);
void input(Program *p);
void ifcond(Program *p);
void inc(Program *p);
void set(Program *p);
void jump(Program *p);
void print(Program *p);
void rnd(Program *p);
/* All variables/constants */
void varcon(Program *p);
bool var(Program *p);
bool con(Program *p);
bool strvar(Program *p);
bool numvar(Program *p);
bool strcon(Program *p);
bool numcon(Program *p);

void nextWord(Program *p);
void test(void);
void testVarCon(void);
void testGrammarFunc(void);

int main(void) {
  test();

  return PASS;
}

void test(void) {
  Program testP;
  char *output;

  printf(" INITIALISING TESTS ... \n");
  /* TEST INITPROGRAM */
  initProgram(&testP);
  assert(testP.currWord == NULL);
  assert(mvm_size(testP.words) == 0);
  assert(testP.errorState == PASS);

  /* TEST MVM_INSERT */
  /* mvm_insert was modified to reverse insertion order so had to be tested */
  mvm_insert(testP.words, "0", "FIRST CELL");
  mvm_insert(testP.words, "1", "SECOND CELL");
  mvm_insert(testP.words, "2", "THIRD CELL");
  assert(strcmp(testP.words->head->data, "FIRST CELL") == 0);
  assert(strcmp(testP.words->head->key, "0") == 0);
  assert(strcmp(testP.words->head->next->data, "SECOND CELL") == 0);
  assert(strcmp(testP.words->head->next->key, "1") == 0);
  output = mvm_print(testP.words);
  assert(strcmp(output, "[0](FIRST CELL) [1](SECOND CELL) [2](THIRD CELL) ") == 0);

  /* TEST NEXT WORD FUNCTION */
  testP.currWord = testP.words->head;
  assert(strcmp(testP.currWord->data, "FIRST CELL") == 0);
  nextWord(&testP);
  assert(strcmp(testP.currWord->data, "SECOND CELL") == 0);
  nextWord(&testP);
  assert(strcmp(testP.currWord->data, "THIRD CELL") == 0);
  nextWord(&testP);
  assert(testP.errorState == EndERROR);
  mvm_free(&testP.words);

  /* TEST VARIABLE/CONSTANT FUNCTIONS */
  testVarCon();
  /* TEST FORMAL GRAMMAR FUNCTIONS */
  testGrammarFunc();

  printf(" TESTING COMPLETE \n");
}

void testVarCon(void) {
  /* TESTING VARIABLE/CONSTANT FUNCTIONS */
  /* All test cases are shown in the xxxxxData arrays to easily see if any
  // have been missed */
  Program testP;
  int i, size, count = 0;
  bool currentBool;
  char keys[][10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                     "11", "12", "13", "14", "15"};
  /************************** STRVAR *********************************/
  char strvarData[][MAXWORDLEN] = {"$A", "$ABCD", "$ZYQ",
                                  "ABC", "$ZA1", "$", "$Hi", "$%A"};
  bool strvarBool[] = {TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE};
  int strvarError[] = {PASS, PASS, PASS, PASS, SyntaxERROR,
                       PASS, SyntaxERROR, SyntaxERROR};
  /************************** NUMVAR *********************************/
  char numvarData[][MAXWORDLEN] = {"%E", "%JAZZ", "%1", "%", "%$E"};
  bool numvarBool[] = {TRUE, TRUE, FALSE, FALSE, FALSE};
  int numvarError[] = {PASS, PASS, SyntaxERROR, PASS, SyntaxERROR};
  /************************** STRCON *********************************/
  char strconData[][MAXWORDLEN] = {"\"HELLO.TXT\"","\"HELLO there\"",
                                  "\"HELLO.TXT", "'HELLO.TXT'", "#URYYB.GKG#",
                                  "#UR Y  YB  GKG#", "#URYYB.GKG", "URYYB.GKG",
                                  "#URYYB.GkG#", "#URYYB176.GKG#",
                                  "#URYYB176@/1#.GKG#"};
  bool strconBool[] = {TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE,
                       FALSE, TRUE, TRUE};
  int strconError[] = {PASS, PASS, SyntaxERROR, PASS, PASS, PASS,
                       SyntaxERROR, PASS, SyntaxERROR, PASS, PASS};
  /************************** NUMCON *********************************/
  char numconData[][MAXWORDLEN] = {"14.103", "1", "140589400000000019909",
                                  "0.153201", "12.6.5", "0.15L21", "k", "."};
  bool numconBool[] = {TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE};
  int numconError[] = {PASS, PASS, PASS, PASS, SyntaxERROR, PASS,
                       PASS, SyntaxERROR};

  /* TEST STRVAR */
  initProgram(&testP);
  size = sizeof(strvarData)/sizeof(strvarData[0]);
  mvm_insert(testP.words, keys[0], strvarData[0]);
  testP.currWord = testP.words->head;
  assert(strvar(&testP) == strvarBool[0]);
  for (i = 1; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], strvarData[i]);
    nextWord(&testP);
    assert(strvar(&testP) == strvarBool[i]);
    assert(testP.errorState == strvarError[i]);
  }
  mvm_free(&testP.words);

  /* TEST NUMVAR */
  initProgram(&testP);
  size = sizeof(numvarData)/sizeof(numvarData[0]);
  mvm_insert(testP.words, keys[0], numvarData[0]);
  testP.currWord = testP.words->head;
  assert(numvar(&testP) == TRUE);
  for (i = 1; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], numvarData[i]);
    nextWord(&testP);
    assert(numvar(&testP) == numvarBool[i]);
    assert(testP.errorState == numvarError[i]);
  }
  assert(testP.errorState == SyntaxERROR);
  mvm_free(&testP.words);

  /* TEST STRCON */
  initProgram(&testP);
  size = sizeof(strconData)/sizeof(strconData[0]);
  mvm_insert(testP.words, keys[0], strconData[0]);
  testP.currWord = testP.words->head;
  assert(strcon(&testP) == TRUE);
  for (i = 1; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], strconData[i]);
    nextWord(&testP);
    assert(strcon(&testP) == strconBool[i]);
    assert(testP.errorState == strconError[i]);
  }
  assert(testP.errorState == PASS);
  mvm_free(&testP.words);

  /* TEST NUMCON */
  initProgram(&testP);
  size = sizeof(numconData)/sizeof(numconData[0]);
  mvm_insert(testP.words, keys[0], numconData[0]);
  testP.currWord = testP.words->head;
  assert(numcon(&testP) == TRUE);
  for (i = 1; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], numconData[i]);
    nextWord(&testP);
    assert(numcon(&testP) == numconBool[i]);
    assert(testP.errorState == numconError[i]);
  }
  assert(testP.errorState == SyntaxERROR);
  mvm_free(&testP.words);

  /* TEST VAR  - All tests for strvar/numvar should pass */
  initProgram(&testP);
  size = sizeof(strvarData)/sizeof(strvarData[0]);
  mvm_insert(testP.words, keys[0], strvarData[0]);
  testP.currWord = testP.words->head;
  assert(var(&testP) == strvarBool[0]);
  for (i = 1; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], strvarData[i]);
    nextWord(&testP);
    assert(var(&testP) == strvarBool[i]);
    assert(testP.errorState == strvarError[i]);
  }
  size = sizeof(numvarData)/sizeof(numvarData[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], numvarData[i]);
    nextWord(&testP);
    assert(var(&testP) == numvarBool[i]);
    assert(testP.errorState == numvarError[i]);
  }
  assert(testP.errorState == SyntaxERROR);

  /* TEST CON  - All tests for strcon/numcon should pass */
  size = sizeof(strconData)/sizeof(strconData[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], strconData[i]);
    nextWord(&testP);
    assert(con(&testP) == strconBool[i]);
    assert(testP.errorState == strconError[i]);
  }
  size = sizeof(numconData)/sizeof(numconData[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, keys[i], numconData[i]);
    nextWord(&testP);
    assert(con(&testP) == numconBool[i]);
    assert(testP.errorState == numconError[i]);
  }
  assert(testP.errorState == SyntaxERROR);

  /* TEST VARCON - ALL WRONG INPUTS SHOULD NOW TRIGGER SYNTAX ERRORS */


  testP.currWord = testP.words->head;
  while (testP.errorState != EndERROR) {
    if (strcmp(testP.currWord->key, "0") == 0) {
      i = 0;
      count++;
    }
    switch (count) {
      case 1:
        currentBool = strvarBool[i];
        break;
      case 2:
        currentBool = numvarBool[i];
        break;
      case 3:
        currentBool = strconBool[i];
        break;
      case 4:
        currentBool = numconBool[i];
        break;
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
    i++;
  }

}

void testGrammarFunc(void) {
  Program testP;
  int i, size;
  char printTests[][MAXWORDLEN] = {"\"HELLO world HOW is 3v3rYTh1nG @89\"",
                                   "93075.459", "$A", "%JAZZ", "#URYYB.GKG#",
                                   "%", "$%ABC", "URYYB.GKG#"};
  int printErrorStates[] = {PASS, PASS, PASS, PASS, PASS, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR};

  /* PROG & INSTRS - test prog & opening/closing brackets */
  /* prog -> '{' -> instrs -> '}' */
  initProgram(&testP);
  mvm_insert(testP.words, "0", "{");
  mvm_insert(testP.words, "1", "}");
  prog(&testP);
  assert(testP.errorState == PASS);
  /* Check for no opening bracket */
  mvm_delete(testP.words, "0");
  prog(&testP);
  assert(testP.errorState == StartERROR);
  /* Check empty array */
  testP.errorState = PASS;
  mvm_delete(testP.words, "1");
  assert(mvm_size(testP.words) == 0);
  prog(&testP);
  assert(testP.errorState == StartERROR);
  /* Check for EndERROR - no closing bracket */
  testP.errorState = PASS;
  mvm_insert(testP.words, "0", "{");
  prog(&testP);
  assert(testP.errorState == EndERROR);
  mvm_free(&testP.words);


  /* INSTRUCT FUNCTIONS */
  /* prog -> '{' -> instrs -> INSTRUCT -> instrs -> */

  /* PRINT FUNCTION TESTS*/
  initProgram(&testP);
  mvm_insert(testP.words, "0", "{");
  mvm_insert(testP.words, "1", "PRINT");
  size = sizeof(printTests)/sizeof(printTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    mvm_insert(testP.words, "2", printTests[i]);
    mvm_insert(testP.words, "3", "}");
    prog(&testP);
    assert(testP.errorState == printErrorStates[i]);
    mvm_delete(testP.words, "2");
    mvm_delete(testP.words, "3");
  }
  assert(testP.errorState == SyntaxERROR);
  mvm_free(&testP.words);


}

/* Initialise struct for storage of current program being parsed */
void initProgram(Program *p) {
  p->words = mvm_init();
  p->currWord = NULL;
  p->errorState = PASS;
}

void prog(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  /* Every program starts reading from mvm head */
  if (p->words->head == NULL) {
    p->errorState = StartERROR;
    return;
  }
  p->currWord = p->words->head;

  if (strcmp(p->currWord->data, "{") != 0) {
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
  if (strcmp(p->currWord->data, "}") == 0) {
    return;
  }
  instruct(p);
  nextWord(p);
  instrs(p);
}

void instruct(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  /* Have a check before and after all functions for invalid spelling/syntax? */
  print(p);

}

/* ALL INSTRUCTION FUNCTIONS */

void print(Program *p) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->currWord->data, "PRINT") == 0) {
    nextWord(p);
    varcon(p);
  }
  if (strcmp(p->currWord->data, "PRINTN") == 0) {
    nextWord(p);
    varcon(p);
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
  int wordLength = strlen(p->currWord->data), i = 1;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  if (wordLength > 1) {
    if (p->currWord->data[0] == '$') {
      while (i < wordLength) {
        if (!isupper(p->currWord->data[i])) {
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
  int wordLength = strlen(p->currWord->data), i = 1;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  if (wordLength > 1) {
    if (p->currWord->data[0] == '%') {
      while (i < wordLength) {
        if (!isupper(p->currWord->data[i])) {
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
  int i = 1, length = strlen(p->currWord->data);
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  /* Check if normal string */
  if (p->currWord->data[0] == '"') {
    if (p->currWord->data[length - 1] == '"') {
        return TRUE;
    }
    else {
      p->errorState = SyntaxERROR;
      return FALSE;
    }
  }

  /* Check if ROT18 encoded string */
  if (p->currWord->data[0] == '#') {
    if (p->currWord->data[length - 1] == '#') {
      while (i < length - 1) {
        if (islower(p->currWord->data[i])) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
        i++;
      }
      return TRUE;
    }
    else {
      p->errorState = SyntaxERROR;
      return FALSE;
    }
  }
  return FALSE;
}

/* Can I make it so the default for the function isn't TRUE? */
bool numcon(Program *p) {
  int i = 0, length = strlen(p->currWord->data), count = 0;
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return FALSE;
  }
  while (i < length) {
    if (isdigit(p->currWord->data[i]) || p->currWord->data[i] == '.') {
      if (p->currWord->data[i] == '.') {
        count++;
        if (count > 1 || length == 1) {
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
  if (p->currWord->next != NULL) {
    p->currWord = p->currWord->next;
  }
  else {
    p->errorState = EndERROR;
  }
}

/* StartERROR = No opening bracket, could be empty array
// SyntaxERROR = Invalid syntax - e.g. used lowercase letters/ missing quotation marks
// EndERROR = No valid closing bracket found at end of words list */
