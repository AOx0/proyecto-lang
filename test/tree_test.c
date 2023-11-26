#include "../src/tree.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int tree_extend_test() {
    int res = 0;

    Tree t1;
    tree_init(&t1, sizeof(uint8_t));

    size_t idx;
    uint8_t *v = (uint8_t *)tree_new_node(&t1, &idx);
    *v = 10;

    for (int i = 0; i < 4; i++) {
        size_t cidx;
        uint8_t *cv = (uint8_t *)tree_new_node(&t1, &cidx);
        *cv = i;

        printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
        tree_new_relation(&t1, idx, cidx);
    }

    Tree t2;
    tree_init(&t2, sizeof(uint8_t));

    v = (uint8_t *)tree_new_node(&t2, &idx);
    *v = 10;

    for (int i = 4; i < 9; i++) {
        size_t cidx;
        uint8_t *cv = (uint8_t *)tree_new_node(&t2, &cidx);
        *cv = i;

        printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
        tree_new_relation(&t2, idx, cidx);
    }

    // tree_extend(&t1, &t2, 0, 2);

    TreeIter ti = tree_iter_new(&t1, 0);

    // while (1) {
    //     TreeIterEntry entry = tree_iter_next(&ti);
    //     if (entry.value == NULL)
    //         break;
    //     printf("%d\n", *(uint8_t *)entry.value);
    // }

    return res;
}

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

    // while (1) {
    //     TreeIterEntry entry = tree_iter_next(&ti);
    //     if (v == NULL)
    //         break;
    //     printf("%d\n", *(uint8_t *)entry.value);
    // }

    puts("DONE");

    ti = tree_iter_new(&t, 9);

    // if (tree_num_child(t, n->id) > 0) {
    //     size_t i = 0;
    //     Vec child = tree_get_childs(t, n->id);
    //     for (size_t i = 0; i<child.len; i++ ) {
    //         Node * v = (Node *)vec_get(&child, i);
    //         node_display_id(v->id, f, t, tabla);
    //     }
    //     vec_drop(&child);
    // }

    tree_drop(&t);

    res |= tree_extend_test();

    return res;
}
