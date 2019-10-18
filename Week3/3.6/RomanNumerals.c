#include <stdio.h>
#define NDEBUG
#include <assert.h>
#include <string.h>
const char romanNumerals[] = "IVXLCDM";
const int arabicNumbers[] = {1,5,10,50,100,500,1000};
#define ARRAYSIZE 7

/* COMMENTS FROM PEER ASSESSMENT
// db16981@bristol.ac.uk - No issues with code, good testing. 70%
// yy19612@bristol.ac.uk - The code is appropriately structured and the logic is clear to reader. It can be 
//complied and excuted normally and the test functions is appropriate as almost all possible input are coverd, 
//but I think it could be further improved in some aspects. 
//1. Global variables appering in line 5, 6 are not recommanded in chapter 1.1 - GLOB in house-style guildline; 
//2. Try to avoid using too deep nesting (e. g. line 123-129), it may not easy to read for readers; 
//3. Using 'typedef enum bool {False = 0, True = 1} bool;' to replace the 1 or 0 for your 'checkFunc'. 
//Overall, I would mark this code 65 / 70. 
// cc19055@bristol.ac.uk - Heavy workload. Most of cases are ok. But it's buggy. When inputting CDIV, your 
//program should get 404 but it didn't (NOW FIXED). I can see that you convert the question"Is a valid roman numeral?" 
//into several special cases. But your logic of if & else is not equivalent to the original question. For example, 
//func checkRomanNum()  outputs that XIXI is valid. func checkRomanNum2() reports CDIV is invalid. 
//House style: [LLEN] you can break some printf into 2 lines. [REPC]func checkValid() have checked the character, 
//in the following func, some sentences like if (roman[i] == romanNumerals[j]) is redundant.
//Testing: much but not convincing.
//Overall: 59%
*/

void test(void);
int romanToArabic(char *roman);
int subtractCase(char *roman, int i);
int checkInput(char *roman);
int checkValid(char *roman);
int checkRomanNum(char *roman);
int checkRomanNum2(char *roman);

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

/* CARRIES OUT ALL TESTING FUNCTIONS */
int checkInput(char *roman) {
  if (checkValid(roman) == 0) {
    return 0;
  }
  if (checkRomanNum(roman) == 0) {
    return 0;
  }
  if (checkRomanNum2(roman) == 0) {
    return 0;
  }
  return 1;
}

/* CHECKS USER INPUT USES VALID ROMAN NUMERAL CHARACTERS */
int checkValid(char *roman) {
  int i=0, j, check;
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
  return 1;
}

int checkRomanNum(char *roman) {
  int i=0, j, count=0, count1=0, count2=0, length = strlen(roman);
  while (roman[i]) {
    /* CHECKS FOR REPETITION OF V, L AND D */
    if (roman[i] == romanNumerals[1]) {
      count++;
    }
    if (roman[i] == romanNumerals[3]) {
      count1++;
    }
    if (roman[i] == romanNumerals[5]) {
      count2++;
    }
    if (count > 1 || count1 > 1 || count2 > 1) {
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
    i++;
  }
  return 1;
}

int checkRomanNum2(char *roman) {
  int i=0, j, k, letter1=1000, letter2=1000, maxvalue=0;
  while (roman[i]) {
    for (j=0;j<ARRAYSIZE;j++) {
      /* CHECKS THAT SUBTRACTED VALUE IS NOT TOO SMALL */
      if (subtractCase(roman, i)) {
        if (roman[i] == romanNumerals[j]) {
          letter1 = arabicNumbers[j];
        }
        if (roman[i+1] == romanNumerals[j]) {
          letter2 = arabicNumbers[j];
          if (letter2 > letter1*10) {
            printf("Invalid number! Subtracted numeral must be at least 1/10 the \
value of the larger numeral!\n");
            return 0;
          }
        }

      }
      /*CHECKS THAT NUMBERS DECREASE FROM LEFT TO RIGHT */
      if (roman[0] == romanNumerals[j]) {
        maxvalue = arabicNumbers[j];
      }
      if (roman[i] == romanNumerals[j]) {
        maxvalue = arabicNumbers[j];
        /* IF NEXT NUMBER IS GREATER AND IT ISN'T SUBTRACTED, GIVE ERROR */
        if (subtractCase(roman,i) == 0) {
          for (k=0;k<ARRAYSIZE;k++) {
            if (roman[i+1] == romanNumerals[k]) {
              if (arabicNumbers[k] > maxvalue) {
                printf("Invalid number! Values must decrease from left to right, e.g. MCXII\n");
                return 0;
              }
            }
          }
        }
        else if (subtractCase(roman,i)){
          /* CHECKS FOR CASES SUCH AS XXIIIV - CANNOT SUBTRACT LARGER NUMBER */
          if (roman[i] == roman[i-1]) {
            printf("Invalid number! Values must decrease from left to right, e.g. MCXII\n");
            return 0;
          }
          /* CHECKS IF THERE ARE MORE ADDITIONAL VALUES AFTER THE SUBTRACTION */
          for (k=0;k<ARRAYSIZE;k++) {
            if (roman[i+2] == romanNumerals[k]) {
              if (arabicNumbers[k] > maxvalue) {
                printf("Invalid number! Values must decrease from left to right, e.g. MCXII\n");
                return 0;
              }
            }
          }
        }
      }
    }
    i++;
  }
  return 1;
}

/* Takes roman numeral input, outputs arabic number equivalent */
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
  assert(checkRomanNum("XXXX") == 0);
  assert(checkRomanNum("IIII") == 0);
  assert(checkRomanNum("LLL") == 0);
  assert(checkRomanNum("VV") == 0);

  assert(checkValid("XXJHL") == 0);
  assert(checkValid("0AJKLXXI") == 0);

  assert(checkInput(" ") == 0);

  assert(checkRomanNum2("XXVXX") == 0);
  assert(checkRomanNum2("XXXLL") == 0);
  assert(checkRomanNum2("XXVIIX") == 0);

  assert(subtractCase("XXIX", 2) == 1);
  assert(subtractCase("XXIX", 1) == 0);
  assert(subtractCase("XXIX", 3) == 0);
  assert(subtractCase("MCM", 1) == 1);

  assert(romanToArabic("MCMXCIX") == 1999);
  assert(romanToArabic("MCMLXVII") == 1967);
  assert(romanToArabic("MCDXCI") == 1491);
}
