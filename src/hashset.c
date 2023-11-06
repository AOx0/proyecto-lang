#include "hashset.h"
#include <stdio.h>

#define HASH_BUFF_SIZE (size_t)3500

HashSet hashset_new(size_t val_size, HashFunction hashf) {
    HashSet res;
    res.values = vec_with_cap(sizeof(Vec), HASH_BUFF_SIZE);

    for (size_t i = 0; i < HASH_BUFF_SIZE; i++) {
        Vec *handler = (Vec *)vec_push(&res.values);
        vec_init(handler, val_size);
    }

    res.hashf = hashf;
    res.val_size = val_size;
    res.elements = 0;
    return res;
}

void hashset_debug(HashSet *hs) {
    printf("HashSet { t_size: %zu, elems: %zu, buff: %zu }\n", hs->val_size,
           hs->elements, HASH_BUFF_SIZE);
}

void hashset_debug_verbose(HashSet *hs) {
    printf("HashSet { t_size: %zu, elems: %zu, buff: %zu, keys: ", hs->val_size,
           hs->elements, HASH_BUFF_SIZE);

    for (size_t i = 0; i < hs->values.len; i++) {
        Vec *v = (Vec *)vec_get(&hs->values, i);
        for (size_t j = 0; j < v->len; j++) {
            printf("%zu(%zu, %zu) ", hs->hashf(vec_get(v, j)).idx, i, j);
        }
    }
    puts("}");
}

int hashset_contains(HashSet *hs, void *value) {
    HashIdx idx = hs->hashf(value);
    Vec *handler = (Vec *)vec_get(&hs->values, idx.idx % HASH_BUFF_SIZE);

    printf("Hit %zu\n", idx.idx % HASH_BUFF_SIZE);

    if (handler->len == 0) {
        return 0;
    } else {
        for (size_t i = 0; i < handler->len; i++) {
            void *svalue = vec_get(handler, i);
            HashIdx shash = hs->hashf(svalue);
            if (memcmp(&shash, &idx, sizeof(HashIdx)) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

void hashset_remove(HashSet *hs, void *value) {
    HashIdx idx = hs->hashf(value);
    Vec *handler = (Vec *)vec_get(&hs->values, idx.idx % HASH_BUFF_SIZE);

    printf("Hit %zu\n", idx.idx % HASH_BUFF_SIZE);

    if (handler->len != 0) {
        for (size_t i = 0; i < handler->len; i++) {
            void *svalue = vec_get(handler, i);
            HashIdx shash = hs->hashf(svalue);
            if (memcmp(&shash, &idx, sizeof(HashIdx)) == 0) {
                vec_remove(handler, i);
            }
        }
    }
}

void *hashset_get(HashSet *hs, void *value) {
    HashIdx idx = hs->hashf(value);
    Vec *handler = (Vec *)vec_get(&hs->values, idx.idx % HASH_BUFF_SIZE);

    printf("Hit %zu\n", idx.idx % HASH_BUFF_SIZE);

    if (handler->len == 0) {
        return NULL;
    } else {
        for (size_t i = 0; i < handler->len; i++) {
            void *svalue = vec_get(handler, i);
            HashIdx shash = hs->hashf(svalue);
            if (memcmp(&shash, &idx, sizeof(HashIdx)) == 0) {
                return svalue;
            }
        }
    }

    return NULL;
}

void *hashset_insert(HashSet *hs, void *value) {
    HashIdx idx = hs->hashf(value);
    Vec *handler = (Vec *)vec_get(&hs->values, idx.idx % HASH_BUFF_SIZE);

    printf("Hit %zu\n", idx.idx % HASH_BUFF_SIZE);

    if (handler->len == 0) {
        void *val_handler = vec_push(handler);
        memcpy(val_handler, value, hs->val_size);
        hs->elements += 1;
        return val_handler;
    } else {
        for (size_t i = 0; i < handler->len; i++) {
            void *svalue = vec_get(handler, i);
            HashIdx shash = hs->hashf(svalue);
            if (memcmp(&shash, &idx, sizeof(HashIdx)) == 0) {
                return NULL;
            } else {
                void *val_handler = vec_push(handler);
                memcpy(val_handler, value, hs->val_size);
                hs->elements += 1;
                return val_handler;
            }
        }
    }

    return NULL;
}

void hashset_drop(HashSet *hs) {
    for (size_t i = 0; i < hs->values.len; i++) {
        Vec *v = (Vec *)vec_get(&hs->values, i);
        vec_drop(v);
    }

    vec_drop(&hs->values);
}
