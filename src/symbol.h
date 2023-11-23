#ifndef LNG_SYMBOLH
#define LNG_SYMBOLH

#include "hashset.h"
#include "str.h"
#include "vector.h"
#include <stdint.h>

enum SymbolType { Function, Procedure, Variable, Constant, Unknown };
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
};
typedef struct FunctionInfo FunctionInfo;

struct VariableInfo {
        size_t addr;
};
typedef struct VariableInfo VariableInfo;

typedef union ConstValue {
        StrSlice str;
        int64_t snum;
        double real;
        int bool;
} ConstValue;

struct ConstantInfo {
        size_t addr;
        ConstValue value;
};
typedef struct ConstantInfo ConstantInfo;

typedef struct Incomplete {
        int f;
} Incomplete;

union SymbolInfo {
        FunctionInfo fun;
        VariableInfo var;
        ConstantInfo cons;
        Incomplete none;
};
typedef union SymbolInfo SymbolInfo;

void data_type_debug(DataType *d);
size_t data_type_size(DataType *d);
size_t data_type_e_size(DataTypeE *d);
void data_type_e_display(FILE *f, DataTypeE *d);
void data_type_display(FILE *f, int is_fun, StrSlice *name, DataType *d);
char *data_type_e_display_return(DataTypeE *d);
void const_value_display(FILE *f, ConstValue *cv, DataType *d);
void const_info_debug(ConstantInfo *f);
void fun_info_debug(FunctionInfo *f);
void var_info_debug(VariableInfo *f);
void sym_type_display(SymbolType st);

HashIdx hash_symbol(void *s);

struct Symbol {
        StrSlice name;
        SymbolType type;
        SymbolInfo info;
        DataType asoc_type;
        size_t scope;
        size_t line;
        size_t nchar;
        Vec refs;
};
typedef struct Symbol Symbol;

#endif
