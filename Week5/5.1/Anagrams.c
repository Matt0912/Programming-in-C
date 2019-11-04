#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define ALPHA 26
#define MAXLEN 50
enum bool {FAIL, PASS};

void test(void);
int isAnagram(char s1[], char s2[]);

int main(int argc, char **argv) {
  FILE* fp;
  char currentWord[MAXLEN];
  test();

  if(argc!=2) {
    fprintf(stderr,"ERROR: Incorrect usage: ./Anagrams 'word'\n");
  }

  if ((fp = fopen("eng_370k_shuffle.txt", "r")) == NULL) {
    fprintf(stderr, "ERROR - Unable to open eng_370k_shuffle.txt");
    exit(1);
  }

  while (fscanf(fp, "%s", currentWord) != EOF) {
    if (isAnagram(currentWord, argv[1])) {
      printf("%s\n", currentWord);
    }
  }



  return 0;
}

void test(void) {
  assert(isAnagram("elvis", "lives") == 1);
  assert(isAnagram("dreads", "sadder") == 1);
  assert(isAnagram("replays", "parsley") == 1);
  assert(isAnagram("listen", "silent") == 1);
  assert(isAnagram("orchestra", "carthorse") == 1);
  /* Two identical words are not isAnagrams */
  assert(isAnagram("elvis", "elvis") == 0);
  assert(isAnagram("neill", "neil") == 0);
  assert(isAnagram("neil", "neill") == 0);
  assert(isAnagram("horse", "short") == 0);

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
