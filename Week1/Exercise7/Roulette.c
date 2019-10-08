#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int userInput(int funds);
int getRand();
int checkBet(int answer, int random_number);

int main(void) {

  int input, result, funds = 10, winnings;
  srand(time(NULL));

  printf("*************************** WELCOME TO ROULETTE ***************************\n");
  printf("You have £10 to play with - you can bet on odd, even, or a number");
  printf(" between 0 and 35.\n");

  while (funds > 0) {
    input = userInput(funds);
    funds -= 1;
    result = getRand();
    winnings = checkBet(input, result);
    if (winnings > 0) {
      funds += winnings + 1;
    }
  }

  return 0;
}

int userInput(int funds) {
  int input;
  printf("\nCurrent Balance = £%d\n", funds);
  printf("To bet on ODD, enter '100', to bet on EVEN, enter '200', or enter a number: ");
  scanf("%d", &input);
  if (input < 0){
    printf("NOT A VALID INPUT\n");
    return -1;
  }
  else {
    return input;
  }
}

int getRand() {
  int randomNumber = (rand() % 35) + 1;
  printf("The roulette wheel has landed on... %d\n", randomNumber);
  return randomNumber;
}

int checkBet(int answer, int random_number) {
  if (answer == 100) {
    if (random_number%2 == 0) {
      return 0;
    } else {
      printf("YOU WON £1!\n");
      return 1;
    }
  }
  if (answer == 200) {
    if (random_number%2 == 0) {
      printf("YOU WON £1!\n");
      return 1;
    } else {
      return 0;
    }
  }
  if (answer < 36) {
    if (random_number == answer) {
      printf("YOU WON £35!!\n");
      return 35;
    }
  }

  return 0;
}
