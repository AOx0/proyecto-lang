#ifndef LNG_TREEH
#define LNG_TREEH

#include "panic.h"
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
        size_t level;
        Vec parents;
        Vec tmp;
};
typedef struct TreeIter TreeIter;

void tree_relations_debug(Tree *t);
void *tree_iter_next(TreeIter *ti);
int tree_iter_has_next(TreeIter *ti);
void tree_extend(Tree *t, Tree *o, size_t childs_of);
Vec tree_get_childs(Tree *t, size_t parent_id);
TreeIter tree_iter_new(Tree *t, size_t root);
size_t tree_num_child(Tree *t, size_t root);
void tree_iter_drop(TreeIter *ti);

void tree_init(Tree *t, size_t t_size);
void *tree_new_node(Tree *t, size_t *self_idx);
void *tree_last_node(Tree *t, size_t *self_idx);
void tree_drop(Tree *t);

TreeEntry *tree_new_relation(Tree *t, size_t from, size_t to);
TreeEntry *tree_new_rel(Tree *t, void *from, void *to);

#endif
