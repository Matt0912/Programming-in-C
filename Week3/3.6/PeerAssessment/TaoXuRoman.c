#include <stdio.h>
#include <assert.h>
#include <ctype.h>


int romanToArabic(char *roman);
void test(void);


int main(int argc, char **argv)
{
 char *p;
 test();

 if(argc == 2){
  p = argv[1];
  while(*p){
   *p = toupper(*p);
   p++;}
  printf("The roman numeral %s is equal to %d\n",\
         argv[1], romanToArabic(argv[1]));
 }else{
  printf("ERROR: Incorrect usage, try e.g.%s XXI\n", argv[0]);
 }
 return 0;
}


int romanToArabic(char *roman)
{
 int a = 0;
 while(*roman){
  switch(*roman){
   case 'I':
    if(*++roman == 'V'){
     a += 4;
     roman++;
    }
    else if(*roman == 'X'){
     a += 9;
     roman++;
    }
    else{
     a += 1;}
    break;

   case 'V':
    a += 5;
    roman++;
    break;   

   case 'X':
    if(*++roman == 'L'){
      a += 40;
      roman++;
    }
     else if(*roman == 'C'){
      a += 90;
      roman++;
    }
     else{
      a += 10;}
     break;

   case 'L':
    a += 50;
    roman++;
    break;  

   case 'C':
    if(*++roman == 'D'){
      a += 400;
      roman++;
    }
     else if(*roman == 'M'){
      a += 900;
      roman++;
    }
     else{
      a += 10;}
     break;

   case 'D':
    a += 500;
    roman++;
    break;  

   case 'M':
    a += 1000;
    roman++;
    break; 
   
  }	/* switch */
 }	/* while */
 return a;
 
}


void test(void)
{
 assert(romanToArabic("IX") == 9);
 assert(romanToArabic("MCMXCIX") == 1999);
 assert(romanToArabic("MCMLXVII") == 1967);
 assert(romanToArabic("MCDXCI") == 1491);
}
