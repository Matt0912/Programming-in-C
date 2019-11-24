#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAXLISTLEN 400000
#define MAXWORDLEN 40

#define DELAY 0.0001

enum bool {FALSE, TRUE};

void test(void);
void lowerString(char *string, char *lowerString);
int isLarger(char *newWord, char *listWord);
int linearSearch(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord, int end);
int binarySearch(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord,
                 int start, int end);
/*
int interpSearch(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord,
                 int start, int end);
*/
int checkFound(char sortedList[MAXLISTLEN][MAXWORDLEN], int index, char *newword);
int copyFromFile(FILE *fp, char sortedList[MAXLISTLEN][MAXWORDLEN]);
void printList(char List[MAXLISTLEN][MAXWORDLEN]);
void delay(int seconds);

int main(int argc, char** argv) {
  FILE *fp;
  static char sortedList[MAXLISTLEN][MAXWORDLEN];
  int fileLength;
  char userInput[MAXWORDLEN];
  clock_t start, end;
  double cpu_time_used;
  test();

  if (argc == 2) {
    strcpy(userInput, argv[1]);
  } else {
    fprintf(stderr, "INCORRECT USAGE: e.g ./binarysearch hello");
  }

  fp = fopen("SortedOutput.txt", "r");
  fileLength = copyFromFile(fp, sortedList);
  fclose(fp);

  start = clock();
  linearSearch(sortedList, userInput, fileLength);
  end = clock();
  cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
  fprintf(stdout, "TIME TAKEN FOR LINEAR SEARCH: %f SECONDS\n", cpu_time_used);

  start = clock();
  binarySearch(sortedList, userInput, 0, fileLength);
  end = clock();
  cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
  fprintf(stdout, "TIME TAKEN FOR BINARY SEARCH: %f SECONDS\n", cpu_time_used);

  /*
  start = clock();
  interpSearch(sortedList, userInput, 0, fileLength);
  end = clock();
  cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
  fprintf(stdout, "TIME TAKEN FOR INTERPOLATION SEARCH: %f SECONDS\n", cpu_time_used);
  */
  return FALSE;
}


void test(void) {
  assert(isLarger("allowed", "maybe") == TRUE);
  assert(isLarger("Allowed", "Z") == TRUE);
  assert(isLarger("allowed", "Allowed") == FALSE);
  assert(isLarger("Allowed", "Allowed") == FALSE);
  assert(isLarger("allowed", "alloweD") == FALSE);
  assert(isLarger("allowed", "allowfd") == TRUE);
  assert(isLarger("allow", "allowing") == TRUE);
  assert(isLarger("almighty", "alright") == TRUE);
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

int linearSearch(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord, int end) {
  int i = 0;
  while (i < end) {
    delay(DELAY);
    if (strcmp(sortedList[i], newWord) == 0) {
      fprintf(stdout, "Linear: %s is #%d in the list!\n", newWord, i+1);
      return TRUE;
    }
    i++;
  }
  fprintf(stdout, "Linear: %s is NOT in the list!\n", newWord);
  return FALSE;
}

int binarySearch(char sortedList[MAXLISTLEN][MAXWORDLEN], char *newWord,
                 int start, int end)
{
  int middle = ((end - start)/2) + start;
  delay(DELAY);
  if (checkFound(sortedList, start, newWord) ||
      checkFound(sortedList, middle, newWord) ||
      checkFound(sortedList, end, newWord)) {
    return TRUE;
  }

  if (start == end || end - start == 1) {
    fprintf(stdout, "Binary: %s is NOT in the list!\n", newWord);
    return FALSE;
  }

  if (isLarger(sortedList[middle], newWord)) {
    start = middle + 1;
  }
  else {
    end = middle - 1;
  }
  binarySearch(sortedList, newWord, start, end);

  return FALSE;
}

int checkFound(char sortedList[MAXLISTLEN][MAXWORDLEN], int index, char *newWord) {
  if (strcmp(sortedList[index], newWord) == 0) {
    fprintf(stdout, "Binary: %s is #%d in the list!\n", newWord, index+1);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/* Copies from file to sortedList sortedListay */
int copyFromFile(FILE *fp, char sortedList[MAXLISTLEN][MAXWORDLEN]) {
  char randStr[MAXWORDLEN];
  int i = 0, j=0;
  while (fscanf(fp, "%d %s %s", &i, randStr, sortedList[j]) != EOF) {
    j++;
  }
  return j;
}

void printList(char List[MAXLISTLEN][MAXWORDLEN]) {
  int i;
  for (i = 0; i < MAXLISTLEN; i++) {
    if (strlen(List[i]) != 0) {
      fprintf(stdout, "%6i: %s", i+1, List[i]);
    }
    else {
      i = MAXLISTLEN;
    }
  }
}

void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
