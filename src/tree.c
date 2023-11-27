#include "tree.h"
#include "hashset.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int tree_iter_has_next(TreeIter *ti) { return ti->remaning.len != 0; }

TreeIterEntry tree_iter_next(TreeIter *ti) {
    if (ti->remaning.len == 0)
        return (TreeIterEntry){
            .level = 0, .did_set = 0, .parent = 0, .child = 0, .value = NULL};

    size_t max_child = ti->tree->relations.len;
    TreeIterEntry top = *(TreeIterEntry *)vec_pop(&ti->remaning);

    if (ti->tree->relations.len != 0) {
        ti->level += 1;
        int did_add = 0;
        // Agregamos todos los hijos
        for (size_t child = max_child - 1; child >= 0; child--) {
            TreeEntry *te = (TreeEntry *)vec_get(&ti->tree->relations, child);

            if (top.child == te->from) {
                did_add = 1;
                *(TreeIterEntry *)vec_push(&ti->remaning) = (TreeIterEntry){
                    .level = ti->level,
                    .did_set = 1,
                    .parent = top.child,
                    .child = te->to,
                    .value = vec_get(&ti->tree->values, te->to)};
            }

            if (child == 0)
                break;
        }

        if (!did_add) {
            ti->level -= 1;
        }
    } else {
        ti->level = 0;
    }

    return top;
}

size_t tree_num_child(Tree *t, size_t root) {
    size_t num = 0;
    if (t->relations.len != 0) {
        // Agregamos todos los hijos
        for (size_t child = t->relations.len - 1; child >= 0; child--) {
            TreeEntry *te = (TreeEntry *)vec_get(&t->relations, child);

            if (root == te->from)
                num++;

            if (child == 0)
                break;
        }
    }

    return num;
}

Vec tree_get_children(Tree *t, size_t parent_id) {
    Vec res = vec_new(sizeof(size_t));
    vec_init(&res, sizeof(size_t));

    if (t->relations.len == 0 || parent_id >= t->values.len ||
        tree_num_child(t, 0) == 0)
        return res;

    size_t nchild = tree_num_child(t, parent_id);

    if (nchild > 0) {
        vec_init_with_cap(&res, sizeof(size_t), nchild);
    } else {
        vec_init(&res, sizeof(size_t));
    }

    if (t->relations.len >= 0) {
        for (size_t i = 0; i < t->relations.len; i++) {
            size_t child = t->relations.len - i - 1;
            TreeEntry *te = (TreeEntry *)vec_get(&t->relations, child);

            if (parent_id == te->from) {
                size_t *val = (size_t *)vec_push(&res);
                *val = te->to;
            }
        }
    }

    return res;
}

TreeIter tree_iter_new(Tree *t, size_t root) {
    TreeIter res;
    res.remaning = vec_new(sizeof(TreeIterEntry));
    TreeIterEntry *rootn = (TreeIterEntry *)vec_push(&res.remaning);
    res.level = 1;
    *rootn = (TreeIterEntry){.level = 0,
                             .did_set = 1,
                             .parent = 0,
                             .child = root,
                             .value = vec_get(&t->values, root)};
    res.tree = t;
    return res;
}

void tree_init(Tree *t, size_t t_size) {
    vec_init(&t->relations, sizeof(TreeEntry));
    vec_init(&t->values, t_size);
}

void *tree_new_node(Tree *t, size_t *self_idx) {
    if (self_idx != NULL)
        *self_idx = t->values.len;
    void *node = vec_push(&t->values);
    return node;
}

void tree_iter_drop(TreeIter *ti) { vec_drop(&ti->remaning); }

void *tree_last_node(Tree *t, size_t *self_idx) {
    *self_idx = t->values.len - 1;
    return vec_get(&t->values, *self_idx);
}

TreeEntry *tree_new_relation(Tree *t, size_t from, size_t to) {
    TreeEntry *te = (TreeEntry *)vec_push(&t->relations);

    if (from == to)
        panic("Trying to create a relation from %zu to %zu", from, to);

    te->from = from;
    te->to = to;
    return te;
}

void tree_relations_debug(Tree *t) {
    printf("Relations:\n");
    for (size_t i = 0; i < t->relations.len; i++) {
        TreeEntry *te = (TreeEntry *)vec_get(&t->relations, i);
        printf("%zu:%zu -> %zu\n", i, te->from, te->to);
    }
}

TreeEntry *tree_new_rel(Tree *t, void *from, void *to) {
    size_t from_idx = 0;
    size_t to_idx = 0;
    int found_from_idx = 0;
    int found_to_idx = 0;

    for (size_t i = 0; i < t->values.len; i++) {
        if (vec_get(&t->values, i) == from) {
            from_idx = i;
            found_from_idx = 1;
            break;
        }
    }
    for (size_t i = 0; i < t->values.len; i++) {
        if (vec_get(&t->values, i) == to) {
            to_idx = i;
            found_to_idx = 1;
            break;
        }
    }

    if (!found_from_idx) {
        panic("Not existing `from` element %p", from);
    }
    if (!found_to_idx) {
        panic("Not existing `to` element %p", to);
    }

    TreeEntry *te = (TreeEntry *)vec_push(&t->relations);
    te->from = from_idx;
    te->to = to_idx;
    return te;
}

void tree_drop(Tree *t) {
    vec_drop(&t->relations);
    vec_drop(&t->values);
}
