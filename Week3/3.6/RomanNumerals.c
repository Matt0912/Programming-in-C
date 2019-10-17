#include <stdio.h>
#include <assert.h>
#include <string.h>
const char romanNumerals[] = "IVXLCDM";
const int arabicNumbers[] = {1,5,10,50,100,500,1000};
#define ARRAYSIZE 7

void test(void);
int romanToArabic(char *roman);
int subtractCases(char *roman, int j);

int main(void) {

  test();

  return 0;
}

int romanToArabic(char *roman) {
  int i = 0, arabic = 0, j;
  while (roman[i]) {
    for (j=0;j<ARRAYSIZE;j++) {
      if (subtractCases(roman, j) == 0) {
        if (roman[i] == romanNumerals[j]) {
          arabic += arabicNumbers[j];
        }
        else if (subtractCases(roman,j) == 1) {
          arabic -= (roman[j])
        }
      }
    }
    i++;
  }

  printf("%s = %d\n", roman, arabic);

  return arabic;
}

int subtractCases(char *roman, int j) {
  if (j>0) {
    if (roman[j] > roman[j-1]) {
        return 1;
    }
  }
  return 0;




void test(void) {
  assert(romanToArabic("MCMXCIX") == 1999);
  assert(romanToArabic("MCMLXVII") == 1967);
  assert(romanToArabic("MCDXCI") == 1491);
}
