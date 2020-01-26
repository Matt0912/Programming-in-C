#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

/* Want to be able to have resizeable arrays of any data type */
#include "mvm.h"
#include "nal.h"

int main(int argc, char** argv) {
  Program p;
  int maxSize;
  FILE *fp;
  srand(time(NULL));
  #ifdef TEST
  test();
  exit(0);
  #endif

  fp = readFile(argc, argv, &maxSize);
  initProgram(&p, maxSize);
  fillWords(&p, fp);

  #ifdef INTERP
  #ifdef EXT
  runSDL(&p);
  printState(&p);
  mvm_free(&p.vars);
  freeProgram(&p, maxSize);
  fclose(fp);
  return 0;
  #endif
  #endif

  prog(&p);

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

/* Calloc currentword as well, and free everything later */
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

/* Initialise struct for storage of current program being parsed */
void initProgram(Program *p, int maxWords) {
  int i;
  #ifdef EXT
  #ifdef INTERP
  p->SDL = (SDLVars *)calloc(1, sizeof(SDLVars));
  p->SDL->xCoord = 0;
  p->SDL->yCoord = 0;
  #endif
  #endif
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
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "FILE") == 0) {
    nextWord(p);
    if (strcon(p)) {
      #ifdef INTERP
      openNewFile(p);
      #endif
      *checked = TRUE;
      return;
    }
    p->errorState = SyntaxERROR;
  }
}

void openNewFile(Program *p) {
  char *fileStr;
  FILE *fp;
  int maxWords;
  Program newP;
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
  mvm_free(&newP.vars);
  newP.vars = p->vars;
  #ifdef EXT
  newP.SDL->sw = p->SDL->sw;
  #endif
  prog(&newP);
  p->errorState = newP.errorState;
  freeProgram(&newP, maxWords);
  fclose(fp);
  free(fileStr);
}

void abortEX(Program *p, bool *checked) {
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "ABORT") == 0) {
    #ifdef INTERP
    p->errorState = Abort;
    #endif
    *checked = TRUE;
    return;
  }
}

void input(Program *p, bool *checked) {
  char key1[MAXWORDLEN], key2[MAXWORDLEN];
  #ifndef EXT
  #ifdef INTERP
  #ifdef TEST
  char userInput1[MAXWORDLEN], userInput2[MAXWORDLEN];
  #else
  char data1[MAXWORDLEN], data2[MAXWORDLEN];
  char userInput1[MAXWORDLEN], userInput2[MAXWORDLEN];
  #endif
  #endif
  #endif

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
              #ifdef EXT
              waitforInput(p);
              #else
              #ifdef INTERP
              #ifdef TEST
              sprintf(userInput1, "\"TESTING\"");
              sprintf(userInput2, "\"input\"");
              mvm_insert(p->vars, key1, userInput1);
              mvm_insert(p->vars, key2, userInput2);
              #else
              if (scanf("%97s %97s", userInput1, userInput2) != 2) {
                p->errorState = VariableERROR;
                return;
              }
              sprintf(data1, "\"%s\"", userInput1);
              sprintf(data2, "\"%s\"", userInput2);
              mvm_insert(p->vars, key1, data1);
              mvm_insert(p->vars, key2, data2);
              #endif
              #endif
              #endif
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
          #ifndef EXT
          #ifdef INTERP
          #ifdef TEST
          sprintf(userInput1, "36");
          mvm_insert(p->vars, key1, userInput1);
          #else
          if (scanf("%99s", userInput1) != 1) {
            p->errorState = VariableERROR;
            return;
          }
          mvm_insert(p->vars, key1, userInput1);
          #endif
          #endif
          #endif
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
      skipFunction(p);
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
      skipFunction(p);
      *checked = TRUE;
    }
  }
}

/* Skips nested functions if conditions aren't met */
void skipFunction(Program *p) {
  int count = 0;
  nextWord(p);
  if (strcmp(p->words[p->currWord], "{") == 0) {
    count = 1;
    while (count > 0 && p->errorState == PASS) {
      nextWord(p);
      if (strcmp(p->words[p->currWord], "{") == 0) {
        count++;
      }
      if (strcmp(p->words[p->currWord], "}") == 0) {
        count--;
      }
    }
  }
  else {
    p->errorState = StartERROR;
    return;
  }
}

