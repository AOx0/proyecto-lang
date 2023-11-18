#ifndef LNG_SYMBOLH
#define LNG_SYMBOLH

#include "hashset.h"
#include "str.h"
#include "vector.h"
#include <stdint.h>

enum SymbolType {
    Function,
    Variable,
    Constant,
    Procedure,
};
typedef enum SymbolType SymbolType;

enum DataType {
    Int,
    Real,
    Str,
    Bool,
    Ukw,
};
typedef enum DataType DataType;

struct FunctionInfo {
        DataType return_type;
};
typedef struct FunctionInfo FunctionInfo;

struct VariableInfo {
        DataType type;
};
typedef struct VariableInfo VariableInfo;

union SymbolInfo {
        FunctionInfo fun;
        VariableInfo var;
};
typedef union SymbolInfo SymbolInfo;

void data_type_debug(DataType *d);
void fun_info_debug(FunctionInfo *f);
void var_info_debug(VariableInfo *f);
void sym_type_display(SymbolType st);

HashIdx hash_symbol(void *s);

struct Symbol {
        StrSlice name;
        SymbolType type;
        SymbolInfo info;
        size_t scope;
        size_t line;
        size_t nchar;
        Vec refs;
};
typedef struct Symbol Symbol;

#endif
