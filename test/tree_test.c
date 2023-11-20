#include "../src/tree.h"
#include <stdio.h>
#include <string.h>

int test_tree() {
    int res = 0;

    Tree t;

    tree_init(&t, sizeof(uint8_t));

    size_t idx;
    uint8_t *v = (uint8_t *)tree_new_node(&t, &idx);
    *v = 10;

    for (int i = 0; i < 4; i++) {
        size_t cidx;
        uint8_t *cv = (uint8_t *)tree_new_node(&t, &cidx);
        *cv = i;

        printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
        tree_new_relation(&t, idx, cidx);

        // vec_debug_verbose(&t.values);
        // vec_debug_verbose(&t.relations);
    }

    v = (uint8_t *)tree_last_node(&t, &idx);

    for (int i = 4; i < 9; i++) {
        size_t cidx;
        uint8_t *cv = (uint8_t *)tree_new_node(&t, &cidx);
        *cv = i;

        printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
        tree_new_relation(&t, idx, cidx);

        // vec_debug_verbose(&t.values);
        // vec_debug_verbose(&t.relations);
    }

    for (size_t i = 0; i < t.values.len; i++) {
        uint8_t *pt = (uint8_t *)vec_get(&t.values, i);

        for (size_t j = 0; j < t.relations.len; j++) {

            TreeEntry *te = (TreeEntry *)vec_get(&t.relations, j);
            uint8_t *ct = (uint8_t *)vec_get(&t.values, te->to);

            if (i == te->from) {
                printf("VALU: (%zu -> %zu) : (%d, %d)\n", te->from, te->to, *pt,
                       *ct);
            }
        }
    }

    TreeIter ti = tree_iter_new(&t, 0);

    while (1) {
        uint8_t *v = tree_iter_next(&ti);
        if (v == NULL)
            break;
        printf("%d\n", *v);
    }

    puts("DONE");

    ti = tree_iter_new(&t, 9);

    while (1) {
        uint8_t *v = tree_iter_next_child(&ti, 0);
        if (v == NULL)
            break;
        printf("%d\n", *v);
    }

    tree_drop(&t);

    return res;
}
