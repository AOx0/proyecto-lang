#include "../src/hashset.h"

int hash_contains(void);

int test_hashset(void) {
    int res = 0;

    printf("%-30s", "SET::hash_contains");
    res |= hash_contains();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    return res;
}

HashIdx hash_num(void *num) {
    HashIdx idx;
    idx.idx = *(size_t *)num;
    return idx;
}

int hash_contains(void) {
    int res = 0;
    size_t n = 0;

    HashSet set = hashset_new(sizeof(size_t), hash_num);

    hashset_insert(&set, &n);
    n = 1;
    hashset_insert(&set, &n);
    n = 4;
    hashset_insert(&set, &n);
    n = 3;
    hashset_insert(&set, &n);
    n = 99;
    hashset_insert(&set, &n);
    n = 13;
    hashset_insert(&set, &n);
    n = 23;
    hashset_insert(&set, &n);

    n = 0;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 1;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 4;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 3;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 99;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 13;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 23;
    if (!hashset_contains(&set, &n))
        res = 1;
    n = 2;
    if (hashset_contains(&set, &n))
        res = 1;
    n = 222;
    if (hashset_contains(&set, &n))
        res = 1;
    n = 232434;
    if (hashset_contains(&set, &n))
        res = 1;
    n = 8;
    if (hashset_contains(&set, &n))
        res = 1;

    hashset_drop(&set);

    return res;
}
