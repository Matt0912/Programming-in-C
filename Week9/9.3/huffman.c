#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define ASCIICHARS 255

enum bool {FALSE, TRUE};

typedef struct arrayStruct {
  int freq;
  char c;
} arrayStruct;

typedef struct node {
  char c;
  int freq;
  struct node *left, *right;
} Node;

void test(void);
void initHistogram(arrayStruct histogram[ASCIICHARS], char *testString);
int compareFunc(const void * a, const void * b);
Node* arrToList(int histogram[ASCIICHARS]);
Node* newNode(char c, int freq);
void printArray(arrayStruct histogram[ASCIICHARS]);

int main(void) {
  test();



  return FALSE;
}

void test(void) {
  char testString[] = "This is the test string!!'@)";
  arrayStruct histogram[ASCIICHARS];

  /* Tests initHistogram - testString = "This is the test string!!'@)" */
  initHistogram(histogram, testString);

  assert(histogram[(int)testString[0]].freq == 1);
  assert(histogram[(int)testString[0]].c == 'T');
  assert(histogram[(int)testString[2]].freq == 3);
  assert(histogram[(int)testString[2]].c == 'i');
  assert(histogram[(int)testString[4]].freq == 4);
  assert(histogram[(int)testString[7]].freq == 4);
  assert(histogram[(int)testString[8]].freq == 4);
  assert(histogram[(int)'k'].freq == 0);
  assert(histogram[(int)'k'].c == 'k');
  assert(histogram[(int)'t'].freq == 4);
  assert(histogram[(int)'T'].freq == 1);
  assert(histogram[(int)'!'].freq == 2);

  qsort(histogram, ASCIICHARS, sizeof(arrayStruct), compareFunc);
  printArray(histogram);
}

void initHistogram(arrayStruct histogram[ASCIICHARS], char *testString) {
  int i;

  for (i = 0; i < ASCIICHARS; i++) {
    histogram[i].c = (char)i;
    histogram[i].freq = 0;
  }

  i = 0;
  while (testString[i]) {
    histogram[(int)testString[i]].c = testString[i];
    (histogram[(int)testString[i]].freq)++;
    i++;
  }
}


/* Frequency compare function for the quicksort */
int compareFunc(const void *a, const void *b) {
  arrayStruct *ia = (arrayStruct *)a;
  arrayStruct *ib = (arrayStruct *)b;
  return (ib->freq - ia->freq);
}

/*
Node* arrToList(int histogram[ASCIICHARS]) {
  Node *startNode = NULL, *currNode;
  int i;

  for (i = 0; i < ASCIICHARS; i++) {
    if (histogram[i] > 0) {
      if (startNode == NULL) {
        startNode = newNode((char)i, histogram[i]);
        currNode = startNode;
      }
      else {
        currNode->next = newNode((char)i, histogram[i]);
        currNode = currNode->next;
      }
    }
  }

  return startNode;
}*/

Node* newNode(char c, int freq) {
  Node* newNode = (Node* )malloc(sizeof(Node));
  newNode->c = c;
  newNode->freq = freq;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}

void printArray(arrayStruct histogram[ASCIICHARS]) {
  int i;
  for (i = 0; i < ASCIICHARS; i++) {
    if (histogram[i].freq > 0) {
      fprintf(stdout, "%c : %d\n", histogram[i].c, histogram[i].freq);
    }
  }
}
