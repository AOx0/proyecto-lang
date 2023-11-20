#include "node.h"
#include "symbol.h"
#include <stdio.h>

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla) {
    switch (n->node_type) {
    case NFunction: {
        fprintf(f, "Function");
        break;
    }
    case NFunctionSign: {
        data_type_e_display(&n->value.fun.return_type);
        fprintf(f, " %.*s(", (int)n->value.fun.name.len, n->value.fun.name.ptr);

        if (n->value.fun.args.len != 0) {
            for (size_t i = 0; i < n->value.fun.args.len; i++) {
                Symbol *sl = (Symbol *)vec_get(&n->value.fun.args, i);
                data_type_e_display(&sl->info.var.type.type);
                fprintf(f, " %.*s", (int)sl->name.len, sl->name.ptr);
                if (i + 1 != n->value.fun.args.len) {
                    fprintf(f, ", ");
                }
            }
        } else {
            fprintf(f, "void");
        }
        fprintf(f, ");\n");

        break;
    }
    case NProgram: {
        size_t i = 0;
        puts("Contenidos de la tabla:");
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
                        break;
                    }
                    default:
                        break;
                    }

                    i += 1;
                }
            }
        }
        fprintf(f, "void %.*s(", (int)n->value.fun.name.len,
                n->value.fun.name.ptr);
        for (size_t i = 0; i < n->value.fun.args.len; i++) {
            StrSlice *sl = (StrSlice *)vec_get(&n->value.fun.args, i);
            fprintf(f, "char * %.*s", (int)sl->len, sl->ptr);
            if (i + 1 != n->value.fun.args.len) {
                fprintf(f, ", ");
            }
        }
        fprintf(f, ") {\n");

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
