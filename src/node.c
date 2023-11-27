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

void display_identation(FILE *f, size_t level) {
    for (size_t i = 0; i < level; i++) {
        fprintf(f, "    ");
    }
}

// So (1, 2), (1, 3) becomes (1, 3), (1, 2)
void flip_tree_relations(Tree *t) {
    Vec new_relations = vec_new(sizeof(TreeEntry));

    for (size_t i = t->relations.len - 1; i >= 0; i--) {
        TreeEntry *te = (TreeEntry *)vec_get(&t->relations, i);

        TreeEntry *new = (TreeEntry *)vec_push(&new_relations);
        new->from = te->from;
        new->to = te->to;

        if (i == 0)
            break;
    }

    vec_drop(&t->relations);
    t->relations = new_relations;
}

Node *ast_create_node(Tree *t, NodeType nt, DataTypeE asoc_type) {
    size_t id = 0;
    void *res = tree_new_node(t, &id);
    ((Node *)res)->id = id;
    ((Node *)res)->node_type = nt;
    ((Node *)res)->asoc_type = asoc_type;
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
        // size_t id_was = value->id;
        Node *new_node = ast_create_node(t, value->node_type, value->asoc_type);
        size_t new_id_is = new_node->id;
        memcpy(new_node, value, t->values.t_size);

        Node *n = vec_get(&t->values, new_id_is);
        n->id = new_id_is;

        // printf("New id for %zu is %zu\n", id_was, new_id_is);
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
    case NStr:
        return "string";
        break;
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
    case NStr:
        printf("NStr");
        break;
    }
}

void display_optype(FILE *f, OpType t) {
    switch (t) {
    case OpAdd: {
        fprintf(f, " + ");
        break;
    }
    case OpSub: {
        fprintf(f, " - ");
        break;
    }
    case OpMul: {
        fprintf(f, " * ");
        break;
    }
    case OpDiv: {
        fprintf(f, " / ");
        break;
    }
    case OpMod: {
        fprintf(f, " %% ");
        break;
    }
    case OpEq: {
        fprintf(f, " == ");
        break;
    }
    case OpNeq: {
        fprintf(f, " != ");
        break;
    }
    case OpBt: {
        fprintf(f, " > ");
        break;
    }
    case OpLt: {
        fprintf(f, " < ");
        break;
    }
    case OpEbt: {
        fprintf(f, " >= ");
        break;
    }
    case OpElt: {
        fprintf(f, " <= ");
        break;
    }
    case OpAnd: {
        fprintf(f, " && ");
        break;
    }
    case OpOr: {
        fprintf(f, " || ");
        break;
    }
    case OpNot: {
        fprintf(f, "!");
        break;
    }
    }
}

void node_display_id(size_t id, FILE *f, Tree *t, HashSet *tabla,
                     size_t level) {
    Node *node = (Node *)vec_get(&t->values, id);
    node_display(node, f, t, tabla, level);
}

