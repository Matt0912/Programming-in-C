#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fmvm.h"
void test(void);

int main(void) {
  int i, j;
  mvm *m;
  char* str;
  char** av;
  char animals[5][10] = {"cat",  "dog",  "bird",  "horse", "frog"};
  char noises[5][10] = {"meow", "bark", "tweet", "neigh", "croak"};

  printf("Basic MVM Tests ... Start\n");
  /* Set up empty array */
  m = mvm_init();
  assert(m != NULL);
  assert(mvm_size(m)==0);

  for(j=0; j<5; j++){
     mvm_insert(m, animals[j], noises[j]);
     assert(mvm_size(m)==j+1);
     i = strcmp(mvm_search(m, animals[j]), noises[j]);
     assert(i==0);
  }

  /* Test building & printing */
  str = mvm_print(m);
  /* inserted in this order when DEFAULTSIZE = 19 */
  i = strcmp(str, "[bird](tweet) [dog](bark) [cat](meow) [frog](croak) [horse](neigh) ");
  assert(i==0);
  free(str);

  /* Search for non-existent key */
  /* No-one knows what the fox says ? */
  assert(mvm_search(m, "fox") == NULL);

  /* Deletions - middle, then front */
  mvm_delete(m, "dog");
  assert(mvm_size(m)==4);
  str = mvm_print(m);
  i = strcmp(str, "[bird](tweet) [cat](meow) [frog](croak) [horse](neigh) ");
  assert(i==0);
  free(str);

  mvm_insert(m, "dog", "bark");
  assert(mvm_size(m)==5);
  str = mvm_print(m);
  i = strcmp(str, "[bird](tweet) [dog](bark) [cat](meow) [frog](croak) [horse](neigh) ");
  assert(i==0);
  free(str);
  mvm_delete(m, "dog");

  mvm_delete(m, "frog");
  assert(mvm_size(m)==3);
  str = mvm_print(m);
  i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) ");
  assert(i==0);
  free(str);

  /* Insert Multiple Keys */
  mvm_insert(m, "frog", "croak");
  mvm_insert(m, "frog", "ribbit");
  assert(mvm_size(m)==5);
  str = mvm_print(m);
  i = strcmp(str, "[bird](tweet) [cat](meow) [frog](croak) -> [frog](ribbit) [horse](neigh) ");
  free(str);
  assert(i==0);

  /* Search Multiple Keys */
  str = mvm_search(m, "frog");
  i = strcmp(str, "croak");
  assert(i==0);

  /* Multisearching */
  av = mvm_multisearch(m, "cat", &i);
  assert(i==1);
  i = strcmp(av[0], "meow");
  assert(i==0);
  free(av);
  av = mvm_multisearch(m, "horse", &i);
  assert(i==1);
  i = strcmp(av[0], "neigh");
  assert(i==0);
  free(av);
  av = mvm_multisearch(m, "frog", &i);
  assert(i==2);
  i = strcmp(av[0], "croak");
  j = strcmp(av[1], "ribbit");
  assert((i==0)&&(j==0));
  free(av);

  /* Delete Multiple Keys */
  /* Deleting single key now deletes all values associated with that key */
  mvm_delete(m, "frog");
  assert(mvm_size(m)==3);
  str = mvm_print(m);
  i = strcmp(str, "[bird](tweet) [cat](meow) [horse](neigh) ");
  assert(i==0);
  free(str);

  /* Weird NULL insert() edge cases */
  mvm_insert(m, NULL, "quack");
  assert(mvm_size(m)==3);
  mvm_insert(NULL, "duck", "quack");
  assert(mvm_size(m)==3);
  mvm_insert(m, "duck", NULL);
  assert(mvm_size(m)==3);

  /* Weird NULL delete() edge cases */
  mvm_delete(m, "");
  assert(mvm_size(m)==3);
  mvm_delete(m, NULL);
  assert(mvm_size(m)==3);
  mvm_delete(NULL, "frog");
  assert(mvm_size(m)==3);
  mvm_delete(m, "bird");
  assert(mvm_size(m)==2);
  str = mvm_print(m);
  i = strcmp(str, "[cat](meow) [horse](neigh) ");
  assert(i==0);
  free(str);

  /* Freeing */
  mvm_free(&m);
  assert(m==NULL);
  assert(mvm_size(m)==0);

  printf("Basic MVM Tests ... Stop\n");

  printf("Checking Test Function ... Start\n");
  test();
  printf("Checking Test Function ... Stop\n");

  return 0;
}

void test(void) {
  mvm* m;
  int j;
  char animals[10][10] = {"cat",  "dog",  "bird",  "horse", "frog",
                         "cow", "cat", "mouse", "pig", "frog"};
  char noises[10][10] = {"meow", "bark", "tweet", "neigh", "croak",
                         "moo", "purr", "squeak", "oink", "ribbit"};
  m = mvm_init();

  /* Test table resize */
  m->capacity = 2;
  mvm_insert(m, animals[0], noises[0]);
  mvm_insert(m, animals[1], noises[1]);

  for(j=2; j<10; j++){
    mvm_insert(m, animals[j], noises[j]);
    assert(mvm_size(m)==j+1);
  }

  mvm_free(&m);
}
