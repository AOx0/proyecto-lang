#include "node.h"
#include "hashset.h"
#include "panic.h"
#include "str.h"
#include "symbol.h"
#include "tree.h"
#include "vector.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

Node *ast_create_node(Tree *t) {
    size_t id = 0;
    void *res = tree_new_node(t, &id);
    ((Node *)res)->id = id;
    return res;
}

// Expands the specified tree with the subtree in the specified root
// It assumes all nodes are of type Node, and updates the id of the nodes
void tree_extend_with_subtree(Tree *t, Tree *o, size_t sub_tree_root,
                              size_t new_parent) {
    if (t->values.t_size != o->values.t_size)
        panic("Tree values size mismatch %zu != %zu", t->values.t_size,
              o->values.t_size);

    if (o->values.len == 0)
        panic("Trying to join empty tree", t->values.t_size, o->values.t_size);

    size_t offset = t->values.len;

    for (size_t i = 0; i < o->values.len; i++) {
        Node *value = (Node *)vec_get(&o->values, i);
        size_t id_was = value->id;
        Node *new_node = ast_create_node(t);
        size_t new_id_is = new_node->id;
        memcpy(new_node, value, t->values.t_size);

        Node *n = vec_get(&t->values, new_id_is);
        n->id = new_id_is;

        printf("New id for %zu is %zu\n", id_was, new_id_is);
    }

    for (size_t i = 0; i < o->relations.len; i++) {
        TreeEntry *te = (TreeEntry *)vec_get(&o->relations, i);
        size_t from = te->from;
        size_t to = te->to;
        tree_new_relation(t, from + offset, to + offset);
    }

    tree_new_relation(t, new_parent, sub_tree_root + offset);
}

void tree_root_extend(Tree *t, Tree *o) {
    tree_extend_with_subtree(t, o, 0, 0);
}

void tree_debug(Tree *t, HashSet *tabla) {

    UNUSED(tabla);

    for (size_t i = 0; i < t->values.len; i++) {
        Node *n = (Node *)vec_get(&t->values, i);
        printf("%zu: ", n->id);
        node_type_debug(n->node_type);

        printf("\n");
    }

    /*
    for (size_t i = 0; i < t->relations.len; i++) {
        Node *n = (Node *)vec_get(&t->values, i);
        printf("%zu: ", n->id);
        node_type_debug(n->node_type);
        printf(" ");
        if (n->node_type != NRoot && n->node_type != NProgram)
            node_display(n, stdout, t, tabla);
        printf("\n");
    }
    */

    tree_relations_debug(t);

    TreeIter ti = tree_iter_new(t, 0);
    while (1) {
        // Print tree with identation
        TreeIterEntry entry = tree_iter_next(&ti);
        if (entry.did_set == 0) {
            break;
        }

        Node *n = (Node *)entry.value;
        for (size_t i = 1; i < entry.level; i++) {
            printf("-");
        }
        node_type_debug(n->node_type);
        printf("\n");
    }
}

char *node_type_display(NodeType nt) {
    switch (nt) {
    case NProgram: {
        return "programa";
    }
    case NWrite: {
        return "escritura";
    }
    case NRead: {
        return "lectura";
    }
    case NVar: {
        return "declaración de variable";
    }
    case NConst: {
        return "declaración de constante";
    }
    case NAssign: {
        return "asignacion";
    }
    case NFunction: {
        return "funcion";
    }
    case NFunctionSign: {
        return "funcion_sign";
    }
    case NVoid: {
        return "void";
    }
    case NExpr: {
        return "expresion";
    }
    case NIf: {
        return "if";
    }
    case NFor: {
        return "bucle for";
    }
    case NWhile: {
        return "bucle while";
    }
    case NCall:
        return "llamada a funcion";
    case NRoot:
        return "root";
    }

    panic("Invalid node type");
    return NULL;
}

