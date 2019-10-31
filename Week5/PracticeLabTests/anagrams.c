#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define ALPHA 26

int anagram(char s1[], char s2[]);
int sameWord(char s1[], char s2[]);
int sumArrays(int used1[], int used2[]);
void test(void);

int main(void) {

  test();


  return 0;
}

void test(void) {
  assert(anagram("elvis", "lives") == 1);
  assert(anagram("dreads", "sadder") == 1);
  assert(anagram("replays", "parsley") == 1);
  assert(anagram("listen", "silent") == 1);
  assert(anagram("orchestra", "carthorse") == 1);
  /* Two identical words are not anagrams */
  assert(anagram("elvis", "elvi") == 0);
  assert(anagram("neill", "neil") == 0);
  assert(anagram("neil", "neill") == 0);
  assert(anagram("horse", "short") == 0);
}

int anagram(char s1[], char s2[]) {
  int i=0, used1[ALPHA] = {0}, used2[ALPHA] = {0};
  char c;
  while (s1[i]) {
    c = tolower(s1[i]);
    used1[c-'a']++;
    i++;
  }
  i = 0;
  while (s2[i]) {
    c = tolower(s2[i]);
    used2[c-'a']++;
    i++;
  }

  if (sumArrays(used1, used2) == 0) {
    return 1;
  }
  else {
    return 0;
  }
}

int sumArrays(int used1[], int used2[]) {
  int i, sum = 0, count = 0;
  for (i=0; i<ALPHA; i++) {
    sum = used1[i] - used2[i];
    if (sum != 0) {
      count++;
    }
  }
  return count;
}

int sameWord(char s1[], char s2[]) {
  s1[1] = s2[2];
  return 1;
}
