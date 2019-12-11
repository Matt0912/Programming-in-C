#include <stdio.h>
#include <stdlib.h>
#define NDEBUG
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "mvm.h"

#define MAXSTRSIZE 200
#define MAXOUTPUTSTR 1000
#define FILENAME "cmudict.txt"

enum bool {FALSE, TRUE};

/* Write tests for checkinput, capitaliseString, printRhymes */
/* Test more edge cases */
int checkInput(int argc, char **argv, int *numPhonemes, int *i);
void readFile(mvm* map1, mvm* map2, int numPhonemes);
int hashIndex(char* string);
int returnPhonemes(char* string, int numPhonemes, char* phonemes);
char *printRhymes(char **rhymes, int count);
void strrev(char *string);
void capitaliseString(char *string);
void test(void);

int main(int argc, char **argv) {
  mvm *map1, *map2;
  char** rhymes, *phonemes, *output;
  int numPhonemes = 0, count, i;
  test();

  if (checkInput(argc, argv, &numPhonemes, &i) == FALSE) {
    fprintf(stderr, "INVALID INPUT - TRY: ./homophones -n 3 'WORD'\n");
    exit(1);
  }

  map1 = mvm_init();
  map2 = mvm_init();
  readFile(map1, map2, numPhonemes);

  while (i < argc) {
    count = 0;
    if ((phonemes = mvm_search(map1, argv[i])) == NULL) {
        fprintf(stderr, "\n%s not found - please try again\n", argv[i]);
        exit(1);
    }
    rhymes = mvm_multisearch(map2, phonemes, &count);
    output = printRhymes(rhymes, count);
    fprintf(stdout, "\n%s (%s): RHYMES = %d\n%s\n", argv[i], phonemes,
            count, output);
    i++;
  }

  free(rhymes);
  free(output);
  mvm_free(&map1);
  mvm_free(&map2);
  return FALSE;
}

/* Check input is valid and in correct format
// Also handle 2 cases where -n is specified or not */
int checkInput(int argc, char **argv, int *numPhonemes, int *i) {
  int x, j;
  if (argc == 1) {
    return FALSE;
  }
  /* If no -n flag given, default to 3 phonemes */
  if (strcmp(argv[1], "-n") != 0) {
    fprintf(stderr, "-n NOT SPECIFIED - DEFAULTED TO 3 PHONEMES\n");
    *numPhonemes = 3;
    *i = 1;
  }
  /* Else extract phoneme number */
  else {
    x = (int)strtol(argv[2],(char **)NULL, 10);
    if (x <= 0) {
      return FALSE;
    }
    *numPhonemes = x;
    *i = 3;
  }
  j = *i;
  while (j < argc) {
    capitaliseString(argv[j]);
    j++;
  }
  return TRUE;
}

void test(void) {
  char testStr1[] = "STANO#S T AA1 N OW0";
  char testStr2[] = "FURGERSON#F ER1 G ER0 S AH0 N";
  /*
  char testStr3[] = "STEELED#S T IY1 L D";
  char testStr4[] = "STEELEDS T IY1 L D";
  char phonemes[MAXSTRSIZE];*/
  char testStr5[] = "testing";
  char testStr6[] = "stano#s t aa1 n ow0";

  mvm* testmap;

  /* Test capitaliseString */
  capitaliseString(testStr1);
  assert(strcmp(testStr1, "STANO#S T AA1 N OW0") == 0);
  capitaliseString(testStr5);
  assert(strcmp(testStr5, "TESTING") == 0);
  capitaliseString(testStr6);
  assert(strcmp(testStr6, "STANO#S T AA1 N OW0") == 0);


  /* Test strrev */
  strrev(testStr1);
  assert(strcmp(testStr1, "0WO N 1AA T S#ONATS") == 0);
  strrev(testStr1);
  assert(strcmp(testStr1, "STANO#S T AA1 N OW0") == 0);
  strrev(testStr2);
  assert(strcmp(testStr2, "N 0HA S 0RE G 1RE F#NOSREGRUF") == 0);
  strrev(testStr2);
  assert(strcmp(testStr2, "FURGERSON#F ER1 G ER0 S AH0 N") == 0);

  /* Test hashIndex */
  assert(hashIndex(testStr1) == 5);
  assert(hashIndex(testStr2) == 9);
  assert(hashIndex(testStr3) == 7);
  assert(hashIndex(testStr4) == FALSE);
  assert(hashIndex(" ") == FALSE);
  assert(hashIndex("BOY#B OY1") == 3);
  assert(hashIndex("STEE#S T IY1 L D") == 4);

  /* Test returnPhonemes */
  assert(returnPhonemes(testStr1, 2, phonemes) == TRUE);
  assert(strcmp(phonemes, "N OW0") == 0);
  assert(returnPhonemes(testStr1, 3, phonemes) == TRUE);
  assert(strcmp(phonemes, "AA1 N OW0") == 0);
  assert(returnPhonemes(testStr1, 5, phonemes) == TRUE);
  assert(strcmp(phonemes, "S T AA1 N OW0") == 0);
  assert(returnPhonemes(testStr1, 6, phonemes) == FALSE);
  assert(returnPhonemes(testStr2, 7, phonemes) == TRUE);
  assert(strcmp(phonemes, "F ER1 G ER0 S AH0 N") == 0);
  assert(returnPhonemes(testStr2, 8, phonemes) == FALSE);
  assert(returnPhonemes(testStr3, 1, phonemes) == TRUE);
  assert(strcmp(phonemes, "D") == 0);
  assert(returnPhonemes(testStr3, 3, phonemes) == TRUE);
  assert(strcmp(phonemes, "IY1 L D") == 0);

  testmap = mvm_init();

  mvm_free(&testmap);

}

