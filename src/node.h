#ifndef LNG_NODEH
#define LNG_NODEH

typedef enum NodeType { Write, Read, Assign } NodeType;

typedef struct Node {
        NodeType node_type;
} Node;

#endif
