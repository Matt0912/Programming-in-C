/* TIME TAKEN TO SORT 370K - 1199.75 SECONDS
// TIME TAKEN TO SORT 9K - 0.75 SECONDS */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAXLISTLEN 400000
#define MAXWORDLEN 40

enum bool {FALSE, TRUE};

void test(void);
void printList(char List[MAXLISTLEN][MAXWORDLEN]);
void lowerString(char *string, char *lowerString);
int isLarger(char *listWord, char *newWord);
int addToList(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord);
void shiftWords(char sortedList[MAXLISTLEN][MAXWORDLEN], int i);

int main(int argc, char **argv) {
  FILE* fp;
  char str[MAXWORDLEN];
  static char sortedList[MAXLISTLEN][MAXWORDLEN];
  clock_t start, end;
  double cpu_time_used;
  test();

  start = clock();

  if(argc!=2) {
    argv[1] = "eng_370k_shuffle.txt";
    fprintf(stderr,"ERROR: Incorrect usage, defaulted to eng_370k_shuffle2.txt"
    " - smaller text file\n");
  }

  if (!(fp = fopen(argv[1], "r"))) {
    fprintf(stderr, "ERROR - Unable to open file %s\n", argv[1]);
    exit(1);
  }
  else {
    fprintf(stderr, "'%s' loaded correctly\n", argv[1]);
  }

  while (fgets(str, MAXWORDLEN, fp) != NULL) {
    addToList(sortedList, str);
  }

  printList(sortedList);
  end = clock();
  cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
  fprintf(stdout, "TIME TAKEN: %f SECONDS\n", cpu_time_used);
  fclose(fp);
  return FALSE;
}


void test(void) {
  static char sortedList[MAXLISTLEN][MAXWORDLEN];
  static char testList[MAXLISTLEN][MAXWORDLEN] = {"hello", "word", "allow",
                                               "search", "insert", "might",
                                           "test", "code", "allow", "thanks"};
  char teststr1[MAXWORDLEN], teststr2[MAXWORDLEN], teststr3[MAXWORDLEN];
  char lowerstr1[MAXWORDLEN], lowerstr2[MAXWORDLEN], lowerstr3[MAXWORDLEN];

  assert(isLarger("allowed", "maybe") == TRUE);
  assert(isLarger("Allowed", "Z") == TRUE);
  assert(isLarger("allowed", "Allowed") == FALSE);
  assert(isLarger("Allowed", "Allowed") == FALSE);
  assert(isLarger("allowed", "alloweD") == FALSE);
  assert(isLarger("allowed", "allowfd") == TRUE);
  assert(isLarger("allow", "allowing") == TRUE);
  assert(isLarger("almighty", "alright") == TRUE);

  strcpy(teststr1, "HeLLO");
  strcpy(teststr2, "hello");
  strcpy(teststr3, "SPELLCHECKer");
  lowerString(teststr1, lowerstr1);
  lowerString(teststr2, lowerstr2);
  lowerString(teststr3, lowerstr3);
  assert(strlen(lowerstr1) == strlen(teststr1));
  assert(strlen(lowerstr2) == strlen(teststr2));
  assert(strlen(lowerstr3) == strlen(teststr3));
  assert(strcmp(lowerstr1, "hello") == 0);
  assert(strcmp(lowerstr2, "hello") == 0);
  assert(strcmp(lowerstr3, "spellchecker") == 0);

  strcpy(teststr1, testList[6]);
  strcpy(teststr2, testList[7]);
  strcpy(teststr3, testList[8]);
  shiftWords(testList, 4);
  assert(strcmp(testList[4], testList[5]) == 0);
  assert(strcmp(teststr1, testList[7]) == 0);
  assert(strcmp(teststr2, testList[8]) == 0);
  assert(strcmp(teststr3, testList[9]) == 0);

  addToList(sortedList, "Zapped");
  addToList(sortedList, "alright");
  addToList(sortedList, "maybe");
  addToList(sortedList, "mast");
  addToList(sortedList, "aim");
  assert(strcmp(sortedList[0],"aim") == 0);
  assert(strcmp(sortedList[1],"alright") == 0);
  assert(strcmp(sortedList[2],"mast") == 0);
  assert(strcmp(sortedList[3],"maybe") == 0);
  assert(strcmp(sortedList[4],"Zapped") == 0);

}

/* If new word is larger than word in list, return TRUE */
int isLarger(char *listWord, char *newWord) {
  int i, length1, length2;
  length1 = strlen(listWord);
  length2 = strlen(newWord);

  for (i = 0; i < length1; i++) {
    if (tolower(listWord[i]) < tolower(newWord[i])) {
      return TRUE;
    }
    else if (tolower(listWord[i]) > tolower(newWord[i])) {
      return FALSE;
    }
  }
  if (i == length1 && length2 > length1) {
    return TRUE;
  }
  return FALSE;
}

int addToList(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord) {
  int i = 0;
  char lowerListWord[MAXWORDLEN], lowerNewWord[MAXWORDLEN];
  lowerString(newWord, lowerNewWord);
  while (i < MAXLISTLEN) {
    if (strlen(sortedList[i]) == 0) {
      strcpy(sortedList[i], newWord);
      return TRUE;
    }
    if (isLarger(sortedList[i], newWord)) {
      i++;
    }
    else if (!isLarger(sortedList[i], newWord)) {
      lowerString(sortedList[i], lowerListWord);
      if (strcmp(lowerListWord, lowerNewWord) != 0) {
        shiftWords(sortedList, i);
        strcpy(sortedList[i], newWord);
        return TRUE;
      }
      else {
        return FALSE;
      }
    }
  }

  return FALSE;
}

void shiftWords(char sortedList[MAXLISTLEN][MAXWORDLEN], int i) {
  int currentLength = i;
  while (strlen(sortedList[currentLength]) != 0) {
    currentLength++;
  }
  while(currentLength > i) {
    strcpy(sortedList[currentLength], sortedList[currentLength - 1]);
    currentLength--;
  }
}

void lowerString(char *string, char *lowerString) {
  int i = 0;
  while (string[i]) {
    lowerString[i] = tolower(string[i]);
    i++;
  }
  lowerString[i] = '\0';
}

void printList(char List[MAXLISTLEN][MAXWORDLEN]) {
  int i;
  FILE* fpWrite;

  fpWrite = fopen("SortedOutput.txt", "w");

  for (i = 0; i < MAXLISTLEN; i++) {
    if (strlen(List[i]) != 0) {
      fprintf(fpWrite, "%6i: %s", i+1, List[i]);
    }
    else {
      i = MAXLISTLEN;
    }
  }
  fclose(fpWrite);
}
