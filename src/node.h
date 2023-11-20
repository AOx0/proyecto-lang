#ifndef LNG_NODEH
#define LNG_NODEH

#include "hashset.h"
#include "str.h"
#include "symbol.h"
#include "tree.h"
#include "vector.h"

typedef enum NodeType {
    NProgram,
    NWrite,
    NRead,
    NVar,
    NVoid,
    NConst,
    NAssign,
    NFunction,
    NFunctionSign
} NodeType;

typedef struct FunctionNode {
        StrSlice name;
        DataTypeE return_type;
        Vec args;
} FunctionNode;

typedef struct VarNode {
        Symbol symbol;
} VarNode;

typedef struct ConstNode {
        Symbol symbol;
        ConstValue value;
} ConstNode;

typedef enum ExprType {
    EIntValue,
    ERealValue,
    EStringValue,
    ESymbol,
    ESymbolIdx,
    EFunctionCall,
} ExprType;

typedef struct FunctionCall {
        Symbol symbol;
        Vec args;
} FunctionCall;

typedef struct IndexedSymbol {
        Symbol symbol;
        size_t index;
} IndexedSymbol;

typedef union ExprValue {
        int64_t int_value;
        double real_value;
        StrSlice string_value;
        Symbol symbol;
        IndexedSymbol symbol_idx;
        FunctionCall function_call;
} ExprValue;

typedef enum OpType {
    OpAnd,
    OpOr,
    OpNot,
    OpEq,
    OpNeq,
    OpGt,
    OpLt,
    OpGte,
    OpLte,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    OpMod,
} OpType;

typedef struct OpNode {
        OpType type;
} OpNode;

typedef struct ExprNode {
        ExprType type;
        ExprValue value;
        DataTypeE asoc_type;
} ExprNode;

typedef union NodeValue {
        StrSlice sl;
        FunctionNode fun;
        VarNode var;
        ConstNode cons;
} NodeValue;

typedef struct Node {
        NodeType node_type;
        NodeValue value;
        size_t id;
} Node;

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla);
void node_display_id(size_t id, FILE *f, Tree *t, HashSet *tabla);
void node_type_debug(NodeType *nt);

#endif
