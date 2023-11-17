#include "symbol.h"

HashIdx hash_symbol(void * s) {
    HashIdx res;
    res.idx = 0;

    Symbol *sy = (Symbol *)s;

    for (size_t i=0; i<sy->name.len && i < 5; i++) {
        res.idx += sy->name.ptr[i] * (i + 1);
    }

    res.idx *= sy->scope + 1;

    // printf("HASH: %zu FROM: %zu(%zu): %.*s\n", res.idx, sy->line, sy->scope, (int)sy->name.len, sy->name.ptr);
    
    return res;
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
