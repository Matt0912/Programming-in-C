#include <stdio.h>
#define NDEBUG
#include <assert.h>
#include <string.h>
const char romanNumerals[] = "IVXLCDM";
const int arabicNumbers[] = {1,5,10,50,100,500,1000};
#define ARRAYSIZE 7

void test(void);
int romanToArabic(char *roman);
int subtractCase(char *roman, int i);
int checkInput(char *roman);

int main(int argc, char **argv) {

  test();

  if( argc==2 ) {
    if (checkInput(argv[1])) {
      printf("The roman numeral %s is equal to %d\n", argv[1], romanToArabic(argv[1]));
    }
  }
  else {
    printf("ERROR: Incorrect usage, try e.g. %s XXI\n", argv[0]);
  }

  return 0;
}

int checkInput(char *roman) {
  int i=0, j, check, check1, check2, length = strlen(roman), letter1=0, letter2=0;
  int maxvalue=0;
  /* CHECKS USER INPUT USES VALID ROMAN NUMERAL CHARACTERS */
  while (roman[i]) {
    check = 0;
    for (j=0;j<ARRAYSIZE;j++) {
      if (roman[i] == romanNumerals[j]) {
        check = 1;
      }
    }
    if (check == 0) {
      printf("Invalid input! Try using numerals I,V,X,L,C,D,M.\n");
      return 0;
    }
    i++;
  }

  /* CHECKS USER INPUT IS A VALID ROMAN NUMERAL */
  check = 0; check1=0; check2=0; i=0;
  while (roman[i]) {
    /* CHECKS FOR REPETITION OF V, L AND D */
    if (roman[i] == romanNumerals[1]) {
      check++;
    }
    if (roman[i] == romanNumerals[3]) {
      check1++;
    }
    if (roman[i] == romanNumerals[5]) {
      check2++;
    }
    if (check > 1 || check1 > 1 || check2 > 1) {
      printf("Invalid number! Can only have one V, L and D per numeral\n");
      return 0;
    }
    /* CHECKS FOR MORE THAN 4 LETTERS BEING USED IN A ROW */
    if (i <= length - 2) {
      for (j=0;j<ARRAYSIZE;j++) {
        if (roman[i] == romanNumerals[j]) {
          if ((roman[i] == roman[i+1]) && (roman[i+1] == roman[i+2]) \
          && (roman[i+2] == roman[i+3])) {
            printf("Invalid number! Four %c's should be an %c%c!\n", roman[i],\
             romanNumerals[j], romanNumerals[j+1]);
            return 0;
          }
        }
      }
    }

    /* CHECKS THAT SUBTRACTED VALUE IS NOT TOO SMALL */
    for (j=0;j<ARRAYSIZE;j++) {
      if (subtractCase(roman, i)) {
        if (roman[i] == romanNumerals[j]) {
          letter1 = arabicNumbers[j];
        }
        if (roman[i+1] == romanNumerals[j]) {
          letter2 = arabicNumbers[j];
        }
        if (letter2 > letter1*10) {
          printf("Invalid number! Subtracted numeral must be at least 1/10 the \
  value of the larger numeral!\n");
          return 0;
        }
      }
      if (roman[0] == romanNumerals[j]) {
        maxvalue = arabicNumbers[j];
      }
      if (roman[i] == romanNumerals[j]) {
        if (arabicNumbers[j] > maxvalue) {
          printf("Invalid number! Values must decrease from left to right, e.g. MCXII\n");
          return 0;
        }
        else {
          maxvalue = arabicNumbers[j];
          printf("MAX VALUE IS %d\n", maxvalue);
        }
      }
    }

    i++;
  }
  return 1;
}

int romanToArabic(char *roman) {
  int i=0, arabic = 0, j;
  while (roman[i]) {
    for (j=0;j<ARRAYSIZE;j++) {
      if (subtractCase(roman, i) == 0) {
        if (roman[i] == romanNumerals[j]) {
          arabic += arabicNumbers[j];
        }
      }
      else if (subtractCase(roman, i)) {
        if (roman[i] == romanNumerals[j]) {
          arabic -= arabicNumbers[j];
        }
      }
    }
    i++;
  }
  return arabic;
}

/* Returns 1 if character needs to be subtracted, 0 otherwise */
/* E.g if IX is input, it will return 1 and 0, subtracting the 1 and adding the 10 */
int subtractCase(char *roman, int i) {
  int letter1=0, letter2=0, j, length = strlen(roman);
  if (i<length) {
    for (j=0;j<ARRAYSIZE;j++) {
      if (roman[i] == romanNumerals[j]) {
        letter1 = arabicNumbers[j];
      }
      if (roman[i+1] == romanNumerals[j]) {
        letter2 = arabicNumbers[j];
      }
    }
    if (letter2 > letter1) {
      return 1;
    }
  }
  return 0;
}


void test(void) {
  assert(checkInput("XXXX") == 0);
  assert(checkInput("VV") == 0);
  assert(checkInput("XXJHL") == 0);
  assert(checkInput(" ") == 0);
  assert(subtractCase("XXIX", 2) == 1);
  assert(subtractCase("XXIX", 1) == 0);
  assert(subtractCase("XXIX", 3) == 0);
  assert(subtractCase("MCM", 1) == 1);
  assert(romanToArabic("MCMXCIX") == 1999);
  assert(romanToArabic("MCMLXVII") == 1967);
  assert(romanToArabic("MCDXCI") == 1491);
}
