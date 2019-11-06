#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define ALPHA 26
#define MAXWORDLEN 50
#define MAXANAGRAMS 20
enum bool {FAIL, PASS};

typedef struct anagramList {
  char wordList[MAXANAGRAMS][MAXWORDLEN];
  int listlength;
  struct anagramList *next;
} anagramList;

void test(void);
int isAnagram(char s1[], char s2[]);
void printList(anagramList *cp);

int main(void) {
  FILE* fp;
  char currentWord[MAXWORDLEN];
  anagramList head = {{{""},{""}}, 0, NULL}, *cp = &head;
  int found;
  test();

  if ((fp = fopen("eng_370k_shuffle2.txt", "r")) == NULL) {
    fprintf(stderr, "ERROR - Unable to open eng_370k_shuffle.txt");
    exit(1);
  }

  while (fscanf(fp, "%s", currentWord) != EOF) {
    found = FAIL;
    cp = &head;
    while (found == FAIL) {
      if (isAnagram(currentWord, cp->wordList[0])) {
        cp->listlength = cp->listlength + 1;
        strcpy(cp->wordList[cp->listlength], currentWord);
        found = PASS;
      }
      else if (cp->next == NULL){
        cp->next = (anagramList *)malloc(sizeof(anagramList));
        cp = cp->next;
        strcpy(cp->wordList[0], currentWord);
        cp->listlength = 1;
        cp->next = NULL;
        found = PASS;
      }
      cp = cp->next;
    }
  }

  printList(&head);
  /*
  fprintf(stdout, "%s\n", head.wordList[0]);
  fprintf(stdout, "%s\n", head.next->wordList[0] );
  fprintf(stdout, "%s\n", head.next->next->wordList[0] );
  */

  return 0;
}

void test(void) {
  assert(isAnagram("elvis", "lives") == 1);
  assert(isAnagram("dreads", "sadder") == 1);
  assert(isAnagram("replays", "parsley") == 1);
  assert(isAnagram("listen", "silent") == 1);
  assert(isAnagram("orchestra", "carthorse") == 1);
  assert(isAnagram("elvis", "elvis") == 0);
  assert(isAnagram("neill", "negl") == 0);
  assert(isAnagram("negl", "neill") == 0);
  assert(isAnagram("horse", "short") == 0);

}

void printList(anagramList *cp) {
  int i, count = 0;
  while (cp->next != NULL) {
    count++;
    if (cp->listlength > 1) {
      fprintf(stdout, "%d: ", cp->listlength);
      for (i = 0; i <= cp->listlength; i++) {
        fprintf(stdout, "%s ", cp->wordList[i]);
      }
      fprintf(stdout, "\n");
    }
    cp = cp->next;
  }
}

int isAnagram(char s1[], char s2[]) {
  int i=0, used[ALPHA] = {0};
  char c;
  if (strcmp(s1, s2) != 0 && strlen(s1) == strlen(s2)) {
    while (s1[i]) {
      c = tolower(s1[i]);
      used[c-'a']++;
      c = tolower(s2[i]);
      used[c-'a']--;
      i++;
    }

    for (i=0; i < ALPHA; i++) {
      if (used[i] != 0) {
        return FAIL;
      }
    }
    return PASS;
  }
  return FAIL;
}
