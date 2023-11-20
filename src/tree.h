#ifndef LNG_TREEH
#define LNG_TREEH

#include "vector.h"
#include <stdint.h>

struct Tree {
        Vec relations;
        Vec values;
};
typedef struct Tree Tree;

struct TreeEntry {
        size_t from;
        size_t to;
};
typedef struct TreeEntry TreeEntry;

struct TreeIter {
        Tree *tree;
        Vec parents;
        Vec tmp;
};
typedef struct TreeIter TreeIter;

void *tree_iter_next(TreeIter *ti);
void *tree_iter_next_child(TreeIter *ti, size_t parent_id);
TreeIter tree_iter_new(Tree *t, size_t root);
void tree_iter_drop(TreeIter *ti);

void tree_init(Tree *t, size_t t_size);
void *tree_new_node(Tree *t, size_t *self_idx);
void *tree_last_node(Tree *t, size_t *self_idx);
void tree_drop(Tree *t);

TreeEntry *tree_new_relation(Tree *t, size_t from, size_t to);
TreeEntry *tree_new_rel(Tree *t, void *from, void *to);

#endif
