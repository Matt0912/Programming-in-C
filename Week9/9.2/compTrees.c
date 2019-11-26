#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define STRSIZE 5000

enum bool {FALSE, TRUE};

typedef struct node {
  char c;
  struct node *left;
  struct node *right;
} Node;

Node *MakeNode(char c);
void InsertRandom(Node *t, Node *n);
void findDepth(Node *head, int currDepth, int *maxDepth);
int compareTrees(Node *head1, Node *head2, int *count);
char *PrintTree(Node *t);

int main(void)
{
  char c;
  Node *head1 = MakeNode('A');
  Node *head2 = MakeNode('A');
  Node *n1, *n2;
  int maxDepth1 = 0, maxDepth2 = 0, count = 0;
  srand(time(NULL));
  for (c = 'B'; c < 'G'; c++){
    n1 = MakeNode(c);
    n2 = MakeNode(c);
    InsertRandom(head1, n1);
    InsertRandom(head2, n2);
  }
  findDepth(head1, 1, &maxDepth1);
  findDepth(head2, 1, &maxDepth2);
  printf("%s\n", PrintTree(head1));
  printf("%s\n", PrintTree(head2));

  if (maxDepth1 == maxDepth2) {
    if (compareTrees(head1, head2, &count)) {
      fprintf(stdout, "TREES ARE IDENTICAL\n");
    }
    else {
      fprintf(stdout, "TREES ARE NOT IDENTICAL\n");
    }
  }
  else {
    fprintf(stdout, "TREES ARE NOT IDENTICAL\n");
  }

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
    findDepth(head->left, currDepth+1, maxDepth);
  }
  if (head->right != NULL) {
    findDepth(head->right, currDepth+1, maxDepth);
  }
  if (currDepth > *maxDepth) {
    *maxDepth = currDepth;
  }
}

int compareTrees(Node *head1, Node *head2, int *count) {
  if (*count > 0) {
    return FALSE;
  }

  if (head1 == head2 && head2 == NULL) {
    return TRUE;
  } else if (head1 == NULL || head2 == NULL){
    (*count)++;
    return FALSE;
  }

  if (head1->c == head2->c) {
    compareTrees(head1->left, head2->left, count);
    compareTrees(head1->right, head2->right, count);
  }
  else {
    (*count)++;
    return FALSE;
  }

  if (*count == 0) {
    return TRUE;
  } else {
    return FALSE;
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
