#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"

int main(void) {
  int i, j;
  mvm *m;
  char* str;
  char** av;
  char animals[5][10] = {"cat",  "dog",  "bird",  "horse", "frog"};
  char  noises[5][10] = {"meow", "bark", "tweet", "neigh", "croak"};

  printf("Basic MVM Tests ... Start\n");
  /* Set up empty array */
  m = mvm_init();
  assert(m != NULL);
  assert(mvm_size(m)==0);


  return 0;
}
