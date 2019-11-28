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
} ArrayStruct;

typedef struct node {
  char c;
  int freq;
  struct node *left, *right;
} Node;

typedef struct nodeQueue {
  int size;
  int maxCapacity;
  Node** array;
} NodeQueue;

void test(void);
int initHistogram(ArrayStruct histogram[ASCIICHARS], char *testString);
int compareFunc(const void * a, const void * b);
NodeQueue* fillQueue(ArrayStruct histogram[ASCIICHARS], int maxCapacity);
Node *buildTree(NodeQueue *Queue);
Node* newNode(char c, int freq);
NodeQueue* newQueue(int maxCapacity);
void printQueue(NodeQueue* Queue);
void printArray(ArrayStruct histogram[ASCIICHARS]);

int main(void) {
  test();



  return FALSE;
}

void test(void) {
  char testString[] = "This is the test string!!'@)";
  ArrayStruct histogram[ASCIICHARS];
  NodeQueue *Queue;
  Node *treeRoot;
  int maxCapacity;
  void *testpoint1, *testpoint2;

  /* Tests initHistogram - testString = "This is the test string!!'@)" */
  maxCapacity = initHistogram(histogram, testString);

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
  assert(maxCapacity == 14);

  /* Tests compareFunc */
  testpoint1 = &histogram[(int)testString[0]];
  testpoint2 = &histogram[(int)testString[2]];
  assert(compareFunc(testpoint1, testpoint2) == 2);
  testpoint1 = &histogram[(int)testString[4]];
  testpoint2 = &histogram[(int)testString[8]];
  assert(compareFunc(testpoint1, testpoint2) == 0);

  qsort(histogram, ASCIICHARS, sizeof(ArrayStruct), compareFunc);
  Queue = fillQueue(histogram, maxCapacity);
  printQueue(Queue);

  /* Test fillQueue */
  assert(Queue->array[0]->freq == 4);
  assert(Queue->array[6]->freq == 2);
  assert(Queue->array[1]->c == 's');
  assert(Queue->array[10]->freq == 1);
  assert(Queue->size == maxCapacity);
  assert(Queue->maxCapacity == maxCapacity);

  treeRoot = buildTree(Queue);
  printQueue(Queue);

}

/* Create histogram of string made of structs containing letter + frequency
// and then return the number of non-zero elements (size) of array */
int initHistogram(ArrayStruct histogram[ASCIICHARS], char *testString) {
  int i, itemCount = 0;

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

  for (i = 0; i < ASCIICHARS; i++) {
    if (histogram[i].freq > 0) {
      itemCount++;
    }
  }

  return itemCount;
}


/* Frequency compare function for the quicksort */
int compareFunc(const void *a, const void *b) {
  ArrayStruct *ia = (ArrayStruct *)a;
  ArrayStruct *ib = (ArrayStruct *)b;
  return (ib->freq - ia->freq);
}

int compareFunc2(const void *a, const void *b) {
  Node *ia = (Node *)a;
  Node *ib = (Node *)b;
  return (ib->freq - ia->freq);
}

/* Create new Queue and fill it with nodes directly created from histogram */
NodeQueue* fillQueue(ArrayStruct histogram[ASCIICHARS], int maxCapacity) {
  NodeQueue* Queue = newQueue(maxCapacity);
  int i = 0;

  while (i < maxCapacity) {
    Queue->array[i] = newNode(histogram[i].c, histogram[i].freq);
    i++;
  }

  Queue->size = maxCapacity;

  return Queue;
}

Node *buildTree(NodeQueue *Queue) {
  Node *combinedNode;
  int combinedFreq, size = Queue->size - 1;

  while (size > 1) {
    size = Queue->size - 1;
    fprintf(stdout, "check");
    combinedFreq = Queue->array[size]->freq + Queue->array[size - 1]->freq;
    combinedNode = newNode('\0', combinedFreq);
    combinedNode->left = Queue->array[size];
    combinedNode->right = Queue->array[size-1];
    Queue->array[size-1] = combinedNode;
    qsort(Queue->array, size-1, sizeof(Node), compareFunc2);
    (Queue->size)--;
  }

  return Queue->array[0];
}

/* Malloc and initialise new node for tree */
Node* newNode(char c, int freq) {
  Node* newNode = (Node* )malloc(sizeof(Node));
  newNode->c = c;
  newNode->freq = freq;
  newNode->left = NULL;
  newNode->right = NULL;

  return newNode;
}

/* Malloc and create queue for storing new nodes */
NodeQueue* newQueue(int maxCapacity) {
  NodeQueue* Queue = (NodeQueue* )malloc(sizeof(NodeQueue));
  Queue->size = 0;
  Queue->maxCapacity = maxCapacity;
  Queue->array = (Node**)malloc(maxCapacity * sizeof(Node));

  return Queue;
}

void printQueue(NodeQueue* Queue) {
  int i = 0;
  while (i < Queue->size) {
    fprintf(stdout, "%c = %d\n", Queue->array[i]->c, Queue->array[i]->freq);
    i++;
  }
}

void printArray(ArrayStruct histogram[ASCIICHARS]) {
  int i;
  for (i = 0; i < ASCIICHARS; i++) {
    if (histogram[i].freq > 0) {
      fprintf(stdout, "%c : %d\n", histogram[i].c, histogram[i].freq);
    }
  }
}
