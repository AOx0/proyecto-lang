#ifndef LNG_SYMBOLH
#define LNG_SYMBOLH

#include "vector.h"
#include "str.h"
#include "hashset.h"
#include <stdint.h>

enum SymbolType {
    Function,
    Variable,
    Constant,
    Procedure,
};
typedef enum SymbolType SymbolType;

void sym_type_display(SymbolType st);

HashIdx hash_symbol(void * s);

struct Symbol {
    StrSlice name;
    SymbolType type;
    size_t scope;
    size_t line;
    size_t nchar;
    Vec refs;
};
typedef struct Symbol Symbol;

#endif
