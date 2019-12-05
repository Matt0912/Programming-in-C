#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "arr.h"

enum bool {FALSE, TRUE};

set* set_init(void) {
  set* s;
  s = (set*) calloc(1,sizeof(set));
  if(s == NULL){
     ON_ERROR("Set Creation Failed\n");
  }
  s->sz = 0;
  s->ua = arr_init();

  return s;
}

set* set_copy(set* s) {
  set* s_copy = set_init();
  s_copy->sz = s->sz;
  s_copy->ua = s->ua;
  return s_copy;
}

/* Create new set, copied from an array of length n*/
set* set_fromarray(arrtype* a, int n) {
  set* s = set_init();

  return s;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l) {
  if (!set_contains(s,l)) {
    arr_set(set->ua, set->sz, l);
    set->sz = set->sz + 1;
  }
}

/* Return size of the set */
int set_size(set* s) {
  return s->sz;
}
/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l) {
  int i;
  for (i = 0; i < s->sz; i++) {
    if (s->ua->data[i] == l) {
      return TRUE;
    }
  }
  return FALSE;
}
/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l) {
  int i=0;
  if (set_contains(s,l)) {
    while (s->ua->data[i] != l) {
      i++;
    }
    while (i < s->sz - 1) {
      s->ua->data[i] = s->ua->data[i+1];
      i++;
    }
    (s->sz)--;
  }
}

/* Remove one element from the set - there's no
   particular order for the elements, so any will do */
arrtype set_removeone(set* s) {
  arrtype k = s->ua->data[s->sz-1];
  (s->sz)--;
  return k;
}

/* Operations on 2 sets */
/* Create a new set, containing all elements from s1 & s2 */
set* set_union(set* s1, set* s2) {
  set* union = set_init();
  int i;
  for (i = 0; i < s1->sz; i++) {
    set_insert(union, s1->ua->data[i]);
  }
  for (i = 0; i < s2->sz; i++) {
    set_insert(union, s2->ua->data[i]);
  }
  return union;
}
/* Create a new set, containing all elements in both s1 & s2 */
set* set_intersection(set* s1, set* s2) {
  set* intersection = set_init();
  int i, minSize;
  if (s1->sz > s2->sz) {
    minSize = s2->sz;
  }
  else {
    minSize = s1->sz;
  }
  for (i = 0; i < minSize; i++) {
    if (set_contains(s2, s1->ua->data[i])) {
      set_insert(intersection, s1->ua->data[i]);
    }
  }

  return intersection;
}

/* Finish up */
/* Clears all space used, and sets pointer to NULL */
void set_free(set** s) {
  *s->sz = 0;
}
