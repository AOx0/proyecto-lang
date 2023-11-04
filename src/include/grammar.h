#ifndef LNG_GRAMMAR
#define LNG_GRAMMAR

#include "str.h"
#include "vector.h"

struct Grammar {
        Vec non_terminal;
        Vec start_ptr;
        Vec num_rules;
        Vec nt_sorted;
        Vec rhs;
};
typedef struct Grammar Grammar;

Grammar gr_new(void);

void gr_load(Grammar *g, char *from_text, size_t len);
void gr_drop(Grammar *g);

#endif