void node_display(Node *n, FILE *f, Tree *t, HashSet *tabla, size_t level) {
    // node_type_debug(n->node_type);
    // printf("\n");

    switch (n->node_type) {
    case NIf: {
        display_identation(f, level);
        fprintf(f, "if (");
        Vec child = tree_get_children(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla, 0);
        fprintf(f, ") {\n");

        id = (size_t *)vec_get(&child, 1);
        v = (Node *)vec_get(&t->values, *id);

        node_display_id(v->id, f, t, tabla, level + 1);

        display_identation(f, level);
        fprintf(f, "}");

        if (child.len == 3) {
            fprintf(f, " else {\n");
            id = (size_t *)vec_get(&child, 2);
            v = (Node *)vec_get(&t->values, *id);
            node_display_id(v->id, f, t, tabla, level + 1);
            display_identation(f, level);
            fprintf(f, "}\n");
        } else {
            fprintf(f, "\n");
        }

        vec_drop(&child);

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

            // Get children of the root node of the unique child
            Vec child = tree_get_children(t, n->id);
            if (child.len != 1) {
                panic("Invalid number of children");
            }

            size_t *id = (size_t *)vec_get(&child, 0);
            Vec children = tree_get_children(t, *id);
            
            for (size_t i = 0; i < children.len; i++) {
                size_t *id = (size_t *)vec_get(&children, i);
                Node *v = (Node *)vec_get(&t->values, *id);
                node_display_id(v->id, f, t, tabla, 0);
                if (i + 1 != children.len) {
                    fprintf(f, ", ");
                }
            }
            fprintf(f, ")");

            vec_drop(&children);

            break;
        }
        case EOp: {
            // Primero mostramos la derecha
            Vec hijos = tree_get_children(t, n->id);

            if (hijos.len != 2) {
               panic("Invalid number of children");
            }

            size_t *id = (size_t *)vec_get(&hijos, 0);
            Node *derecha = (Node *)vec_get(&t->values, *id);

            node_display_id(derecha->id, f, t, tabla, 0);

            // Luego el operador
            display_optype(f, n->value.expr.value.op);

            // Y por ultimo la izquierda
            Node *izquierda =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 1));
            node_display_id(izquierda->id, f, t, tabla, 0);

            break;
        }
        case EUnaryOp: {
            // Mostramos el operador
            fprintf(f, "%c(", n->value.expr.value.op);

            // El valor
            Vec hijos = tree_get_children(t, n->id);

            // if (hijos.len != 1) {
            //     panic("Invalid number of children");
            // }

            Node *derecha =
                (Node *)vec_get(&t->values, *(size_t *)vec_get(&hijos, 0));
            node_display_id(derecha->id, f, t, tabla, 0);
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
        Vec child = tree_get_children(t, n->id);

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

                        node_display(&n, f, t, tabla, 0);
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
            node_display_id(v->id, f, t, tabla, 0);
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
        fprintf(f, "\n");
        FunctionNode node = n->value.fun;
        fprintf(f, "%s",
                data_type_e_display_return(node.symbol.asoc_type.type));
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

        Vec child = tree_get_children(t, n->id);

        // printf("Numero de anidados: %zu\n", child.len);

        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            Node *v = (Node *)vec_get(&t->values, *id);
            node_display_id(v->id, f, t, tabla, level + 1);
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

        Vec child = tree_get_children(t, n->id);
        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);
            printf("%zu\n", *id);
            Node *v = (Node *)vec_get(&t->values, *id);
            node_display_id(v->id, f, t, tabla, level + 1);
        }
        vec_drop(&child);

        fprintf(f, "}\n");
        break;
    }
    case NWrite: {
        display_identation(f, level);
        fprintf(f, "printf(\"");

        Vec child = tree_get_children(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);

        Vec children = tree_get_children(t, *id);

        for (size_t i = 0; i < children.len; i++) {
            size_t *id = (size_t *)vec_get(&children, i);
            Node *v = (Node *)vec_get(&t->values, *id);
            
            /*if (v->node_type != NExpr && v->node_type != NStr) {
                panic("Expected expression or constant");
            }*/ 
            

            switch (v->value.expr.type) {
            case EIntValue: {
                fprintf(f, "%s", data_type_e_display_scan(Int));
                break;
            }
            case ERealValue: {
                fprintf(f, "%s", data_type_e_display_scan(Real));
                break;
            }
            case EStringValue: {
                fprintf(f, "%s", data_type_e_display_scan(Str));
                break;
            }
            case ESymbol: {
                Symbol *s =
                    (Symbol *)hashset_get(tabla, &v->value.expr.value.symbol);
                fprintf(f, "%s", data_type_e_display_scan(s->asoc_type.type));

                break;
            }
            case ESymbolIdx: {
                Symbol *s = (Symbol *)hashset_get(
                    tabla, &v->value.expr.value.symbol_idx.symbol);
                fprintf(f, "%s", data_type_e_display_scan(s->asoc_type.type));
                break;
            }
            case EFunctionCall: {
                Symbol *s = (Symbol *)hashset_get(
                    tabla, &v->value.expr.value.function_call.symbol);
                fprintf(f, "%s", data_type_e_display_scan(s->asoc_type.type));
                break;
            }
            case EOp: {
                fprintf(f, "%s", data_type_e_display_scan(n->asoc_type));
                break;
            }
            case EUnaryOp: {
                fprintf(f, "%s", data_type_e_display_scan(n->asoc_type));
                break;
            }
            }
            if (v->node_type == NStr) {
                fprintf(f, "%s", data_type_e_display_scan(Str));
            }
        }

        vec_drop(&child);
        vec_drop(&children);

        fprintf(f, "\", ");

        child = tree_get_children(t, n->id);
        id = (size_t *)vec_get(&child, 0);

        children = tree_get_children(t, *id);

        if (children.len != 0) {
            for (size_t i = 0; i < children.len; i++) {
                size_t *id = (size_t *)vec_get(&children, i);
                Node *v = (Node *)vec_get(&t->values, *id);
                node_display_id(v->id, f, t, tabla, 0);
                if (i + 1 != children.len) {
                    fprintf(f, ", ");
                }
            }
        } else {
            fprintf(f, "");
        }

        
        fprintf(f, ");\n");

        vec_drop(&children);

        break;
    }
    case NRead: {
        display_identation(f, level);
        fprintf(f, "scanf(\"");
        Vec child = tree_get_children(t, n->id);
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
        Vec child = tree_get_children(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);

        Symbol *s = (Symbol *)hashset_get(tabla, &v->value.expr.value.symbol);

        display_identation(f, level);
        if (s->type == Function || s->type == Procedure) {
            fprintf(f, "return ");
        } else {
            node_display_id(v->id, f, t, tabla, 0);
            fprintf(f, " = ");
        }

        id = (size_t *)vec_get(&child, 1);
        v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla, 0);
        fprintf(f, ";\n");

        break;
    }
    case NFor: {
        display_identation(f, level);
        fprintf(f, "for ()");
        Vec child = tree_get_children(t, n->id);
        if (child.len != 2) {
            panic("Invalid number of children");
        }
        break;
    }
    case NWhile: {
        display_identation(f, level);
        fprintf(f, "while (");
        Vec child = tree_get_children(t, n->id);
        size_t *id = (size_t *)vec_get(&child, 0);
        Node *v = (Node *)vec_get(&t->values, *id);
        node_display_id(v->id, f, t, tabla, 0);
        fprintf(f, ") {\n");

        id = (size_t *)vec_get(&child, 1);
        v = (Node *)vec_get(&t->values, *id);

        node_display_id(v->id, f, t, tabla, level + 1);

        display_identation(f, level);
        fprintf(f, "}\n");

        break;
    }
    case NCall: {
        display_identation(f, level);
        fprintf(f, "%.*s(", (int)n->value.call.symbol.name.len,
                n->value.call.symbol.name.ptr);

        Vec child = tree_get_children(t, n->id);
        if (child.len != 1) {
            panic("Invalid number of children");
        }

        size_t *id = (size_t *)vec_get(&child, 0);
        Vec children = tree_get_children(t, *id);

        if (children.len != 0) {
            for (size_t i = 0; i < children.len; i++) {
                size_t *id = (size_t *)vec_get(&children, i);
                Node *v = (Node *)vec_get(&t->values, *id);
                node_display_id(v->id, f, t, tabla, 0);
                if (i + 1 != children.len) {
                    fprintf(f, ", ");
                }
            }
        } else {
            fprintf(f, "");
        }

        fprintf(f, ");\n");
        break;
    }
    case NVoid: {
        Vec child = tree_get_children(t, n->id);

        for (size_t i = 0; i < child.len; i++) {
            size_t *id = (size_t *)vec_get(&child, i);

            node_display_id(*id, f, t, tabla, level);
        }
    } break;
    case NStr:
        fprintf(f, "\"");
        fprintf(f, "%.*s", (int)n->value.sl.len, n->value.sl.ptr);
        fprintf(f, "\"");
        break;
    }
}
