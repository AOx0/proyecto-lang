#ifndef LNG_HASHSET
#define LNG_HASHSET

#define HASH_BUFF_SIZE (size_t)100


#include "vector.h"
#include <string.h>
#include <stddef.h>

struct HashIdx {
        size_t idx;
};

typedef struct HashIdx HashIdx;
typedef HashIdx (*HashFunction)(void *);

struct HashSet {
        Vec values;
        size_t val_size;
        size_t elements;
        HashFunction hashf;
};

typedef struct HashSet HashSet;

HashSet hashset_new(size_t val_size, HashFunction hashf);
void hashset_init(HashSet *hs, size_t val_size, HashFunction hashf);

void hashset_debug_verbose(HashSet *hs);
void hashset_debug(HashSet *hs);
int hashset_contains(HashSet *hs, void *value);
void *hashset_insert(HashSet *hs, void *value);
void hashset_remove(HashSet *hs, void *value);
void *hashset_get(HashSet *hs, void *value);
void hashset_drop(HashSet *hs);

#endif
