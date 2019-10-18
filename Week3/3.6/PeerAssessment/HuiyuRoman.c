#include<stdio.h>
#include<string.h>
#include<assert.h>

int romanToArabic(char* roman);
void test();

int main(int argc, char** argv) {

	if (argc == 2) {
		printf("The romam numeral%s is equal to %d\n", \
			argv[1], romanToArabic(argv[1]));
	}
	else {
		printf("ERROR:Incorrect usage,try e.g.%sXXI\n", argv[0]);
	}
	return 0;
}
void test() {

	assert(romanToArabic("MCMXCIX") == 1999);
	assert(romanToArabic("MCMLXVII") == 1967);
	assert(romanToArabic("MCDXCI") == 1491);

}
int romanToArabic(char* roman) {

	int len;
	int  i;
	int sum = 0;
	len = strlen(roman);

	for (i = 0; i < len; i++) {
		if (*(roman + i) == 'I') {
			if (*(roman + i + 1) == 'V') {
				sum += 4;
				i++;
			}
			else if (*(roman + i + 1) == 'X') {
				sum += 9;
				i++;
			}
			else {
				sum++;
			}

			continue;

		}
		if (*(roman + i) == 'V') {
			sum = sum + 5;

			continue;
		}
		if (*(roman + i) == 'X') {
			if (*(roman + i + 1) == 'L') {
				sum += 40;
				i++;
			}
			else if (*(roman + i + 1) == 'C') {
				sum += 90;
				i++;
			}
			else {
				sum += 10;
			}
			continue;
		}
		if (*(roman + i) == 'L') {
			sum += 50;

			continue;

		}
		if (*(roman + i) == 'C') {
			if (*(roman + i + 1) == 'D') {
				sum += 400;
				i++;
			}
			else if (*(roman + i + 1) == 'M') {
				sum += 900;
				i++;
			}
			else {
				sum += 100;
			}
			continue;
		}
		if (*(roman + i) == 'D') {
			sum += 500;

			continue;
		}
		if (*(roman + i) == 'M') {
			sum += 1000;

			continue;
		}
	}

	return sum;
}


