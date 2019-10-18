#include <stdio.h>
#include <assert.h>
#include <ctype.h> 

/*                   --- ROMAN ---

Application receives character input from the user, 
a function checks the characters are valid roman numerals 
and if so, the input is converted to arabic numerals and 
returns this value to the user. 

*/

#define TRUE    1
#define FALSE   0

#define I       1   
#define X       10
#define C       100

/* Total number of roman numeral characters. */
#define NUMERALS  7

/* The maximum number that can be defined using this 
particular roman numeral set. */
#define MAXNUMB 3999

/* Checks whether input characters are valid roman 
numerals. */
int checkvalidchar(char *roman);

/* Sums the total of each valid roman numeral character. */
int valuatechars(char *roman);

/* Determines how much to subtract based on any valid two 
character combinations. i.e. for the numeral IV, I (1) is 
subtracted twice from the values of V and I (6). */
int subtractdoublechar(char *roman);

/* Final function subtracts from the sum to return the
total value of the string in arabic numbers. */
int romanToArabic(char *roman);

/* Testing function. */
void testfunc(void);

int main(int argc, char **argv)
{
 testfunc();

 if(argc == 2){

   if(checkvalidchar(argv[1]) == FALSE){
     printf("Incorrect usage: Input includes characters which are not " 
     "valid roman numerals. \n");
     }

   if((romanToArabic(argv[1]) > MAXNUMB)){
      printf("Incorrect usage: The input number is not a valid roman "
      "numeral.\n");
     }

   if(checkvalidchar(argv[1]) == TRUE && 
      romanToArabic(argv[1]) <= MAXNUMB){
      printf("The roman numeral %s is equal to %d\n",\
      argv[1], romanToArabic(argv[1]));
     }
   } 
 
 else {
   printf("Error: Incorrect usage, try e.g. %s XXI\n", argv[0]);
   }

return 0;
}

/* FUNCTIONS BELOW */
/* Checks if the input characters are valid roman numerals */
/* I.e. characters U, F, G etc. func will be return false. */

int checkvalidchar(char *roman)
{
  int i;
  int invalidcount = 0;

  for(i = 0; roman[i] != '\0'; i++){
    roman[i] = toupper(roman[i]); 

    if(roman[i] != 'I' && roman[i] != 'V' && roman[i] != 'X' && 
      roman[i] != 'C' && roman[i] != 'M' && roman[i] != 'D' && 
      roman[i] != 'L'){
      invalidcount++;
    }
  } 

  if(invalidcount > 0){
    return FALSE;
  }

  else{
    return TRUE;
  }
}

/* Converts each character to the arabic equivalent
and returns the sum of all input characters. */

int valuatechars(char *roman)
{
  int i, j;
  unsigned sum = 0;
  int values[2][NUMERALS] = {
    {'I', 'V', 'X', 'L', 'C', 'D', 'M'},
    {1, 5, 10, 50, 100, 500, 1000}
      };

  for(i = 0; roman[i] != '\0'; i++){
    roman[i] = toupper(roman[i]); 

    for(j = 0; j < NUMERALS; j++){
      if((int) roman[i] == values[0][j]){
      sum = sum + values[1][j];
      }
    }
  }  
return sum;
}

/* Checks if the input characters are a roman set of two 
I.e. IV, IX, XC etc. then calculates how much to subtract 
from the sum. */

int subtractdoublechar(char *roman)
{
  int i;
  unsigned sum = 0;

  for(i = 0; roman[i] != '\0'; i++){
    roman[i] = toupper(roman[i]); 

    if(roman[i] == 'I' && (roman[i+1] == 'V' || 
      roman[i+1] == 'X')){
      sum = sum + (I * 2);
    }

    if(roman[i] == 'X' && (roman[i+1] == 'C' ||
      roman[i+1] == 'L')){
      sum = sum + (X * 2);
    }

    if(roman[i] == 'C' && (roman[i+1] == 'D' ||
      roman[i+1] == 'M')){
      sum = sum + (C * 2);
    }
  } 
return sum;
}

/* Function below takes the sum of characters and subtracts any
double chars to return total value. */

int romanToArabic(char *roman)
{
return (valuatechars(&roman[0]) - subtractdoublechar(&roman[0]));
}

void testfunc(void)
{
char testarray1[] = "mxi";
char testarray2[] = {'M', 'C', 'M', 'X', 'C', 'I', 'X', '\0'};
char testarray3[] = {'M', 'C', 'M', 'L', 'X', 'V', 'I', 'I', '\0'};
char testarray4[] = {'M', 'C', 'D', 'X', 'C', 'I', '\0'};
char testarray5[] = {'M', 'M', 'M', 'C', 'M', 'X', 'C', 'I', 'X', '\0'};
char testarray6[] = {'M', 'M', 'Y', 'Z', 'M', 'X', 'C', 'I', 'X', '\0'};
char testarray7[] = {'X', '\0'};
char testarray8[] = "IVXLCDM";
char testarray9[] = "IVIXXLXCCDCM";

/* Evaluates whether check valid func will flag false
for invalid input characters. */
assert(checkvalidchar(&testarray6[0]) == FALSE); 
assert(checkvalidchar(&testarray7[0]) == TRUE); 

/* Evaluates whether function will process lower case numerals. */
assert(romanToArabic(&testarray1[0]) == 1011); 

/* Checks the provided test cases & highest possible number 3999. */
assert(romanToArabic(&testarray2[0]) == 1999); 
assert(romanToArabic(&testarray3[0]) == 1967); 
assert(romanToArabic(&testarray4[0]) == 1491); 
assert(romanToArabic(&testarray5[0]) == 3999); 

/* Checks funcs assign correct values to each char. */
assert(valuatechars(&testarray8[0]) == 1666); 
assert(subtractdoublechar(&testarray9[0]) == 444);
}