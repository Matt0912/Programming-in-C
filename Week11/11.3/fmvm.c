#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"

#define DEFAULTSIZE 19
#define MAXSTRSIZE 150

enum bool {FALSE, TRUE};

unsigned long generateHash(char* key);
mvm* resizeTable(mvm* m);
int isPrime(int num);

mvm* mvm_init(void) {
  mvm* map;
  map = (mvm *)calloc(1, sizeof(mvm));
  map->hashTable = (mvmcell*)calloc(DEFAULTSIZE, sizeof(mvmcell*));
  if ((map == NULL) || (map->hashTable == NULL)) {
    ON_ERROR("FAILED TO ALLOCATE MEMORY\n");
  }
  map->numkeys = 0;
  map->tableSize = DEFAULTSIZE;
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
  mvmcell* cell;
  int index;
  if (m != NULL && key != NULL && data != NULL) {
    cell = (mvmcell *)calloc(1, sizeof(mvmcell));
    cell->hash = generateHash(key);
    cell->key = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->key, key);
    cell->data = (char *)malloc(sizeof(char)*MAXSTRSIZE);
    strcpy(cell->data, data);
    index = cell->hash%m->capacity;
    m->hashTable[index] = cell;
    (m->numkeys)++;
    if (m->numkeys > m->capacity*0.8) {
      resizeTable(m);
    }
  }
}

/* djb2 hashing algorithm - taken from http://www.cse.yorku.ca/~oz/hash.html */
unsigned long generateHash(char* key) {
  unsigned long hash = 5381, i = 0, length = strlen(key)-1;
  while (key[i]) {
    hash = ((hash << 5) + hash) + key[i];
    i++;
  }
  return hash;
}

mvm* resizeTable(mvm* m) {
  mvm* newmap;
  int i = 0;
  newmap = (mvm *)calloc(1, sizeof(mvm));
  newmap->hashTable = (mvmcell*)calloc(m->capacity*5, sizeof(mvmcell*));
  if ((newmap == NULL) || (newmap->hashTable == NULL)) {
    ON_ERROR("FAILED TO ALLOCATE MEMORY\n");
  }
  newmap->numkeys = m->numkeys;
  newmap->capacity = m->capacity*5;
  while (!isPrime(newmap->capacity)) {
    (newmap->capacity)++;
  }
  while (m->numkeys > 0) {
    while (i < m->capacity) {
      if (m->hashTable[i] != NULL) {
        newmap[m->hashTable[i]->hash%newmap->capacity] = m->hashTable[i];
        (m->numkeys)--;
      }
      i++;
    }
  }
  mvm_free(&m);
  return newmap;
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