int checkCond(Program *p) {
  #ifdef INTERP
  char *strcon1 = NULL, *strcon2 = NULL;
  float numcon1 = FLOAT_INIT_VAL, numcon2 = FLOAT_INIT_VAL;
  #endif
  int result = 0;
  if (p->errorState != PASS) {
    return Less;
  }
  if (strcmp(p->words[p->currWord], "(") == 0) {
    nextWord(p);
    varcon(p);
    #ifdef INTERP
    if (strcon(p)) {
      strcon1 = formatStrcon(p->words[p->currWord]);
    }
    else if (numcon(p)) {
      numcon1 = strtod(p->words[p->currWord], NULL);
    }
    else if (strvar(p)) {
      strcon1 = returnVars(p);
    }
    else if (numvar(p)) {
      numcon1 = strtod(returnVars(p), NULL);
    }
    #endif
    if (p->errorState == PASS) {
      nextWord(p);
      if (strcmp(p->words[p->currWord], ",") == 0) {
        nextWord(p);
        varcon(p);
        #ifdef INTERP
        if (strcon(p)) {
          strcon2 = formatStrcon(p->words[p->currWord]);
        }
        else if (numcon(p)) {
          numcon2 = strtod(p->words[p->currWord], NULL);
        }
        else if (strvar(p)) {
          strcon2 = returnVars(p);
        }
        else if (numvar(p)) {
          numcon2 = strtod(returnVars(p), NULL);
        }
        #endif
        if (p->errorState == PASS) {
          nextWord(p);
          if (strcmp(p->words[p->currWord], ")") == 0) {
            #ifdef INTERP
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
            #endif
            return result;
          }
        }
      }
    }
  }
  p->errorState = SyntaxERROR;
  return Less;
}

/* Return Greater if first var is greater than, Equal if equal, Less if first
// is smaller, Invalid if invalid comparison */
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
  #ifdef INTERP
  double i;
  char newData[MAXWORDLEN], *oldData;
  #endif
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strcmp(p->words[p->currWord], "INC") == 0) {
    nextWord(p);
    if (strcmp(p->words[p->currWord], "(") == 0) {
      nextWord(p);
      if (numvar(p)) {
        #ifdef INTERP
        if (mvm_size(p->vars) > 0) {
          if (mvm_search(p->vars, p->words[p->currWord]) != NULL) {
            oldData = mvm_search(p->vars, p->words[p->currWord]);
            i = strtod(oldData, NULL);
            i = i + 1;
            if (isPosInteger(oldData)) {
              sprintf(newData, "%.0f", i);
            }
            else {
              sprintf(newData, "%.2f", i);
            }
            mvm_delete(p->vars, p->words[p->currWord]);
            mvm_insert(p->vars, p->words[p->currWord], newData);
            #endif
            nextWord(p);
            if (strcmp(p->words[p->currWord], ")") == 0) {
              *checked = TRUE;
              return;
              #ifdef INTERP
            }
          }
          else {
            p->errorState = VariableERROR;
            return;
          }
          #endif
        }
      }
    }
    p->errorState = SyntaxERROR;
  }
}

