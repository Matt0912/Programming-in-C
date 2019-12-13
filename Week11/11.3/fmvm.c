#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"

#define DEFAULTSIZE 19
#define MAXSTRSIZE 150
#define MAXOUTPUTSTR 500
#define LISTSIZE 100

enum bool {FALSE, TRUE};

/* Write comments at top of fmvm.h*/

unsigned long generateHash(char* key);
void resizeTable(mvm* m);
int isPrime(int num);
void freeCell(mvmcell** ptr);

mvm* mvm_init(void) {
  mvm* map;
  map = (mvm *)calloc(1, sizeof(mvm));
  map->hashTable = (mvmcell**)calloc(DEFAULTSIZE, sizeof(mvmcell*));
  if ((map == NULL) || (map->hashTable == NULL)) {
    ON_ERROR("FAILED TO ALLOCATE MEMORY\n");
  }
  map->numkeys = 0;
  map->capacity = DEFAULTSIZE;
  return map;
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
  mvmcell* cell, *ptr;
  int index;
  if (m != NULL && key != NULL && data != NULL) {
    cell = (mvmcell *)calloc(1, sizeof(mvmcell));
    cell->key = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->key, key);
    cell->data = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->data, data);
    cell->link = NULL;
    index = generateHash(key)%m->capacity;

    if (m->hashTable[index] != NULL) {
      ptr = m->hashTable[index];
      while (ptr->link != NULL) {
        ptr = ptr->link;
      }
      ptr->link = cell;
    }
    else {
      m->hashTable[index] = cell;
    }

    (m->numkeys)++;
    if (m->numkeys > m->capacity*0.8) {
      resizeTable(m);
    }
  }
}

char* mvm_print(mvm* m) {
  char *output = (char *)calloc(1, MAXOUTPUTSTR*sizeof(char));
  char str[MAXSTRSIZE*3];
  mvmcell* ptr;
  int i = 1, count = 0;
  while (count < m->capacity) {
    ptr = m->hashTable[count];
    while (strlen(output) > MAXOUTPUTSTR*0.8*i) {
      i++;
      output = (char*)realloc(output, sizeof(char)*MAXOUTPUTSTR*i);
    }
    if (ptr != NULL) {
      sprintf(str, "[%s](%s) ", ptr->key, ptr->data);
      strcat(output, str);
      while (ptr->link != NULL) {
        sprintf(str, "-> [%s](%s) ", ptr->link->key, ptr->link->data);
        strcat(output, str);
        ptr = ptr->link;
      }
    }
    count++;
  }
  return output;
}

/* Decided to go through linked list and delete all data associated with key */
void mvm_delete(mvm* m, char* key) {
  int index;
  mvmcell* ptr, *ptrPrev;
  if (m != NULL && key != NULL) {
    index = generateHash(key)%m->capacity;
    ptr = m->hashTable[index];
    if (ptr != NULL) {
      if (strcmp(key, ptr->key) == 0) {
        while (ptr->link != NULL) {
          ptrPrev = ptr;
          ptr = ptr->link;
          freeCell(&ptrPrev);
          (m->numkeys)--;
        }
        freeCell(&ptr);
        m->hashTable[index] = NULL;
        (m->numkeys)--;
      }
    }
  }
}

/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key) {
  int index;
  if (m == NULL || key == NULL) {
    ON_ERROR("INVALID SEARCH INPUT\n");
  }
  index = generateHash(key)%m->capacity;
  if (m->hashTable[index] != NULL) {
    if (strcmp(key, m->hashTable[index]->key) == 0) {
      return m->hashTable[index]->data;
    }
  }
  return NULL;
}

/* Return *argv[] list of pointers to all values stored
// with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n) {
  char **argv = (char **)calloc(1, LISTSIZE*sizeof(char*));
  mvmcell* ptr;
  int i = 1, index = generateHash(key)%m->capacity;
  if (strcmp(m->hashTable[index]->key, key) == 0) {
    ptr = m->hashTable[index];
    argv[*n] = ptr->data;
    (*n)++;
    while (ptr->link != NULL) {
      if ((*n) > (LISTSIZE * i * 0.8)) {
        i++;
        argv = (char **)realloc(argv, sizeof(char**)*LISTSIZE*i);
      }
      if (strcmp(ptr->link->key, key) == 0) {
        argv[*n] = ptr->link->data;
        (*n)++;
      }
      ptr = ptr->link;
    }
  }
  return argv;
}

/* Free & set p to NULL */
void mvm_free(mvm** p) {
  mvm* m = *p;
  mvmcell* ptr, *ptrPrev;
  int count = 0;
  while (count < m->capacity) {
    ptr = m->hashTable[count];
    if (ptr != NULL) {
      while (ptr->link != NULL) {
        ptrPrev = ptr;
        ptr = ptr->link;
        freeCell(&ptrPrev);
      }
      freeCell(&ptr);
    }
    m->hashTable[count] = NULL;
    count++;
  }
  ptr = NULL;
  free(m->hashTable);
  free(m);
  *p = NULL;
}

/* djb2 hashing algorithm - taken from http://www.cse.yorku.ca/~oz/hash.html */
unsigned long generateHash(char* key) {
  unsigned long hash = 5381, i = 0;
  while (key[i]) {
    hash = ((hash << 5) + hash) + key[i];
    i++;
  }
  return hash;
}

void resizeTable(mvm* m) {
  mvmcell** newHashTable;
  int i = 0, newIndex, newCapacity;
  mvmcell* ptr, *ptr2, *prevPtr;
  newCapacity = m->capacity*5;
  while (!isPrime(newCapacity)) {
    (newCapacity)++;
  }
  newHashTable = (mvmcell**)calloc(newCapacity, sizeof(mvmcell*));
  if (newHashTable == NULL) {
    ON_ERROR("FAILED TO ALLOCATE MEMORY\n");
  }
  while (i < m->capacity) {
    ptr = m->hashTable[i];
    if (ptr != NULL) {
      newIndex = generateHash(ptr->key)%newCapacity;
      newHashTable[newIndex] = ptr;
      while (ptr->link != NULL) {
        prevPtr = ptr;
        ptr = ptr->link;
        prevPtr->link = NULL;
        newIndex = generateHash(ptr->key)%newCapacity;
        if (newHashTable[newIndex] != NULL) {
          ptr2 = newHashTable[newIndex];
          while (ptr2->link != NULL) {
            ptr2 = ptr2->link;
          }
          ptr2->link = ptr;
        }
        else {
          newHashTable[newIndex] = ptr;
        }
      }
      m->hashTable[i] = NULL;
    }
    i++;
  }

  m->capacity = newCapacity;
  free(m->hashTable);
  m->hashTable = newHashTable;

}

void freeCell(mvmcell** ptr) {
  mvmcell* cell = *ptr;
  free(cell->key);
  free(cell->data);
  free(cell);
  *ptr = NULL;
}

int isPrime(int num) {
  int factor = 2;
  if (num < 2) {
    return FALSE;
  }
  while (factor < num/2) {
    if (num % factor == 0) {
      return FALSE;
    }
    else {
      factor++;
    }
  }
  return TRUE;
}
