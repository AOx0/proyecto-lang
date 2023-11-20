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
    NAssign,
    NFunction,
    NFunctionSign
} NodeType;

typedef struct FunctionNode {
        StrSlice name;
        DataTypeE return_type;
        Vec args;
} FunctionNode;

typedef union NodeValue {
        StrSlice sl;
        FunctionNode fun;
} NodeValue;

typedef struct Node {
        NodeType node_type;
        NodeValue value;
} Node;

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla);

#endif
