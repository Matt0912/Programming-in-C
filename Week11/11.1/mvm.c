#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mvm.h"

#define MAXSTRSIZE 150

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
    cell->key = key;
    cell->data = data;
    cell->next = m->head;
    m->head = cell;
    (m->numkeys)++;
  }
}

/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m) {
  char *output = (char *)calloc(1, MAXSTRSIZE*sizeof(char));
  char str[MAXSTRSIZE];
  mvmcell* ptr = m->head;
  while (ptr != NULL) {
    sprintf(str, "[%s](%s) ", ptr->key, ptr->data);
    strcat(output, str);
    ptr = ptr->next;
  }
  return output;
}

/* Remove one key/value */
void mvm_delete(mvm* m, char* key) {
  mvmcell* ptr;
  int i = 0;
  if (m != NULL && key != NULL) {
    ptr = m->head;
    /* If head = cell to be deleted */
    if (strcmp(ptr->key, key) == 0) {
      m->head = m->head->next;
      free(ptr);
      (m->numkeys)--;
    }
    /* Else search through list */
    else {
      while (i < m->numkeys - 1) {
        if (strcmp(ptr->next->key, key) == 0) {
          free(ptr->next);
          ptr->next = ptr->next->next;
          (m->numkeys)--;
          i = m->numkeys;
        }
        else {
          ptr = ptr->next;
          i++;
        }
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
  char **argv = (char **)malloc(sizeof(char*));
  mvmcell* ptr = m->head;
  int i = 0, j = 0;
  do {
    if (strcmp(ptr->key, key) == 0) {
      argv[i] = ptr->data;
      (*n)++;
      i++;
    }
    ptr = ptr->next;
    j++;
  } while (j < m->numkeys);
  return argv;
}
/* Free & set p to NULL */
void mvm_free(mvm** p) {
  mvm* m = *p;
  mvmcell* ptr = m->head;
  while (m->head != NULL) {
    m->head = m->head->next;
    free(ptr);
    ptr = m->head;
  }
  *p = NULL;
}
