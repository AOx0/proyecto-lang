#include "../src/vector.h"
#include "../src/hashset.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


extern HashIdx hash_num(void *);

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

void tree_init(Tree *t, size_t t_size) {
  vec_init(&t->relations, sizeof(TreeEntry));
  vec_init(&t->values, t_size);
  size_t curr = 0;
  size_t len = 0;
}

void *tree_new_node(Tree *t, size_t * self_idx) {
  *self_idx = t->values.len;
  void * node = vec_push(&t->values);
  return node;
}

void *tree_last_node(Tree *t, size_t * self_idx) {
  *self_idx = t->values.len - 1;
  return vec_get(&t->values, *self_idx);
}

TreeEntry *tree_new_relation(Tree *t, size_t from, size_t to) {
  TreeEntry * te = (TreeEntry *)vec_push(&t->relations);
  te->from = from;
  te->to = to;
  return te;
}

// Add for
// Add i = 0 in for
// add i < 9 in for
// add i++ in for
// 

void tree_drop(Tree *t) {
  vec_drop(&t->relations);
  vec_drop(&t->values);
}

int test_tree() {
  int res = 0;

  Tree t;
 
  tree_init(&t, sizeof(uint8_t));

  size_t idx;
  uint8_t * v = (uint8_t *)tree_new_node(&t, &idx);
  *v = 10;

  for (int i= 0; i<4; i++) {
    size_t cidx;
    uint8_t * cv = (uint8_t *)tree_new_node(&t, &cidx);
    *cv = i;
  
    printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
    tree_new_relation(&t, idx, cidx);

    vec_debug_verbose(&t.values);
    vec_debug_verbose(&t.relations);
  }

  v = (uint8_t *)tree_last_node(&t, &idx);

  for (int i= 4; i<9; i++) {
    size_t cidx;
    uint8_t * cv = (uint8_t *)tree_new_node(&t, &cidx);
    *cv = i;
  
    printf("Nuevo de ID %zu a %zu con valor %d\n", idx, cidx, i);
    tree_new_relation(&t, idx, cidx);

    vec_debug_verbose(&t.values);
    vec_debug_verbose(&t.relations);
  }

  for (size_t i= 0; i < t.values.len; i++) {
    uint8_t * pt = (uint8_t *)vec_get(&t.values, i);

    for (size_t j=0; j < t.relations.len; j++) {
  
      TreeEntry * te = (TreeEntry *)vec_get(&t.relations, j);
      uint8_t * ct = (uint8_t *)vec_get(&t.values, te->to);

      if (i == te->from) {
        printf("VALU: (%zu -> %zu) : (%d, %d)\n", te->from, te->to, *pt, *ct);
      }
    }
  }

  tree_drop(&t);

  return res;
}
