#include "symbol.h"
#include <stdio.h>

HashIdx hash_symbol(void *s) {
    HashIdx res;
    res.idx = 0;

    Symbol *sy = (Symbol *)s;

    for (size_t i = 0; i < sy->name.len && i < 5; i++) {
        res.idx += sy->name.ptr[i] * (i + 1);
    }

    res.idx *= sy->scope + 1;

    // printf("HASH: %zu FROM: %zu(%zu): %.*s\n", res.idx, sy->line, sy->scope,
    // (int)sy->name.len, sy->name.ptr);

    return res;
}

void data_type_debug(DataType *d) {
    switch (*d) {
    case Int: {
        printf("Int");
        break;
    }
    case Real: {
        printf("Real");
        break;
    }
    case Str: {
        printf("Str");
        break;
    }
    case Bool: {
        printf("Bool");
        break;
    }
    case Ukw: {
        printf("Ukw");
        break;
    }
    }
}

void fun_info_debug(FunctionInfo *f) {
    printf("FunctionInfo { type: ");
    data_type_debug(&f->return_type);
    printf(" }");
}

void var_info_debug(VariableInfo *f) {
    printf("VariableInfo { type: ");
    data_type_debug(&f->type);
    printf(" }");
}

void sym_type_display(SymbolType st) {
    switch (st) {
    case Function:
        printf("FUN");
        break;
    case Variable:
        printf("VAR");
        break;
    case Constant:
        printf("CON");
        break;
    case Procedure:
        printf("PRC");
        break;
    default:
        puts("Panic: Invalid SymbolType");
        exit(1);
    }
}
