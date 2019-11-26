#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define STRSIZE 5000

typedef struct node {
  char c;
  struct node *left;
  struct node *right;
} Node;

Node *MakeNode(char c);
void InsertRandom(Node *t, Node *n);
void findDepth(Node *head, int currDepth, int *maxDepth);
char *PrintTree(Node *t);

int main(void)
{
  char c;
  Node *head = MakeNode('A');
  Node *n;
  int maxDepth = 0;
  srand(time(NULL));
  for (c = 'B'; c < 'G'; c++){
    n = MakeNode(c);
    InsertRandom(head, n);
  }
  findDepth(head, 1, &maxDepth);
  printf("%s\n", PrintTree(head));
  fprintf(stdout, "Max depth = %d\n", maxDepth);
  return 0;
}

Node *MakeNode(char c)
{
  Node *n = (Node *)calloc(1, sizeof(Node));
  assert(n != NULL);
  n->c = c;
  return n;
}

void InsertRandom(Node *t, Node *n)
{
  /* Left */
  if((rand()%2) == 0){
    if(t->left == NULL){
      t->left = n;
    }
    else {
    InsertRandom(t->left, n);
    }
  }
  /* Right */
  else {
    if(t->right == NULL) {
      t->right = n;
    }
    else {
    InsertRandom(t->right, n);
    }
  }
}

void findDepth(Node *head, int currDepth, int *maxDepth) {
  if(head->left != NULL){
    head = head->left;
    findDepth(head, currDepth+1, maxDepth);
  }
  else if (head->right != NULL) {
    head = head->right;
    findDepth(head, currDepth+1, maxDepth);
  }
  else {
    if (currDepth > *maxDepth) {
      *maxDepth = currDepth;
    }
  }
}

char *PrintTree(Node *t)
{
  char *str;
  /* Assign strsize amount of memory, check isn't null */
  assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
  if(t == NULL){
    strcpy(str, "*");
    return str;
  }
  sprintf(str, "%c (%s) (%s)", t->c, PrintTree(t->left), PrintTree(t->right));
  return str;
}
