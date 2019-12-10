#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mvm.h"

#define MAXSTRSIZE 50
#define MAXOUTPUTSTR 1000
#define LISTSIZE 2

enum bool {FALSE, TRUE};

mvm* mvm_init(void) {
  mvm* list;
  list = (mvm *)calloc(1, sizeof(mvm));
  if (list == NULL) {
    ON_ERROR("LIST CREATION FAILED TO ALLOCATE MEMORY\n");
  }
  list->head = NULL;
  list->numkeys = 0;
  return list;
}

/* Number of key/value pairs stored */
int mvm_size(mvm* m) {
  if (m == NULL) {
    return FALSE;
  }
  return m->numkeys;
}

/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data) {
  mvmcell* cell = (mvmcell *)calloc(1, sizeof(mvmcell));
  if (m == NULL || key == NULL || data == NULL) {
    free(cell);
  }
  else {
    cell->key = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->key, key);
    cell->data = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->data, data);
    cell->next = m->head;
    m->head = cell;
    (m->numkeys)++;
  }
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m) {
  char *output = (char *)calloc(1, MAXOUTPUTSTR*sizeof(char));
  char str[MAXSTRSIZE*3];
  mvmcell* ptr = m->head;
  int i = 1;
  while (ptr != NULL) {
    sprintf(str, "[%s](%s) ", ptr->key, ptr->data);
    if (strlen(output) > MAXOUTPUTSTR*0.8*i) {
      output = (char*)realloc(output, sizeof(char)*MAXOUTPUTSTR*i);
      i++;
    }
    strcat(output, str);
    ptr = ptr->next;
  }
  return output;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key) {
  mvmcell *curr, *prev;
  int i = 0;
  if (m != NULL && key != NULL) {
    curr = m->head;
    prev = NULL;
    /* If head = cell to be deleted */
    if (strcmp(curr->key, key) == 0) {
      m->head = m->head->next;
      free(curr->key);
      free(curr->data);
      free(curr);
      (m->numkeys)--;
    }
    /* Else search through list */
    else {
      while (i < m->numkeys - 1) {
        prev = curr;
        curr = curr->next;
        if (strcmp(curr->key, key) == 0) {
          prev->next = curr->next;
          free(curr->key);
          free(curr->data);
          free(curr);
          (m->numkeys)--;
          i = m->numkeys;
        }
        i++;
      }
    }
  }
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key) {
  mvmcell* ptr = m->head;
  if (ptr == NULL || m == NULL) {
    ON_ERROR("INVALID INPUT");
  }
  while (ptr != NULL) {
    if (strcmp(ptr->key, key) == 0) {
      return ptr->data;
    }
    else {
      ptr = ptr->next;
    }
  }
  return NULL;
}

/* Return *argv[] list of pointers to all values stored
// with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n) {
  char **argv = (char **)calloc(1, LISTSIZE*sizeof(char**));
  mvmcell* ptr = m->head;
  int i = 1, j = 0;
  while (j < m->numkeys) {
    if (strcmp(ptr->key, key) == 0) {
      if (*n > LISTSIZE * i * 0.8) {
        printf("n = %d\n", *n);
        argv = (char **)realloc(argv, sizeof(char**)*LISTSIZE*i);
        i++;
      }
      argv[*n] = ptr->data;
      (*n)++;
    }
    ptr = ptr->next;
    j++;
  }
  return argv;
}

/* Free & set p to NULL */
void mvm_free(mvm** p) {
  mvm* m = *p;
  mvmcell* ptr = m->head;
  while (m->head != NULL) {
    m->head = m->head->next;
    free(ptr->key);
    free(ptr->data);
    free(ptr);
    ptr = m->head;
  }
  free(m);
  *p = NULL;
}
