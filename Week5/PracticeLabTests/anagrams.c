#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define ALPHA 26
enum bool {FAIL, PASS};

int anagram(char s1[], char s2[]);
int derange(char s1[], char s2[]);
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
  assert(anagram("elvis", "elvis") == 0);
  assert(anagram("neill", "neil") == 0);
  assert(anagram("neil", "neill") == 0);
  assert(anagram("horse", "short") == 0);

  assert(derange("elvis", "lives") == 0);
  assert(derange("dreads", "sadder") == 1);
  assert(derange("replays", "parsley") == 1);
  assert(derange("listen", "silent") == 0);
  assert(derange("orchestra", "carthorse") == 1);
}

int anagram(char s1[], char s2[]) {
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

int derange(char s1[], char s2[]) {
  int i = 0;
  char c1, c2;
  if (anagram(s1, s2)) {
    while (s1[i]) {
      c1 = tolower(s1[i]);
      c2 = tolower(s2[i]);
      if (c1 == c2) {
        return FAIL;
      }
      i++;
    }
    return PASS;
  }
  else {
    return FAIL;
  }
}
