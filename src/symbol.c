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

void data_type_e_debug(DataTypeE *d) {
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
    case Void: {
        printf("Void");
        break;
    }
    case Ukw: {
        puts("Panic: Invalid DataType");
        exit(1);
    }
    }
}

void data_type_e_display(DataTypeE *d) {
    switch (*d) {
    case Int: {
        printf("int");
        break;
    }
    case Real: {
        printf("real");
        break;
    }
    case Str: {
        printf("str");
        break;
    }
    case Bool: {
        printf("bool");
        break;
    }
    case Void: {
        printf("void");
        break;
    }
    case Ukw: {
        puts("Panic: Invalid DataType");
        exit(1);
    }
    }
}

void data_type_debug(DataType *d) {
    printf("DataType { type: ");
    data_type_e_debug(&d->type);
    printf(", num: %zu }", d->size);
}

size_t data_type_size(DataType *d) {
    size_t size;
    switch (d->type) {
    case Int: {
        size = 4;
        break;
    }
    case Real: {
        size = 4;
        break;
    }
    case Str: {
        size = 1;
        break;
    }
    case Bool: {
        size = 1;
        break;
    }
    case Ukw: {
        puts("Panic: Invalid DataType");
        exit(1);
    }
    }
    return size * d->size;
}

void fun_info_debug(FunctionInfo *f) {
    printf("FunctionInfo { type: ");
    data_type_debug(&f->return_type);
    printf(" }");
}

void var_info_debug(VariableInfo *f) {
    printf("VariableInfo { type: ");
    data_type_debug(&f->type);
    printf(", size: %zu, addr: %zu }", data_type_size(&f->type), f->addr);
}

void const_info_debug(ConstantInfo *f) {
    printf("ConstantInfo { type: ");
    data_type_debug(&f->type);
    printf(", size: %zu, addr: %zu }", data_type_size(&f->type), f->addr);
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
