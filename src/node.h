#ifndef LNG_NODEH
#define LNG_NODEH

#include "hashset.h"
#include "panic.h"
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
    NFunctionSign,
    NExpr
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
    EOp,
    EUnaryOp,
} ExprType;

typedef struct FunctionCall {
        Symbol symbol;
        Tree args;
        DataTypeE return_type;
} FunctionCall;

typedef struct IndexedSymbol {
        Symbol symbol;
        size_t index;
} IndexedSymbol;

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
    OpAssign,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    OpMod,
} OpType;

typedef union ExprValue {
        int64_t int_value;
        double real_value;
        StrSlice string_value;
        Symbol symbol;
        IndexedSymbol symbol_idx;
        FunctionCall function_call;
        OpType op;
} ExprValue;

typedef struct OpNode {
        OpType type;
} OpNode;

typedef struct ExprNode {
        ExprType type;
        ExprValue value;
} ExprNode;

typedef union NodeValue {
        StrSlice sl;
        FunctionNode fun;
        VarNode var;
        ConstNode cons;
        ExprNode expr;
        OpNode op;
} NodeValue;

typedef struct Node {
        NodeType node_type;
        NodeValue value;
        size_t id;
        DataTypeE asoc_type;
} Node;

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla);
void node_display_id(size_t id, FILE *f, Tree *t, HashSet *tabla);
void node_type_debug(NodeType *nt);

#endif
