#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "varr.h"

/* Default physical size of array -
   no need to share this */
#define ARRDEFSZ 32

/* Set up array with no elements, but a physical container having
   a default ARRDEFSZ size */
arr* arr_init(int elementsize) {
   arr* l;
   l = (arr*) calloc(1,sizeof(arr));
   if(l == NULL){
      ON_ERROR("Creation of Array Failed\n");
   }
   l->data = calloc(ARRDEFSZ, elementsize);
   if(l->data == NULL){
      ON_ERROR("Creation of Array Failed\n");
   }
   l->pz = ARRDEFSZ;
   l->elsz = elementsize;
   return l;
}

/* No array *written* out-of-bounds, resize */
void arr_set(arr *l, int n, void* v) {
   char* ptr = (char *)l->data;
   if (n < 0){
      ON_ERROR("Array accessed with negative index ...\n");
   }
   /* Index greater than current physical size, expand */
   if(n >= l->pz){
      /* Like malloc, but copies original data, then frees original */
      l->data = realloc(l->data, l->elsz*(n+1));
      if(l->data == NULL){
         ON_ERROR("Resize of Array Failed\n");
      }
      l->pz = n+1;
   }

   ptr = ptr + ((n)*l->elsz);
   *ptr = *v;



}


/* Error when array *read* out-of-bounds, unlike set */
void* arr_get(arr *l, int n) {
   if((n >= l->pz) || (n < 0)){
      ON_ERROR("Array read out of bounds\n");
   }
   return &l->data + (n*l->elsz);
}

/* Pass in the pointer to the arr pointer ,so it can be set to NULL after */
void arr_free(arr **p) {
   arr* a = *p;
   free(a->data);
   free(a);
   /* Helps to assert that the job has been done.*/
   *p = NULL;
}
