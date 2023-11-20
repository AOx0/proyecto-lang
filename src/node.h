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
    NAssign,
    NFunction,
    NFunctionSign
} NodeType;

typedef struct FunctionNode {
        StrSlice name;
        DataTypeE return_type;
        Vec args;
} FunctionNode;

typedef struct BlockNode {
} BlockNode;

typedef struct VarNode {
        Symbol symbol;
} VarNode;

typedef struct ConstNode {
        Symbol symbol;
} ConstNode;

typedef union NodeValue {
        StrSlice sl;
        FunctionNode fun;
        BlockNode block;
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