void node_type_debug(NodeType nt) {
    switch (nt) {
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
    case NExpr: {
        printf("NExpr");
        break;
    }
    case NIf: {
        printf("NIf");
        break;
    }
    case NFor: {
        printf("NFor");
        break;
    }
    case NWhile: {
        printf("NWhile");
        break;
    }
    case NCall:
        printf("NCall");
        break;
    case NRoot:
        printf("NRoot");
        break;
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
    case NIf: {
        fprintf(f, "if (");
        Vec child = tree_get_childs(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla);
        fprintf(f, ") {\n");
        break;
    }
    case NExpr: {
        switch (n->value.expr.type) {
        case EIntValue: {
            fprintf(f, "%" PRId64, n->value.expr.value.int_value);
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
                panic("Symbol not found");
            }
            fprintf(f, "%.*s", (int)s->name.len, s->name.ptr);
            break;
        }
        case ESymbolIdx: {
            Symbol *s =
                (Symbol *)hashset_get(tabla, &n->value.expr.value.symbol);
            if (s == NULL) {
                panic("Symbol not found");
            }
            fprintf(f, "%.*s[%zu]", (int)s->name.len, s->name.ptr,
                    n->value.expr.value.symbol_idx.index);
            break;
        }
        case EFunctionCall: {
            Symbol *s = (Symbol *)hashset_get(
                tabla, &n->value.expr.value.function_call.symbol);
            if (s == NULL) {
                panic("Symbol not found");
            }

            if (s->info.fun.args.len == 0) {
                fprintf(f, "%.*s", (int)s->name.len, s->name.ptr);
                return;
            }

            fprintf(f, "%.*s(", (int)s->name.len, s->name.ptr);
            Tree args = n->value.expr.value.function_call.args;
            Vec childs = tree_get_childs(&args, 0);
            for (size_t i = 0; i < childs.len; i++) {
                size_t *id = (size_t *)vec_get(&childs, i);
                Node *v = (Node *)vec_get(&args.values, *id);
                node_display_id(v->id, f, &args, tabla);
                if (i + 1 != childs.len) {
                    fprintf(f, ", ");
                }
            }
            fprintf(f, ")");
            break;
        }
        case EOp: {
            // Primero mostramos la derecha
            Vec hijos = tree_get_childs(t, n->id);

            // if (hijos.len != 2) {
            //      panic("Invalid number of childs");
            //  }

            Node *derecha =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 1));

            if (derecha->node_type != NExpr) {
                panic("Esperaba una expresion");
            }

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

            // if (hijos.len != 1) {
            //     panic("Invalid number of childs");
            // }

            Node *derecha =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 0));
            node_display_id(derecha->id, f, t, tabla);
            fprintf(f, ")");
            break;
        }
        }
        break;
    }
    case NRoot: {
        fprintf(f, "#include <stdint.h>\n");
        fprintf(f, "#include <stdio.h>\n");
        fprintf(f, "#include <stdlib.h>\n\n");
        Vec child = tree_get_childs(t, n->id);

        size_t i = 0;
        while (tabla->elements > i) {
            for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
                Vec *arr = (Vec *)vec_get(&tabla->values, j);
                for (size_t h = 0; h < arr->len; h++) {
                    Symbol *s = (Symbol *)vec_get(arr, h);
                    switch (s->type) {
                    case Function:
                    case Procedure: {
                        Node n = {.node_type = NFunctionSign,
                                  .value.fun = {
                                      .args = s->info.fun.args,
                                      .symbol = *s,
                                  }};
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
        fprintf(f, "\n");

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
        data_type_display(f, 0, &node.symbol.name, &node.symbol.asoc_type);
        fprintf(f, ";\n");
        break;
    }
    case NConst: {
        ConstNode node = n->value.cons;
        data_type_display(f, 0, &node.symbol.name, &node.symbol.asoc_type);
        fprintf(f, " = ");
        const_value_display(f, &node.symbol.info.cons.value,
                            &node.symbol.asoc_type);
        fprintf(f, ";\n");
        break;
    }
    case NFunction: {
        FunctionNode node = n->value.fun;
        data_type_e_display_return(node.symbol.asoc_type.type);
        fprintf(f, " %.*s(", (int)node.symbol.name.len, node.symbol.name.ptr);

        if (node.symbol.info.fun.args.len != 0) {
            for (size_t i = 0; i < node.symbol.info.fun.args.len; i++) {
                Symbol *sl = (Symbol *)vec_get(&node.symbol.info.fun.args, i);
                data_type_e_display(f, sl->asoc_type.type);
                fprintf(f, " %.*s", (int)sl->name.len, sl->name.ptr);
                if (i + 1 != node.symbol.info.fun.args.len) {
                    fprintf(f, ", ");
                }
            }
        } else {
            fprintf(f, "");
        }

        fprintf(f, ") {\n");

        Vec child = tree_get_childs(t, n->id);
        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            Node *v = (Node *)vec_get(&t->values, *id);
            fprintf(f, "    ");
            node_display_id(v->id, f, t, tabla);
        }
        vec_drop(&child);

        fprintf(f, "}\n");

        break;
    }
    case NFunctionSign: {
        data_type_display(
            f, 1, &n->value.fun.symbol.name,
            &(DataType){.type = n->value.fun.symbol.asoc_type.type, .size = 1});
        fprintf(f, " %.*s(", (int)n->value.fun.symbol.name.len,
                n->value.fun.symbol.name.ptr);

        if (n->value.fun.args.len != 0) {
            for (size_t i = 0; i < n->value.fun.args.len; i++) {
                Symbol *sl = (Symbol *)vec_get(&n->value.fun.args, i);
                data_type_e_display(f, sl->asoc_type.type);
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
        fprintf(f, "\nvoid %.*s(", (int)n->value.fun.symbol.name.len,
                n->value.fun.symbol.name.ptr);
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

        fprintf(f, "printf(\"");

        Vec child = tree_get_childs(t, n->id);
        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            Node *v = (Node *)vec_get(&t->values, *id);
            if (v->node_type != NExpr) {
                panic("Expected expression");
            }

            switch (v->value.expr.type) {
            case EIntValue: {
                data_type_e_display_scan(Int);
                break;
            }
            case ERealValue: {
                data_type_e_display_scan(Real);
                break;
            }
            case EStringValue: {
                data_type_e_display_scan(Str);
                break;
            }
            case ESymbol: {
                Symbol *s =
                    (Symbol *)hashset_get(tabla, &v->value.expr.value.symbol);
                data_type_e_display_scan(s->asoc_type.type);
                break;
            }
            case ESymbolIdx: {
                Symbol *s = (Symbol *)hashset_get(
                    tabla, &v->value.expr.value.symbol_idx.symbol);
                data_type_e_display_scan(s->asoc_type.type);
                break;
            }
            case EFunctionCall: {
                Symbol *s = (Symbol *)hashset_get(
                    tabla, &v->value.expr.value.function_call.symbol);
                data_type_e_display_scan(s->asoc_type.type);
                break;
            }
            case EOp: {
                data_type_e_display_scan(n->asoc_type);
                break;
            }
            case EUnaryOp: {
                data_type_e_display_scan(n->asoc_type);
                break;
            }
            }
        }

        break;
    }
    case NRead: {
        fprintf(f, "scanf(\"");
        Vec child = tree_get_childs(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);
        data_type_e_display_scan(v->value.read.target_symbol.asoc_type.type);
        fprintf(f, "\", &%.*s);\n", (int)v->value.read.target_symbol.name.len,
                v->value.read.target_symbol.name.ptr);
        if (v->value.read.newline == 1) {
            fprintf(f, "\n");
        }
        fprintf(f, "\"");
        break;
    }
    case NAssign: {
        Vec child = tree_get_childs(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 1);
        Node *v = (Node *)vec_get(&t->values, *id);

        node_display_id(v->id, f, t, tabla);
        fprintf(f, " = ");

        id = (size_t *)vec_get(&child, 0);
        v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla);
        fprintf(f, ";\n");

        break;
    }
    case NFor:
        fprintf(f, "For\n");
        break;
    case NWhile:
        fprintf(f, "while (");
        Vec child = tree_get_childs(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla);
        fprintf(f, ") {\n");

        id = (size_t *)vec_get(&child, 1);
        v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla);

        fprintf(f, "}\n");

        break;
    case NCall:
        fprintf(f, "%.*s(", (int)n->value.call.symbol.name.len,
                n->value.call.symbol.name.ptr);

        if (n->value.call.args.relations.len != 0) {
            Node *nnnnn = (Node *)vec_get(&n->value.call.args.values, 0);
            node_display_id(nnnnn->id, f, t, tabla);
        } else {
            fprintf(f, "");
        }

        break;
    case NVoid: {
        Vec child = tree_get_childs(t, n->id);

        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            node_display_id(*id, f, t, tabla);
        }
    } break;
    }
}
