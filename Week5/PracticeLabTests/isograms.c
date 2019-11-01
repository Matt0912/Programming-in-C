#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#define ALPHA 26
#define MAXLEN 50
enum bool {FAIL, PASS};

void test(void);
int isogram(char *s);

int main(int argc, char **argv) {
  int maxlength = 0, length;
  char longestIsogram[MAXLEN], currentWord[MAXLEN];
  FILE* fp;
  test();

  if(argc!=2) {
    argv[1] = "eng_370k_shuffle.txt";
    fprintf(stderr,"ERROR: Incorrect usage, defaulted to eng_370k_shuffle.txt\n");
  }

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "ERROR - Unable to open file %s\n", argv[1]);
    exit(1);
  }

  while (fscanf(fp, "%s", currentWord) != EOF) {
    if (isogram(currentWord)) {
      length = strlen(currentWord);
      if (length > maxlength) {
        maxlength = length;
        strcpy(longestIsogram, currentWord);
      }
    }
  }
  fprintf(stdout, "The longest isogram in the list was %s, with %d letters.\n", longestIsogram, maxlength);

  fclose(fp);
  return 0;
}

void test(void) {
  assert(isogram("programming") == 0);
  assert(isogram("housewarmings") == 0);
  assert(isogram("abductions") == 1);
  assert(isogram("housewarming") == 1);
  assert(isogram("hydromagnetics") == 1);
  assert(isogram("uncopyrightable") == 1);
}

int isogram(char *s) {
  int i = 0, used[ALPHA] = {0};
  char c;

  while (s[i]) {
    c = tolower(s[i]);
    used[c-'a']++;
    i++;
  }

  for (i=0; i < ALPHA; i++) {
    if (used[i] > 1) {
      return FAIL;
    }
  }
  return PASS;
}