void readFile(mvm* map1, mvm* map2, int numPhonemes) {
  char str[MAXSTRSIZE], key[MAXSTRSIZE], data[MAXSTRSIZE];
  FILE *fp;
  int hash;

  fp = fopen(FILENAME, "r");

  if (fp == NULL) {
    fprintf(stderr, "ERROR - Unable to open dictionary file\n");
    exit(1);
  }

  while (fgets(str, MAXSTRSIZE+1, fp) != NULL) {
    hash = hashIndex(str);
    strcpy(key, str);
    key[hash] = '\0';
    returnPhonemes(str, numPhonemes, data);
    mvm_insert(map1, key, data);
    mvm_insert(map2, data, key);
  }

  fclose(fp);
}

/* Returns string from list of pointers to words that rhyme with input */
char *printRhymes(char **rhymes, int count) {
  char *output = (char *)calloc(1, MAXOUTPUTSTR*sizeof(char));
  int i = 0, j = 1;
  while (i < count) {
    if (strlen(output) > MAXOUTPUTSTR*0.8*j) {
      j++;
      output = (char*)realloc(output, sizeof(char)*MAXOUTPUTSTR*j);
    }
    strcat(output, rhymes[i]);
    strcat(output, " ");
    i++;
  }

  return output;
}

/* Return the index of the '#' in a string*/
int hashIndex(char* string) {
  int i = 0;
  while (string[i]) {
    if (string[i] == '#') {
      return i;
    }
    i++;
  }
  return FALSE;
}

/* Stores the last n phonemes in string 'phonemes', returns TRUE if
// successful, returns FALSE if too many phonemes requested (but still
// returns the max number of phonemes for that word) */
int returnPhonemes(char* string, int numPhonemes, char phonemes[]) {
  int i = strlen(string)-1, j = 0, count = 0;
  int hash = hashIndex(string);

  while (i >= hash) {
    /* Removes \n and random chars from string */
    while (!isprint(string[i])) {
      i--;
    }
    if (string[i] == ' ' || string[i] == '#') {
      count++;
    }
    if (count == numPhonemes) {
      phonemes[j] = '\0';
      strrev(phonemes);
      return TRUE;
    }
    /* If '#' is reached but haven't got enough phonemes, return FALSE
    // - still store reversed string */
    if (string[i] == '#') {
      if (count < numPhonemes) {
        phonemes[j] = '\0';
        strrev(phonemes);
        return FALSE;
      }
    }
    phonemes[j] = string[i];
    i--;
    j++;
  }
  return FALSE;
}

/* String.h should include a string reverse function but isn't available
// on linux for some reason? Adapted code from:
// https://stackoverflow.com/questions/8534274/is-the-strrev-function-
// not-available-in-linux */
void strrev(char *string) {
  int i = strlen(string)-1, j = 0;
  char temp;

  if (string != NULL) {
    while (i > j) {
      temp = string[i];
      string[i] = string[j];
      string[j] = temp;
      j++;
      i--;
    }
  }
}

void capitaliseString(char *string) {
  int i = 0, length = strlen(string);
  while (i < length) {
    string[i] = toupper(string[i]);
    i++;
  }
}
