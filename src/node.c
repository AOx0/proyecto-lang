#include "node.h"
#include "symbol.h"
#include "tree.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

void node_type_debug(NodeType *nt) {
    switch (*nt) {
    case NProgram: {
        printf("NProgram");
        break;
    }
    case NWrite: {
        printf("NWrite");
        break;
    }
    case NRead: {
        printf("NRead");
        break;
    }
    case NVar: {
        printf("NVar");
        break;
    }
    case NAssign: {
        printf("NAssign");
        break;
    }
    case NFunction: {
        printf("NFunction");
        break;
    }
    case NFunctionSign: {
        printf("NFunctionSign");
        break;
    }
    case NVoid: {
        printf("NVoid");
        break;
    }
    default:
        puts("Panic: Invalid type");
        exit(1);
    }
}

void node_display_id(size_t id, FILE *f, Tree *t, HashSet *tabla) {
    Node *node = (Node *)vec_get(&t->values, id);
    node_display(node, f, t, tabla);
}

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla) {
    node_type_debug(&n->node_type);
    printf("\n");

    switch (n->node_type) {
    case NVoid: {
        if (tree_num_child(t, n->id) > 0) {
            TreeIter ti = tree_iter_new(t, n->id);
            printf("SIZE: %zu\n", t->values.len);
            size_t i = 0;

            while (1) {
                printf("I %zu \n", i);
                Node *v = tree_iter_next_child(&ti, n->id);
                if (v == NULL)
                    break;
                printf("ID %zu ", v->id);
                node_type_debug(&v->node_type);
                puts("");
                node_display_id(v->id, f, t, tabla);
            }

            vec_drop(&ti.tmp);
            vec_drop(&ti.parents);
        }
        break;
    };
    case NVar: {
        VarNode node = n->value.var;
        data_type_display(f, 0, &node.symbol.name, &node.symbol.info.var.type);
        fprintf(f, ";\n");
        break;
    }
    case NConst: {
        ConstNode node = n->value.cons;
        data_type_display(f, 0, &node.symbol.name, &node.symbol.info.cons.type);
        fprintf(f, " = ");
        const_value_display(f, &node.symbol.info.cons.value,
                            &node.symbol.info.cons.type);
        fprintf(f, ";\n");
        break;
    }
    case NFunction: {
        fprintf(f, "Function");
        break;
    }
    case NFunctionSign: {
        data_type_display(
            f, 1, &n->value.fun.name,
            &(DataType){.type = n->value.fun.return_type, .size = 1});
        fprintf(f, " %.*s(", (int)n->value.fun.name.len, n->value.fun.name.ptr);

        if (n->value.fun.args.len != 0) {
            for (size_t i = 0; i < n->value.fun.args.len; i++) {
                Symbol *sl = (Symbol *)vec_get(&n->value.fun.args, i);
                data_type_e_display(f, &sl->info.var.type.type);
                fprintf(f, " %.*s", (int)sl->name.len, sl->name.ptr);
                if (i + 1 != n->value.fun.args.len) {
                    fprintf(f, ", ");
                }
            }
        } else {
            fprintf(f, "void");
        }
        fprintf(f, ")");

        break;
    }
    case NProgram: {
        size_t i = 0;
        while (tabla->elements > i) {
            for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
                Vec *arr = (Vec *)vec_get(&tabla->values, j);
                for (size_t h = 0; h < arr->len; h++) {
                    Symbol *s = (Symbol *)vec_get(arr, h);
                    switch (s->type) {
                    case Function:
                    case Procedure: {
                        Node n = (Node){
                            .node_type = NFunctionSign,
                            .value.fun = (FunctionNode){
                                .args = s->info.fun.args,
                                .return_type = s->info.fun.return_type.type,
                                .name = s->name}};
                        node_display(&n, f, t, tabla);
                        fprintf(f, ";\n");
                        break;
                    }
                    default:
                        break;
                    }

                    i += 1;
                }
            }
        }

        fprintf(f, "\nvoid %.*s(", (int)n->value.fun.name.len,
                n->value.fun.name.ptr);
        for (size_t i = 0; i < n->value.fun.args.len; i++) {
            StrSlice *sl = (StrSlice *)vec_get(&n->value.fun.args, i);
            fprintf(f, "char * %.*s", (int)sl->len, sl->ptr);
            if (i + 1 != n->value.fun.args.len) {
                fprintf(f, ", ");
            }
        }
        fprintf(f, ") {\n");

        if (tree_num_child(t, n->id) > 0) {
            TreeIter ti = tree_iter_new(t, n->id);
            while (1) {
                Node *v = tree_iter_next_child(&ti, n->id);
                if (v == NULL)
                    break;
                fprintf(f, "    ");
                node_display_id(v->id, f, t, tabla);
            }
        }

        fprintf(f, "}\n");
        break;
    }
    case NWrite: {
        fprintf(f, "Write");
        break;
    }
    case NRead: {
        fprintf(f, "Read");
        break;
    }
    case NAssign: {
        fprintf(f, "Assign");
        break;
    }
    }
}
