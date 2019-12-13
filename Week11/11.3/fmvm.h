/* Attempted to make homophones searching faster through the use of hash tables
// Used each key to generate an index in the hash table (using the hash function
// djb2), and dealt with any collisions by implementing a linked list for any
// keys with the same index.

// I re-allocated memory for the hash table as more values were input, which
// resulted in making a function to generate a new hash table, populate it with
// the existing values using their new hashes, and freeing the old table

// Because each index could have keys which were not identical, when printing
// values I had to use a strcmp function so that only relevant values were printed

// In reality, fmvm.c is slower than mvm.c, but this is due to constant re-allocation
// of memory, and I also believe that djb2 is a slow hashing function as it ran
// faster before implementing it

// If I had more time, I'd investigate double hashing so that each linked list only
// contains values with identical keys */

/* Error that can't be ignored */
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

struct mvmcell {
   char* key;
   char* data;
   struct mvmcell* link;
};
typedef struct mvmcell mvmcell;

struct mvm {
   mvmcell** hashTable;
   int numkeys;
   int capacity;
};
typedef struct mvm mvm;

mvm* mvm_init(void);
/* Number of key/value pairs stored */
int mvm_size(mvm* m);
/* Insert one key/value pair */
void mvm_insert(mvm* m, char* key, char* data);
/* Store list as a string "[key](value) [key](value) " etc.  */
char* mvm_print(mvm* m);
/* Remove one key/value */
void mvm_delete(mvm* m, char* key);
/* Return the corresponding value for a key */
char* mvm_search(mvm* m, char* key);
/* Return *argv[] list of pointers to all values stored with key, n is the number of values */
char** mvm_multisearch(mvm* m, char* key, int* n);
/* Free & set p to NULL */
void mvm_free(mvm** p);