/* Use mvm to store variables */
void set(Program *p, bool *checked) {
  char key[MAXWORDLEN];
  #ifdef INTERP
  char *result;
  #endif
  /* Line of code at start of every function */
  if (p->errorState != PASS) {
    return;
  }
  if (strvar(p)) {
    strcpy(key, p->words[p->currWord]);
    nextWord(p);
    if (strcmp(p->words[p->currWord], "=") == 0) {
      nextWord(p);
      #ifdef INTERP
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
      #endif
      if (strcon(p) || strvar(p)) {
        *checked = TRUE;
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
      #ifdef INTERP
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
      #endif
      if (numcon(p) || numvar(p)) {
        *checked = TRUE;
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
      if (!isPosInteger(p->words[p->currWord])) {
        p->errorState = SyntaxERROR;
        return;
      }
      i = strtol(p->words[p->currWord], NULL, 10);
      if (i > p->totalWords) {
        p->errorState = OverflowERROR;
        return;
      }
      #ifdef INTERP
      p->currWord = i-1;
      #endif
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
      #ifdef INTERP
      fprintf(stdout, "\n");
      #endif
      *checked = TRUE;
      return;
    }
    #ifdef INTERP
    printVarCon(p);
    fprintf(stdout, "\n");
    #endif
    *checked = TRUE;
  }

  if (strcmp(p->words[p->currWord], "PRINTN") == 0) {
    nextWord(p);
    varcon(p);
    #ifdef INTERP
    printVarCon(p);
    #endif
    *checked = TRUE;
  }
}

void printVarCon(Program *p) {
  char *output;
  if (strcon(p)) {
    output = formatStrcon(p->words[p->currWord]);
    #ifdef EXT
    SDL_DrawRoom(*p->SDL->sw, p);
    printOutput(*p->SDL->sw, output);
    #else
    fprintf(stdout, "%s", output);
    #endif
    free(output);
  }
  else if (numcon(p)) {
    fprintf(stdout,"%s", p->words[p->currWord]);
  }
  else if (var(p)) {
    if (mvm_size(p->vars) > 0) {
      output = returnVars(p);
      if (output == NULL) {
        p->errorState = VariableERROR;
        return;
      }
      #ifdef EXT
      SDL_DrawRoom(*p->SDL->sw, p);
      printOutput(*p->SDL->sw, output);
      #else
      fprintf(stdout,"%s", output);
      #endif
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
char *returnVars(Program *p) {
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
  #ifdef INTERP
  char newData[MAXWORDLEN];
  #endif
  char key[MAXWORDLEN];
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
          #ifdef INTERP
          sprintf(newData, "%d", rand() % 100);
          mvm_insert(p->vars, key, newData);
          #endif
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
        /* If only a '.', syntax error */
        if (count1 > 1 || length == 1) {
          p->errorState = SyntaxERROR;
          return FALSE;
        }
      }
      if (p->words[p->currWord][i] == '-') {
        count2++;
        /* If only a '-', syntax error */
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
  (p->currWord)++;
  if (p->words[p->currWord][0] == '\0' && p->errorState != Abort) {
    p->errorState = EndERROR;
  }
}

/* Returns true for positive integers */
bool isPosInteger(char *number) {
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

void freeProgram(Program *p, int maxWords) {
  int i;
  for (i = 0; i < maxWords; i++) {
    free(p->words[i]);
  }
  free(p->words);
}

void printState(Program *p) {
  switch (p->errorState) {
    case PASS:
      #ifdef INTERP
      fprintf(stderr, "Interpreted OK\n");
      #else
      fprintf(stderr, "Parsed OK\n");
      #endif
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
    case Abort:
      fprintf(stderr, "Interpreted OK (ABORT called)\n");
      break;
  }
}

/**************************** EXTENSION FUNCTIONS ****************************/

#ifdef EXT
#ifdef INTERP
void runSDL(Program *p) {
  SDL_Simplewin sw;

  Neill_SDL_Init(&sw);
  p->SDL->sw = &sw;

  do {
    SDL_DrawRoom(sw, p);
    keyboard(&sw, p);

    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_UpdateScreen(&sw);
    if (p->currWord == 0) {
      prog(p);
    }

  }while(!sw.finished);

  SDL_Quit();
  atexit(SDL_Quit);
}

void SDL_DrawRoom(SDL_Simplewin sw, Program *p) {
  SDL_Rect rectangle;
  int x, y;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  char *escString1 = "PRESS ESC";
  char *escString2 = " TO EXIT ";

  Neill_SDL_ReadFont(fontdata, "mode7.fnt");
  /* Reset room to black */
  Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
  rectangle.x = 0;
  rectangle.y = 0;
  rectangle.w = WWIDTH;
  rectangle.h = WHEIGHT;
  SDL_RenderFillRect(sw.renderer, &rectangle);
  /* Draw white room outline and text box*/
  Neill_SDL_SetDrawColour(&sw, 192,192,192);
  rectangle.x = 0;
  rectangle.y = 0;
  rectangle.w = ROOMSIZE;
  rectangle.h = ROOMSIZE;
  SDL_RenderDrawRect(sw.renderer, &rectangle);
  rectangle.y = ROOMSIZE * 3/4;
  rectangle.h = ROOMSIZE * 1/4;
  SDL_RenderDrawRect(sw.renderer, &rectangle);
  /* Draw compass */
  Neill_SDL_SetDrawColour(&sw, 192,192,192);
  rectangle.w = 5;
  rectangle.h = 125;
  rectangle.x = WWIDTH - 107;
  rectangle.y = 50;
  SDL_RenderFillRect(sw.renderer, &rectangle);
  Neill_SDL_DrawChar(&sw, fontdata, 'N', rectangle.x - 5, rectangle.y - 20);
  Neill_SDL_DrawChar(&sw, fontdata, 'S', rectangle.x - 5, rectangle.y + 135);
  Neill_SDL_SetDrawColour(&sw, 192,192,192);
  rectangle.w = 125;
  rectangle.h = 5;
  rectangle.x = WWIDTH - 165;
  rectangle.y = 112;
  SDL_RenderFillRect(sw.renderer, &rectangle);
  Neill_SDL_DrawChar(&sw, fontdata, 'E', rectangle.x + 130, rectangle.y - 2);
  Neill_SDL_DrawChar(&sw, fontdata, 'W', rectangle.x - 20, rectangle.y - 2);
  /* Write instructions */
  Neill_SDL_DrawString(&sw, fontdata, "W = UP", ROOMSIZE, ROOMSIZE/2);
  Neill_SDL_DrawString(&sw, fontdata, "A = LEFT", ROOMSIZE, ROOMSIZE/2 + 30);
  Neill_SDL_DrawString(&sw, fontdata, "S = DOWN", ROOMSIZE, ROOMSIZE/2 + 60);
  Neill_SDL_DrawString(&sw, fontdata, "D = RIGHT", ROOMSIZE, ROOMSIZE/2 + 90);
  Neill_SDL_DrawString(&sw, fontdata, "T = INTERACT", ROOMSIZE-5, ROOMSIZE/2 + 140);
  Neill_SDL_DrawString(&sw, fontdata, escString1, ROOMSIZE, ROOMSIZE - 60);
  Neill_SDL_DrawString(&sw, fontdata, escString2, ROOMSIZE, ROOMSIZE - 20);
  /* Draw character */
  Neill_SDL_SetDrawColour(&sw, 224, 224, 224);
  x = (ROOMSIZE*(p->SDL->xCoord))/6 + 50;
  y = ROOMSIZE - ROOMSIZE/4 - (ROOMSIZE*p->SDL->yCoord)/6 - 100;
  Neill_SDL_RenderFillCircle(sw.renderer, x, y, 25);

  Neill_SDL_UpdateScreen(&sw);
}

void keyboard(SDL_Simplewin *sw, Program *p) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
          if (event.key.keysym.sym==SDLK_ESCAPE) {
            sw->finished = 1;
            SDL_Quit();
            break;
          }
          if (event.key.keysym.sym==SDLK_w) {
            if (p->SDL->yCoord < 3) {
              p->SDL->yCoord += 1;
            }
            break;
          }
          if (event.key.keysym.sym==SDLK_d) {
            if (p->SDL->xCoord < 5) {
              p->SDL->xCoord += 1;
            }
            break;
          }
          if (event.key.keysym.sym==SDLK_s) {
            if (p->SDL->yCoord > 0) {
              p->SDL->yCoord -= 1;
            }
            break;
          }
          if (event.key.keysym.sym==SDLK_a) {
            if (p->SDL->xCoord > 0) {
              p->SDL->xCoord -= 1;
            }
            break;
          }
          if (event.key.keysym.sym==SDLK_t) {
            p->SDL->interact = TRUE;
            break;
          }
      default:
          break;
    }
  }
}

void printOutput(SDL_Simplewin sw, char *text) {
  char output[LINESIZE];
  int i = 0, j = 0, length = strlen(text), offset = 0;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  Neill_SDL_ReadFont(fontdata, "mode7.fnt");

  if (length > LINESIZE) {
    for (i = 0; i < length; i++) {
      output[j] = text[i];
      j++;
      if (j == LINESIZE - 1 || i == length - 1) {
        output[j] = '\0';
        Neill_SDL_DrawString(&sw, fontdata, output, 5, ROOMSIZE*3/4 + 20 + offset);
        Neill_SDL_UpdateScreen(&sw);
        offset += 20;
        j = 0;
      }
    }
  }
  else {
    Neill_SDL_DrawString(&sw, fontdata, text, 5, ROOMSIZE*3/4 + 20);
  }
  Neill_SDL_UpdateScreen(&sw);
  SDL_Delay(MILLISECONDDELAY * 1000);
}

void waitforInput(Program *p) {
  int oldX = p->SDL->xCoord, oldY = p->SDL->yCoord;
  int coordSum = oldX + oldY;
  while (p->SDL->xCoord + p->SDL->yCoord == coordSum && p->SDL->interact == FALSE) {
    SDL_Delay(MILLISECONDDELAY);
    keyboard(p->SDL->sw, p);
    SDL_DrawRoom(*p->SDL->sw, p);
  }
  if (p->SDL->interact == TRUE) {
    if (p->SDL->xCoord == 1 && p->SDL->yCoord == 0) {
      mvm_insert(p->vars, "$A", "\"TOUCH\"");
      mvm_insert(p->vars, "$B", "\"BIKE\"");
      p->SDL->interact = FALSE;
    }
    else {
      SDL_Quit();
    }
  }
  else {
    mvm_insert(p->vars, "$A", "\"MOVE\"");
    if (p->SDL->xCoord > oldX) {
      mvm_insert(p->vars, "$B", "\"EAST\"");
      printf("%d, %d\n", p->SDL->xCoord, p->SDL->yCoord);
    }
    if (p->SDL->xCoord < oldX) {
      mvm_insert(p->vars, "$B", "\"WEST\"");
    }
    if (p->SDL->yCoord > oldY) {
      mvm_insert(p->vars, "$B", "\"NORTH\"");
    }
    if (p->SDL->yCoord < oldY) {
      mvm_insert(p->vars, "$B", "\"SOUTH\"");
    }
  }
}
#endif
#endif

/***************************** ALL TESTING FUNCTIONS ************************/
void test(void) {

  #ifdef INTERP
  printf(" INITIALISING INTERPRETER TESTS ... \n");
  #else
  printf(" INITIALISING PARSER TESTS ... \n");
  #endif

  /* TEST FORMAL GRAMMAR FUNCTIONS - DIFFERENT FOR PARSE & INTERP */
  #ifdef INTERP
  interpTestFunc();
  #else
  /* TEST BASIC FUNCTIONS */
  basicFuncTest();
  /* TEST VARIABLE/CONSTANT FUNCTIONS */
  testVarCon();
  parseTestFunc();
  #endif

  printf(" TESTING COMPLETE \n");

}

void basicFuncTest(void) {
  Program testP;
  int i, size, maxWords;
  char testString[MAXWORDLEN];
  char *ROTStrings[] = {"URYYB", "ABCDEFG", "JKLM", "hello",
                        "While9", "123", "09812jkl"};
  char *decodedStrings[] = {"HELLO", "NOPQRST", "WXYZ", "uryyb",
                            "Juvyr4", "678", "54367wxy"};
  FILE *fp;
  char *argv[] = {"./parse", "ReadfileTesting.txt"};

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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* TEST ROTDECODE FUNCTION */
  size = sizeof(ROTStrings)/sizeof(ROTStrings[0]);
  for (i = 0; i < size; i++) {
    strcpy(testString, ROTStrings[i]);
    ROTDecode(testString);
    assert(strcmp(testString, decodedStrings[i]) == 0);
  }

  /* TEST ISPOSINTEGER */
  assert(isPosInteger("12") == TRUE);
  assert(isPosInteger("120019092") == TRUE);
  assert(isPosInteger("0") == TRUE);
  assert(isPosInteger("1.2") == FALSE);
  assert(isPosInteger("12.091") == FALSE);
  assert(isPosInteger("-1") == FALSE);
  assert(isPosInteger("-12.091") == FALSE);

  /* TEST READFILE */
  /* If statement checks for if filename already exists to prevent overwriting*/
  /* If it does already exist, TESTS WILL NOT PASS */
  if ((fp = fopen(argv[1], "r")) == NULL) {
    fp = fopen(argv[1], "w");
    fprintf(fp, "{ "
                "PRINT \"Hello World!\" "
                "%%A = 13.452 "
                "$K = #ROT18 String should be one word#"
                " }");
    fclose(fp);
  }
  fp = readFile(2, argv, &maxWords);
  assert(maxWords == 75);

  /* TEST FILLWORDS FUNCTIONS */
  initProgram(&testP, maxWords);
  fillWords(&testP, fp);
  assert(strcmp(testP.words[0], "{") == 0);
  assert(strcmp(testP.words[1], "PRINT") == 0);
  assert(strcmp(testP.words[2], "\"Hello World!\"") == 0);
  assert(strcmp(testP.words[3], "%A") == 0);
  assert(strcmp(testP.words[5], "13.452") == 0);
  assert(strcmp(testP.words[8], "#ROT18 String should be one word#") == 0);
  assert(strcmp(testP.words[9], "}") == 0);
  assert(testP.totalWords == 10);

  mvm_free(&testP.vars);
  freeProgram(&testP, maxWords);
  remove(argv[1]);
  fclose(fp);
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

}

void parseTestFunc(void) {
  Program testP;
  int i, size;

  char fileTests[][MAXWORDLEN] = {"\"WOW.txt\"", "\"file1.nal\"",
                                  "#WOW.TXT#", "##"};
  int fileErrorStates[] = {PASS, PASS, PASS, SyntaxERROR};

  char inputStrTests1[][MAXWORDLEN] = {"$C", "$STRVAR", "VAR1"};
  char inputStrTests2[][MAXWORDLEN] = {"$STRVAR", "$TEST", "VAR2"};
  int inputStrErrorStates[] = {PASS, PASS, SyntaxERROR};
  char inputNumTests[][MAXWORDLEN] = {"%VAR", "TEST"};
  int inputNumErrorStates[] = {PASS, SyntaxERROR};

  char ifcondTests[][MAXWORDLEN] = {"%D", "$ABC", "\"Hello\"", "19.07",
                                    "#YUI#", "\"hey"};
  int ifcondErrorStates[] = {PASS, PASS, PASS, PASS, PASS, StartERROR};

  char incTests[][MAXWORDLEN] = {"%ABC", "%T", "%C ", "%", "$ABC",
                                 "93", ".%K", "%R1"};
  int incErrorStates[] = {PASS, PASS, SyntaxERROR, SyntaxERROR, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR, SyntaxERROR};

  char setTestsSTR1[][MAXWORDLEN] = {"$A", "$STRVAR", "$HI=", "$TEST"};
  char setTestsSTR2[][MAXWORDLEN] = {"\"hi\"", "$A", "$HI", "%ABC"};
  char setTestsNUM1[][MAXWORDLEN] = {"%ABC", "%KP", "%HI=", "%FIN"};
  char setTestsNUM2[][MAXWORDLEN] = {"17.3", "%ABC", "1.3", "$TEST"};
  int setErrorStates[] = {PASS, PASS, SyntaxERROR, SyntaxERROR};


  char jumpTests[][MAXWORDLEN] = {"5", "49", "0", "-1", "-5.5", "10.2", "19.0",
                                  "51", "152"};
  int jumpErrorStates[] = {PASS, PASS, PASS, SyntaxERROR, SyntaxERROR,
                           SyntaxERROR, SyntaxERROR, OverflowERROR, OverflowERROR};

  char printTests[][MAXWORDLEN] = {"\"HELLO HOW is 3v3rYTh1nG @89\"",
                                   "93075.459", "$A", "%JAZZ", "#URYYB.GKG#",
                                   "%", "$%ABC", "URYYB.GKG#", "97hello32"};
  int printErrorStates[] = {PASS, PASS, PASS, PASS, PASS, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR, SyntaxERROR};

  char rndTests[][MAXWORDLEN] = {"(", "%E", ")",
                                 "(", "%NUMBERVAR", ")",
                                 "(", "%", ")",
                                 "(", "$P", ")",
                                 "(", "%E", "()",
                                 "[", "%E", ")"};
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* ABORT FUNCTION TEST */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "ABORT");
  strcpy(testP.words[2], "}");
  prog(&testP);
  assert(testP.errorState == PASS);
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* INPUT FUNCTION TESTS */
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* IFCOND FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "IFEQUAL");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ",");
  strcpy(testP.words[6], ")");
  strcpy(testP.words[7], "{");
  strcpy(testP.words[8], "ABORT");
  strcpy(testP.words[9], "}");
  strcpy(testP.words[10], "}");
  size = sizeof(ifcondTests)/sizeof(ifcondTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests[i]);
    strcpy(testP.words[5], ifcondTests[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
  }
  strcpy(testP.words[1], "IFGREATER");
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests[i]);
    strcpy(testP.words[5], ifcondTests[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* INC FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "INC");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ")");
  strcpy(testP.words[5], "}");
  size = sizeof(incTests)/sizeof(incTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], incTests[i]);
    prog(&testP);
    assert(testP.errorState == incErrorStates[i]);
  }
  mvm_free(&testP.vars);
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
  }
  strcpy(testP.words[2], "!=");
  strcpy(testP.words[1], setTestsSTR1[0]);
  strcpy(testP.words[3], setTestsSTR2[0]);
  prog(&testP);
  assert(testP.errorState == SyntaxERROR);
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* JUMP FUNCTION TESTS*/
  initProgram(&testP, MAXWORDS);
  testP.totalWords = 50;
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "JUMP");
  strcpy(testP.words[3], "}");
  size = sizeof(jumpTests)/sizeof(jumpTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[2], jumpTests[i]);
    prog(&testP);
    assert(testP.errorState == jumpErrorStates[i]);
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* PRINT FUNCTION TESTS*/
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
  }
  assert(testP.errorState == SyntaxERROR);
  mvm_free(&testP.vars);
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
  }
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

}

