#include "node.h"
#include "hashset.h"
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
    case NConst: {
        printf("NConst");
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
    // node_type_debug(&n->node_type);
    // printf("\n");

    switch (n->node_type) {
    case NExpr: {
        switch (n->value.expr.type) {
        case EIntValue: {
            fprintf(f, "%lld", n->value.expr.value.int_value);
            break;
        }
        case ERealValue: {
            fprintf(f, "%lf", n->value.expr.value.real_value);
            break;
        }
        case EStringValue: {
            fprintf(f, "\"%.*s\"", (int)n->value.expr.value.string_value.len,
                    n->value.expr.value.string_value.ptr);
            break;
        }
        case ESymbol: {
            Symbol *s =
                (Symbol *)hashset_get(tabla, &n->value.expr.value.symbol);
            if (s == NULL) {
                printf("Panic: Symbol not found\n");
                exit(1);
            }
            fprintf(f, "%.*s", (int)s->name.len, s->name.ptr);
            break;
        }
        case ESymbolIdx: {
            Symbol *s =
                (Symbol *)hashset_get(tabla, &n->value.expr.value.symbol);
            if (s == NULL) {
                printf("Panic: Symbol not found\n");
                exit(1);
            }
            fprintf(f, "%.*s[%zu]", (int)s->name.len, s->name.ptr,
                    n->value.expr.value.symbol_idx.index);
            break;
        }
        case EFunctionCall: {
            Symbol *s = (Symbol *)hashset_get(
                tabla, &n->value.expr.value.function_call.symbol);
            if (s == NULL) {
                printf("Panic: Symbol not found\n");
                exit(1);
            }
            fprintf(f, "%.*s(", (int)s->name.len, s->name.ptr);
            Tree args = n->value.expr.value.function_call.args;
            TreeIter ti = tree_iter_new(&args, 0);
            while (1) {
                size_t *id = tree_iter_next(&ti);
                if (id == NULL) {
                    break;
                }
                Node *n = (Node *)vec_get(&args.values, *id);
                node_display(n, f, &args, tabla);
                if (tree_iter_has_next(&ti)) {
                    fprintf(f, ", ");
                }
            }
            fprintf(f, ")");
            break;
        }
        case EOp: {
            // Primero mostramos la derecha
            Vec hijos = tree_get_childs(t, n->id);

            if (hijos.len != 2) {
                printf("Panic: Invalid number of childs\n");
                exit(1);
            }

            Node *derecha =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 1));
            node_display_id(derecha->id, f, t, tabla);

            // Luego el operador
            fprintf(f, " %c ", n->value.expr.value.op);

            // Y por ultimo la izquierda
            Node *izquierda =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 0));
            node_display_id(izquierda->id, f, t, tabla);

            break;
        }
        case EUnaryOp: {
            // Mostramos el operador
            fprintf(f, "%c(", n->value.expr.value.op);

            // El valor
            Vec hijos = tree_get_childs(t, n->id);

            if (hijos.len != 1) {
                printf("Panic: Invalid number of childs\n");
                exit(1);
            }

            Node *derecha =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 0));
            node_display_id(derecha->id, f, t, tabla);
            fprintf(f, ")");
            break;
        }
        }
        break;
    }
    case NVoid: {
        fprintf(f, "#include <stdint.h>\n");
        fprintf(f, "#include <stdio.h>\n");
        fprintf(f, "#include <stdlib.h>\n\n");
        Vec child = tree_get_childs(t, n->id);
        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            // printf("%zu\n", *id);
            Node *v = (Node *)vec_get(&t->values, *id);
            node_display_id(v->id, f, t, tabla);
        }
        vec_drop(&child);
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
        fprintf(f, "\n");
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

        Vec child = tree_get_childs(t, n->id);
        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            printf("%zu\n", *id);
            Node *v = (Node *)vec_get(&t->values, *id);
            node_display_id(v->id, f, t, tabla);
        }
        vec_drop(&child);

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
