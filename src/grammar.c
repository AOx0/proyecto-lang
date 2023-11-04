#include "include/grammar.h"
#include <stdio.h>
#include <stdlib.h>

Grammar gr_new(void) {
    Grammar res;
    
    res.non_terminal = vec_new(sizeof(String));
    res.start_ptr = vec_new(sizeof(size_t));
    res.num_rules = vec_new(sizeof(size_t));
    res.nt_sorted = vec_new(sizeof(size_t));
    res.rhs = vec_new(sizeof(String));
    
    return res;
}


void gr_load(Grammar *g, char * from_text, size_t len) {
    StrSlice text = str_slice_from_cstr(from_text, len);
    
    StrIter lines = str_iter_from_slice(&text, "\n");
    
    size_t n_line = 0;
    while (!lines.ended) {
        n_line++;
        StrSlice line = str_iter_next(&lines);
        
        if (line.len == 0) continue;
        
        StrSliceDupla dp = str_slice_split_once(&line, " ");
        
        if (!dp.did_perform) {
            printf("Panic: Invalid rule at %zu\n", n_line);
            exit(1);
        }

        size_t * nt_sorted = (size_t *)vec_push(&g->nt_sorted);
        size_t * start_ptr = (size_t *)vec_push(&g->start_ptr);
        size_t * num_rules = (size_t *)vec_push(&g->num_rules);
        String * nt = (String *)vec_push(&g->non_terminal);
        str_init_from_slice(nt, &dp.lhs);
        *nt_sorted = 0;
        *num_rules = 0;
       
        StrIter rules = str_iter_from_slice(&dp.rhs, "|");
        
        while (!rules.ended) {
            *num_rules+=1;
            StrSlice rule = str_iter_next(&rules);
            String * rhs = (String *)vec_push(&g->rhs);
            str_init_from_slice(rhs, &rule);
        }
    }
    
    for (size_t i=0; i < g->non_terminal.len; i++) {
        String * nt = (String *)vec_get(&g->non_terminal, i);
        printf("NT: %s (%zu)\n", str_as_ref(nt), *(size_t *)vec_get(&g->num_rules, i));

    }        
}

void gr_drop(Grammar *g) {
    for (size_t i=0; i < g->non_terminal.len; i++) {
        String * nt = (String *)vec_get(&g->non_terminal, i);
        str_drop(nt);
    }        
    for (size_t i=0; i < g->rhs.len; i++) {
        String * nt = (String *)vec_get(&g->rhs, i);
        str_drop(nt);
    }

    vec_drop(&g->non_terminal);
    vec_drop(&g->rhs);
    vec_drop(&g->num_rules);
    vec_drop(&g->nt_sorted);
    vec_drop(&g->start_ptr);
}
