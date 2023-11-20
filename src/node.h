#ifndef LNG_NODEH
#define LNG_NODEH

#include "symbol.h"
#include "vector.h"
#include "str.h"
#include "tree.h"
#include "hashset.h"

typedef enum NodeType { NProgram, NWrite, NRead, NAssign, NFunction, NFunctionSign } NodeType;

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

void node_display(Node * n, FILE * f, Tree * t, HashSet * tabla);


#endif
