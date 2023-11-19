#include "tree.h"

void *tree_iter_next(TreeIter *ti) {
    if (ti->tree->relations.len == 0)
        return NULL;
    if (ti->parents.len == 0)
        return NULL;

    size_t max_child = ti->tree->relations.len;
    size_t curr = *(size_t *)vec_last(&ti->parents);

    void *pt = vec_get(&ti->tree->values, curr);
    void *temp = &ti->tmp.ptr;
    memcpy(temp, pt, ti->tree->values.t_size);

    vec_pop(&ti->parents);

    // Agregamos todos los hijos
    for (size_t child = max_child - 1; child >= 0; child--) {
        TreeEntry *te = (TreeEntry *)vec_get(&ti->tree->relations, child);

        if (curr == te->from)
            *(size_t *)vec_push(&ti->parents) = te->to;

        if (child == 0)
            break;
    }

    return temp;
}

TreeIter tree_iter_new(Tree *t, size_t root) {
    TreeIter res;
    res.parents = vec_new(sizeof(size_t));
    size_t *rootn = (size_t *)vec_push(&res.parents);
    *rootn = root;
    res.tree = t;
    res.tmp = vec_with_cap(t->values.t_size, 1);
    return res;
}

void tree_init(Tree *t, size_t t_size) {
    vec_init(&t->relations, sizeof(TreeEntry));
    vec_init(&t->values, t_size);
    size_t curr = 0;
    size_t len = 0;
}

void *tree_new_node(Tree *t, size_t *self_idx) {
    *self_idx = t->values.len;
    void *node = vec_push(&t->values);
    return node;
}

void tree_iter_drop(TreeIter *ti) {
    vec_drop(&ti->parents);
    vec_drop(&ti->tmp);
}

void *tree_last_node(Tree *t, size_t *self_idx) {
    *self_idx = t->values.len - 1;
    return vec_get(&t->values, *self_idx);
}

TreeEntry *tree_new_relation(Tree *t, size_t from, size_t to) {
    TreeEntry *te = (TreeEntry *)vec_push(&t->relations);
    te->from = from;
    te->to = to;
    return te;
}

void tree_drop(Tree *t) {
    vec_drop(&t->relations);
    vec_drop(&t->values);
}
