#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#define MAXSTRING 75

void test(void);
int isPalindrome(char *input, char *newString);
int formatString(char *input, char *newString);

int main(void) {
  char input[MAXSTRING], newString[MAXSTRING];
  test();
  printf("Input a palindrome you want to test: ");
  scanf("%[^\n]%*c", input);

  if (isPalindrome(input, newString)) {
    printf("Congratulations! \"%s\" is a palindrome!\n", input);
  } else {
    printf("\"%s\" is not a palindrome.\n", input);
  }

  return 0;
}

int formatString(char *input, char *newString) {
  /* Formats input as lowercase new string AND returns new string length */
  int i=0, count = 0;
  while (input[i]) {
    if (isalpha(input[i])) {
      newString[count] = tolower(input[i]);
      count++;
    }
    i++;
  }
  return count;
}

int isPalindrome(char *input, char *newString) {
  int i, strlength;
  strlength = formatString(input, newString);
  for (i=0;i<strlength;i++) {
    if (newString[i] != newString[strlength-(i+1)]) {
      return 0;
    }
  }
  return 1;
}

void test(void) {
  char testString[100];
  assert(isPalindrome("kayak",testString) == 1);
  assert(isPalindrome("A man, a plan, a canal: Panama!",testString) == 1);
  assert(isPalindrome("Madam, in Eden I’m Adam,",testString) == 1);
  assert(isPalindrome("Level, madam, level!",testString) == 1);
  assert(isPalindrome("hello MY Name IS",testString) == 0);
  assert(formatString("aZH:el:-l o.", testString)==7);
  assert(formatString("      8hi()()..!", testString)==2);

}