void interpTestFunc(void) {
  Program testP;
  int i, size;
  float num;
  char *testStr;
  FILE *fp;

  /******************************* INPUT ***********************************/
  char inputStrTests1[][MAXWORDLEN] = {"$C", "$STRVAR", "$VA", "VAR1"};
  char inputStrTests2[][MAXWORDLEN] = {"$STRVAR", "$TEST", "$VB", "VAR2"};
  int inputStrErrorStates[] = {PASS, PASS, PASS, SyntaxERROR};
  char inputNumTests[][MAXWORDLEN] = {"%VAR", "%VA", "%VB", "TEST"};
  int inputNumErrorStates[] = {PASS, PASS, PASS, SyntaxERROR};

  /******************************* IFCOND ***********************************/
  char ifcondTests1[][MAXWORDLEN] = {"\"testing\"", "$ABC", "%D", "19.07", "#YUI#",
                                    "\"hey", "15.3", "\"NINE\""};
  char ifcondTests2[][MAXWORDLEN] = {"$ABC", "\"Hello\"", "19.07", "%D", "$ABC",
                                    "\"hey", "\"NINE\"", "15.3"};
  int ifcondErrorStates[] = {PASS, PASS, PASS, PASS, PASS, StartERROR,
                             CompERROR, CompERROR};

  /******************************* COMPCONS **********************************/
  /* $ABC = "testing", %D = 900.4 */
  int checkCondResults[] = {Equal, Greater, Greater, Less, Less, Less,
                            Invalid, Invalid};

  /******************************* COMPCONS **********************************/
  char *strcon1[MAXWORDLEN] = {"More", "Less", "Greater", "Equal",
                                "Tie", NULL, NULL, NULL, NULL, "Fifteen", NULL};
  char *strcon2[MAXWORDLEN] = {"Less", "More", "Great", "Equal",
                                "Tezakdsjf", NULL, NULL, NULL, NULL,
                                NULL, "1"};
  float numcon1[] = {FLOAT_INIT_VAL, FLOAT_INIT_VAL, FLOAT_INIT_VAL,
                     FLOAT_INIT_VAL, FLOAT_INIT_VAL, 16.7, 10.00, 5, 1,
                     FLOAT_INIT_VAL, 1};
  float numcon2[] = {FLOAT_INIT_VAL, FLOAT_INIT_VAL, FLOAT_INIT_VAL,
                     FLOAT_INIT_VAL, FLOAT_INIT_VAL, 13, -100, 5.0000,
                     1.2, 15, FLOAT_INIT_VAL};
  int compConsResults[] = {Greater, Less, Greater, Equal, Greater, Greater,
                           Greater, Equal, Less, Invalid, Invalid};

  /********************************* INC ***********************************/
  char incNums[][MAXWORDLEN] = {"15", "432.1", "-10", "67.0", "23", "0"};
  char incNumsResults[][MAXWORDLEN] = {"16", "433.10", "-9.00", "68.00", "24", "1"};
  char incTests[][MAXWORDLEN] = {"%A", "%C ", "%", "$ABC", "93", ".%K", "%R1"};
  int incErrorStates[] = {PASS, SyntaxERROR, SyntaxERROR, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR, SyntaxERROR};

  /********************************* SET ***********************************/
  /* All 5 arrays should be same size */
  char setTestsSTR1[][MAXWORDLEN] = {"$A", "$STRVAR", "$D", "$D", "$ROT",
                                     "$TEST", "$VAR", "$HI=", "$TEST"};
  char setTestsSTR2[][MAXWORDLEN] = {"\"hi\"", "$A", "\"A A a 8\"", "\"A2\"",
                                     "#URYYB#", "$VAR", "$VAR","$HI", "%ABC"};
  char setTestsSTR3[][MAXWORDLEN] = {"\"hi\"", "\"hi\"", "\"A A a 8\"","\"A2\"",
                                     "#URYYB#", "$VAR", "$VAR","$HI", "%ABC"};
  char setTestsNUM1[][MAXWORDLEN] = {"%ABC", "%KP", "%D", "%D", "%NUM", "%NUM",
                                     "%TEST","%HI=", "%FIN"};
  char setTestsNUM2[][MAXWORDLEN] = {"17.3", "%ABC", "-100.5", "50", "%D",
                                     "%TEST", "%TEST", "1.3", "$TEST"};
  char setTestsNUM3[][MAXWORDLEN] = {"17.3", "17.3", "-100.5", "50", "50",
                                     "%TEST", "%TEST", "1.3", "$TEST"};
  int setErrorStates[] = {PASS, PASS, PASS, PASS, PASS, VariableERROR,
                          VariableERROR, SyntaxERROR, SyntaxERROR};
  /********************************* RETURNVARS *******************************/
  /* Use settests to test returnVars */
  char returnVarsOutputs1[][MAXWORDLEN] = {"hi", "hi", "A A a 8", "A2", "HELLO"};
  char returnVarsOutputs2[][MAXWORDLEN] = {"17.3", "17.3", "-100.5", "50", "50"};

  /********************************* JUMP *******************************/
  char jumpTests[][MAXWORDLEN] = {"6", "10", "-1", "-5.5", "10.2", "19.0",
                                  "51", "152"};
  int jumpErrorStates[] = {PASS, PASS, SyntaxERROR, SyntaxERROR, SyntaxERROR,
                           SyntaxERROR, OverflowERROR, OverflowERROR};

  /***************************** FORMATSTRCON *******************************/
  char formatStrconTests[][MAXWORDLEN] = {"\"HELLO HOW is 3v3rYTh1nG @89\"",
                                    "#123#", "#URYYB.GKG#", "\"97hello32\"",
                                    "#HELLO.TXT#"};
  char formatStrconOutputs[][MAXWORDLEN] = {"HELLO HOW is 3v3rYTh1nG @89",
                                            "678", "HELLO.TXT", "97hello32",
                                           "URYYB.GKG"};

  /********************************* RND *********************************/
  char rndTests[][MAXWORDLEN] = {"(", "%E", ")",
                                 "(", "%NUMBERVAR", ")",
                                 "(", "%", ")",
                                 "(", "$P", ")",
                                 "(", "%E", "()",
                                 "[", "%HH", ")"};
  int rndErrorStates[] = {PASS, PASS, SyntaxERROR, SyntaxERROR,
                            SyntaxERROR, SyntaxERROR};

  /**************************** FUNCTIONERROR *********************************/
  /* All cases should cause function error */
  char functionErrorTests[][MAXWORDLEN] = {"jUMP", "PRINT ", "inc",
                                           "Set", "AB0RT"};

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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* INSTRUCT FUNCTIONS */
  /* prog -> '{' -> instrs -> INSTRUCT -> instrs -> */

  /* FILE FUNCTION TESTS */
  /* If these filenames already exist, if statement stops from overwriting */
  /* If this happens, TESTS WILL FAIL */
  if ((fp = fopen("FILETests1.txt", "r")) == NULL) {
    fp = fopen("FILETests1.txt", "w");
    fprintf(fp, "{ FILE \"FILETests2.txt\" }");
    fclose(fp);
  }
  if ((fp = fopen("FILETests2.txt", "r")) == NULL) {
    fp = fopen("FILETests2.txt", "w");
    fprintf(fp, "{ FILE #SVYRGrfgf8.gkg# }");
    fclose(fp);
  }
  if ((fp = fopen("FILETests3.txt", "r")) == NULL) {
    fp = fopen("FILETests3.txt", "w");
    fprintf(fp, "{ %%A = 5.6 ABORT }");
    fclose(fp);
  }

  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "FILE");
  strcpy(testP.words[2], "\"FILETests1.txt\"");
  strcpy(testP.words[3], "}");
  prog(&testP);
  assert(mvm_size(testP.vars) > 0);
  assert(strcmp(mvm_search(testP.vars, "%A"), "5.6") == 0);
  assert(testP.errorState == Abort);
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);
  remove("FILETests1.txt");
  remove("FILETests2.txt");
  remove("FILETests3.txt");

  /* ABORT FUNCTION TEST */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "ABORT");
  strcpy(testP.words[2], "}");
  prog(&testP);
  assert(testP.errorState == Abort);
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* INPUT FUNCTION TESTS */
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
    if (testP.errorState == PASS) {
      assert(strcmp(mvm_search(testP.vars, inputStrTests1[i]),
             "\"TESTING\"") == 0);
      assert(strcmp(mvm_search(testP.vars, inputStrTests2[i]),
             "\"input\"") == 0);
    }
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
    if (testP.errorState == PASS) {
      assert(strcmp(mvm_search(testP.vars, inputNumTests[i]), "36") == 0);
    }
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* IFCOND FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "IFEQUAL");
  strcpy(testP.words[2], "(");
  strcpy(testP.words[4], ",");
  strcpy(testP.words[6], ")");
  strcpy(testP.words[7], "{");
  strcpy(testP.words[8], "%A");
  strcpy(testP.words[9], "=");
  strcpy(testP.words[10], "3");
  strcpy(testP.words[11], "}");
  strcpy(testP.words[12], "}");
  mvm_insert(testP.vars, "$ABC", "\"testing\"");
  mvm_insert(testP.vars, "%D", "900.4");
  size = sizeof(ifcondTests1)/sizeof(ifcondTests1[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests1[i]);
    strcpy(testP.words[5], ifcondTests2[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
    /* Test skipFunction functionality - if condition isn't met, skip code */
    if (checkCondResults[i] == Equal) {
      assert(mvm_size(testP.vars) > 2);
      assert(strcmp(mvm_search(testP.vars, "%A"), "3") == 0);
      mvm_delete(testP.vars, "%A");
    }
    else {
      assert(mvm_size(testP.vars) == 2);
    }
  }
  strcpy(testP.words[1], "IFGREATER");
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[3], ifcondTests1[i]);
    strcpy(testP.words[5], ifcondTests2[i]);
    prog(&testP);
    assert(testP.errorState == ifcondErrorStates[i]);
    /* Test skipFunction functionality - if condition isn't met, skip code */
    if (checkCondResults[i] == Greater) {
      assert(mvm_size(testP.vars) > 2);
      assert(strcmp(mvm_search(testP.vars, "%A"), "3") == 0);
      mvm_delete(testP.vars, "%A");
    }
    else {
      assert(mvm_size(testP.vars) == 2);
    }
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);


  /* CHECKCOND FUNCTION TESTS */
  initProgram(&testP, MAXWORDS);
  strcpy(testP.words[0], "(");
  strcpy(testP.words[2], ",");
  strcpy(testP.words[4], ")");
  mvm_insert(testP.vars, "$ABC", "\"testing\"");
  mvm_insert(testP.vars, "%D", "900.4");
  size = sizeof(ifcondTests1)/sizeof(ifcondTests1[0]);
  for (i = 0; i < size; i++) {
    testP.currWord = 0;
    testP.errorState = PASS;
    strcpy(testP.words[1], ifcondTests1[i]);
    strcpy(testP.words[3], ifcondTests2[i]);
    assert(checkCond(&testP) == checkCondResults[i]);
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /* COMPCONS FUNCTION TESTS */
  size = sizeof(compConsResults)/sizeof(compConsResults[0]);
  for (i = 0; i < size; i++) {
    assert(compCons(strcon1[i], numcon1[i], strcon2[i], numcon2[i])
           == compConsResults[i]);
  }

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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);


  /* JUMP FUNCTION TESTS*/
  initProgram(&testP, MAXWORDS);
  testP.totalWords = 13;
  strcpy(testP.words[0], "{");
  strcpy(testP.words[1], "JUMP");
  strcpy(testP.words[3], "$A");
  strcpy(testP.words[4], "=");
  strcpy(testP.words[5], "#jump#");
  strcpy(testP.words[6], "$A");
  strcpy(testP.words[7], "=");
  strcpy(testP.words[8], "#JUMP#");
  strcpy(testP.words[9], "}");
  strcpy(testP.words[10], "$A");
  strcpy(testP.words[11], "=");
  strcpy(testP.words[12], "\"hello\"");
  strcpy(testP.words[13], "}");
  size = sizeof(jumpTests)/sizeof(jumpTests[0]);
  for (i = 0; i < size; i++) {
    testP.errorState = PASS;
    strcpy(testP.words[2], jumpTests[i]);
    prog(&testP);
    assert(testP.errorState == jumpErrorStates[i]);
    if (strcmp(jumpTests[i], "6") == 0) {
      assert(strcmp(mvm_search(testP.vars, "$A"), "#JUMP#") == 0);
    }
    if (strcmp(jumpTests[i], "10") == 0) {
      assert(strcmp(mvm_search(testP.vars, "$A"), "\"hello\"") == 0);
    }
  }
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

  /*FORMATSTRCON TESTS */
  size = sizeof(formatStrconTests)/sizeof(formatStrconTests[0]);
  for (i = 0; i < size; i++) {
    testStr = formatStrcon(formatStrconTests[i]);
    assert(strcmp(testStr, formatStrconOutputs[i]) == 0);
    free(testStr);
  }


  /*returnVars TESTS */
  initProgram(&testP, MAXWORDS);
  size = sizeof(setTestsSTR1)/sizeof(setTestsSTR1[0]);
  for (i = 0; i < size; i++) {
    if (setErrorStates[i] == PASS) {
      /* String tests */
      mvm_insert(testP.vars, setTestsSTR1[i], setTestsSTR3[i]);
      strcpy(testP.words[i], setTestsSTR1[i]);
      testP.currWord = i;
      testStr = returnVars(&testP);
      assert(strcmp(testStr, returnVarsOutputs1[i]) == 0);
      free(testStr);


      /* Numvar Tests */
      mvm_insert(testP.vars, setTestsNUM1[i], setTestsNUM3[i]);
      strcpy(testP.words[i], setTestsNUM1[i]);
      testStr = returnVars(&testP);
      assert(strcmp(testStr, returnVarsOutputs2[i]) == 0);
    }
  }
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
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
  mvm_free(&testP.vars);
  freeProgram(&testP, MAXWORDS);

}
