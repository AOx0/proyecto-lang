#ifndef LNG_SYMBOLH
#define LNG_SYMBOLH

#include "hashset.h"
#include "str.h"
#include "vector.h"
#include <stdint.h>

enum SymbolType {
    Function,
    Procedure,
    Variable,
    Constant,
};
typedef enum SymbolType SymbolType;

enum DataTypeE {
    Int,
    Real,
    Str,
    Bool,
    Void,
    Ukw,
};
typedef enum DataTypeE DataTypeE;

struct DataType {
        DataTypeE type;
        size_t size;
};
typedef struct DataType DataType;

struct FunctionInfo {
        Vec args;
        DataType return_type;
};
typedef struct FunctionInfo FunctionInfo;

struct VariableInfo {
        DataType type;
        size_t addr;
};
typedef struct VariableInfo VariableInfo;

struct ConstantInfo {
        DataType type;
        size_t addr;
};
typedef struct ConstantInfo ConstantInfo;

union SymbolInfo {
        FunctionInfo fun;
        VariableInfo var;
        ConstantInfo cons;
};
typedef union SymbolInfo SymbolInfo;

void data_type_debug(DataType *d);
size_t data_type_size(DataType *d);
size_t data_type_e_size(DataTypeE *d);
void data_type_e_display(DataTypeE *d);
void const_info_debug(ConstantInfo *f);
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
