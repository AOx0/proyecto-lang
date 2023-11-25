%code top {
    #include "hashset.h"
    #include "parser.h"
    #include "str.h"
    #include "symbol.h"
    #include "node.h"
    #include "tree.h"

    extern FILE * OUT_FILE;
    
    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);

    extern size_t line;
    extern size_t nchar;
    extern size_t yyleng;
    extern int err;

    size_t tnchar;
    size_t scope = 0;
    size_t fun_id = 0;
    size_t addr = 0;

    Tree ast;
    HashSet tabla;
    String wrn_buff;

    Node * tree_get_root(Tree *t) {
        return (Node *)vec_get(&t->values, 0);
    }

    Node * tree_get_node(Tree *t, size_t id) {
        return (Node *)vec_get(&t->values, id);
    }

    void add_reference_to_sym(Symbol * s, size_t line) {
        size_t *ref = vec_push(&s->refs);
        *ref = line;
    }

    Symbol *assert_sym_exists(Symbol * s) {
        tnchar = nchar;
        size_t orig_scope = s->scope;
        int found = 0;
        nchar = s->nchar;
        Symbol *res = NULL;

        for (size_t i = orig_scope; i >= 0; i -= orig_scope) {
            s->scope = i;
            // printf("Looking %.*s in scope %zu\n", (int)s->name.len,
            // s->name.ptr , i);
            if (hashset_contains(&tabla, s)) {
                found = 1;
                res = (Symbol *)hashset_get(&tabla, s);
                add_reference_to_sym(res, s->line);
                break;
            }
            if (i == 0)
                break;
        }

        if (!found) {
            str_clear(&wrn_buff);
            nchar = s->nchar;
            char lit[] = "Error: Simbolo no declarado en el scope actual: ";
            // printf("Scope: %zu\n", scope);
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope,
            // (int)s->name.len, s->name.ptr);
        }
        s->scope = orig_scope;
        nchar = tnchar;
        return res;
    }

    void assert_not_sym_exists(Symbol * s) {
        tnchar = nchar;
        nchar = s->nchar;
        if (hashset_contains(&tabla, s)) {
            str_clear(&wrn_buff);
            char lit[] = "Error: Simbolo ya declarado en el mismo scope: ";
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("No existe %zu(%zu):  %.*s\n", s->line, s->scope,
            // (int)s->name.len, s->name.ptr);
        }
        nchar = tnchar;
    }

    void assert_variable_assigned_type(Node *var, Node *val) {
        if (var->asoc_type != val->asoc_type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento asignar una expresion de tipo distinto a una variable: ");
            str_push_n(&wrn_buff, var->value.expr.value.symbol.name.ptr,  var->value.expr.value.symbol.name.len);
            str_push(&wrn_buff, ", la variable es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&var->asoc_type));
            str_push(&wrn_buff, " y la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&val->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_expr_type(Node *a, Node *b) {
        if (a->asoc_type != b->asoc_type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento realizar operaciones entre expresiones de tipos distintos: ");
            str_push(&wrn_buff, "El primer operando es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&a->asoc_type));
            str_push(&wrn_buff, " y el segundo es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&b->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_node_is_printable(Node *n) {
        if (n->asoc_type == Void || n->asoc_type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir una expresion de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&n->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_ident_is_printable(Symbol *s) {
        if (s->asoc_type.type == Void || s->asoc_type.type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir un identificador de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(&s->asoc_type.type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }
}

%code requires {

    #ifndef LNG_PARSERH
    #define LNG_PARSERH

    #include "hashset.h"
    #include "vector.h"
    #include <inttypes.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include "symbol.h"
    #include "node.h"
    #include "str.h"
    #include "tree.h"

    extern FILE *yyin, *yyout;

    enum RelOp { And, Or };
    typedef enum RelOp RelOp;

    enum AddOp { Add, Sub };
    typedef enum AddOp AddOp;

    enum MulOp { Div, Mod, Mul };
    typedef enum MulOp MulOp;

    #endif
}

%union {
    int64_t snum;
    uint64_t unum;
    double fnum;
    char *ident;
    StrSlice slice;
    DataType type;
    Symbol symbol;
    Vec idents;
    RelOp relop;
    OpType op;
    Tree subtree;
    ExprNode expr;
    FunctionCall function_call;
}

/* Ident */
%token <symbol>IDENT;

/* Par */
%token <fnum>CONST_REAL <snum>CONST_ENTERA <slice>CONST_CADENA 
    <relop>RELOP <op>ADDOP <op>MULOP;

/* Keywords */
%token OP_ASIGN KW_PROCEDURE KW_PROG KW_CONST KW_VAR KW_ARRAY KW_OF KW_FUNC
        KW_BEGIN KW_END KW_READ KW_READLN KW_WRITE KW_WRITELN KW_WHILE KW_FOR
            KW_DO KW_TO KW_DOWNTO KW_IF KW_THEN KW_ELSE KW_DOTS;

/* OPs */
%token RELOP_EQ RELOP_NEQ RELOP_BT RELOP_LT RELOP_EBT RELOP_ELT RELOP_AND
        RELOP_NOT RELOP_OR;

/* Types */
%token T_INT T_REAL T_STR T_BOOL;

%type <idents>ident_lista;
%type <idents>parametros_lista;
%type <idents>argumentos;
%type <type>estandard_tipo;
%type <type>tipo;
%type <expr>relop;
%type <idents>decl;
%type <idents>decl_const;
%type <idents>decl_var;
%type <subtree>expresion;
%type <subtree>expresion_lista;
%type <subtree>factor;
%type <subtree>for_asignacion;
%type <subtree>termino;
%type <subtree>variable;
%type <subtree>relop_expresion;
%type <subtree>relop_and;
%type <subtree>relop_not;
%type <subtree>instrucciones;
%type <subtree>if_instruccion;
%type <subtree>relop_expresion_simple;
%type <subtree>instruccion_compuesta;
%type <subtree>repeticion_instruccion;
%type <subtree>lectura_instruccion;
%type <subtree>escritura_instruccion;
%type <subtree>relop_paren;
%type <subtree>variable_asignacion;
%type <function_call>llamado_funcion;
%type <function_call>procedure_instruccion;

%destructor {
    // printf("Dropping ident_lista:  ");
    // vec_debug_verbose(&$$);

    for (size_t i = 0; i < $$.len; i++) {
        Symbol *s = vec_get(&$$, i);
        vec_drop(&s->refs);
    }

    vec_drop(&$$);
} ident_lista;

%destructor {
    // printf("Dropping parametros_lista:  ");
    // vec_debug_verbose(&$$);

    for (size_t i = 0; i < $$.len; i++) {
        Symbol *s = vec_get(&$$, i);
        vec_drop(&s->refs);
    }

    vec_drop(&$$);
} parametros_lista;

%start programa;

%% 
programa : {
    hashset_init(&tabla, sizeof(Symbol), hash_symbol);
    str_init(&wrn_buff);
    tree_init(&ast, sizeof(Node));
}
KW_PROG IDENT '(' ident_lista ')' ';' decl subprograma_decl
    instruccion_compuesta '.' {

    for (size_t i = 0; i < $5.len; i++) {
        Symbol *s = vec_get(&$5, i);
        assert_not_sym_exists(s);
        s->type = Variable;
        s->asoc_type = (DataType) { .type = Str, .size = 1 };
        s->info.var = (VariableInfo){ .addr = addr};
        hashset_insert(&tabla, s);
    }

    $3.type = Function;
    $3.asoc_type = (DataType) { .type = Void, .size = 0 };
    $3.info.fun = (FunctionInfo) {
        .args = $5,
    };
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);

    size_t idx;
    Node * node = (Node *)tree_new_node(&ast, &idx);
    *node = (Node){ 
        .node_type = NVoid,
        .id = idx,
        .asoc_type = Void,
    };

    size_t child_idx;
    node = (Node *)tree_new_node(&ast, &child_idx);
    *node = (Node) {
        .node_type = NProgram,
        .id = child_idx,
        .value.fun = (FunctionNode) { .name = $3.name, .args = $5, .return_type = Void },
        .asoc_type = Void
    };
    tree_new_relation(&ast, idx, child_idx); 

    for (size_t i = 0; i < $8.len; i++) {
        Symbol *s = vec_get(&$8, i);
        node = (Node *)tree_new_node(&ast, &child_idx);

        if (s->type == Variable) { 
            *node = (Node) {
                .node_type = NVar,
                .id = child_idx,
                .value.var = (VarNode) { .symbol = *s }, 
            };
        } else {
            *node = (Node) {
                .node_type = NConst,
                .id = child_idx,
                .value.cons = (ConstNode) { .symbol = *s, .value.bool = 1 }, 
            };
        }
        tree_new_relation(&ast, idx, child_idx); 
    }


    #ifdef PRINT_TABLE
    size_t i = 0;
    while (tabla.elements > i) {
        for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
            Vec *arr = (Vec *)vec_get(&tabla.values, j);
            for (size_t h = 0; h < arr->len; h++) {
                    Symbol *s = (Symbol *)vec_get(arr, h);
                    sym_type_display(s->type);
                    printf(" (%.2zu,%.2zu), name: %10.*s, location: %2zu:%-2zu, "
                           "scope: %zu, ", j, h, (int)s->name.len, s->name.ptr, s->line, s->nchar,
                           s->scope);

                    printf("type: ");
                    data_type_debug(&s->asoc_type);
                    printf(", ");
                        
                    printf("info: ");

                    switch (s->type) {
                        case Function: { fun_info_debug(&s->info.fun); break; }
                        case Variable: { var_info_debug(&s->info.var); break; }
                        case Constant: { const_info_debug(&s->info.cons); break; }
                        case Procedure: { fun_info_debug(&s->info.fun); break; }
                        default: { panic("Invalid SymbolType"); } 
                    }
                    printf(", ");

                    printf("refs: { ");
                    for (size_t i = 0; i < s->refs.len; i++) {
                        size_t *ref = (size_t *)vec_get(&s->refs, i);
                        printf("%zu", *ref);
                        if (i + 1 != s->refs.len) {
                            printf(", ");
                        }
                    }
                    printf(" }");
                puts("");
                i += 1;
                // vec_drop(&s->refs);
            }
        }
    }
    #endif

    if (err == 0) 
        node_display_id(idx, OUT_FILE, &ast, &tabla);
    


    // Al final liberamos la tabla de hashes de memoria
    vec_drop(&$5);
    hashset_drop(&tabla);
    str_drop(&wrn_buff);
    tree_drop(&ast);
};

ident_lista : IDENT ',' ident_lista {
    $$ = $3;
    Symbol *s = vec_push(&$$);
    *s = $1;
};
ident_lista : IDENT {
    $$ = vec_new(sizeof(Symbol));
    Symbol *s = vec_push(&$$);
    *s = $1;
};

/* Declaration of var and const */
decl : decl_var | decl_const | { $$ = vec_new(sizeof(Symbol)); };

decl_var : decl KW_VAR ident_lista ':' tipo ';' {
    for (size_t i = 0; i < $3.len; i++) {
        Symbol *s = vec_get(&$3, i);
        s->type = Variable;
        s->asoc_type = $5;
        s->info.var = (VariableInfo){.addr = addr};
        addr += data_type_size(&$5);
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
    }

    $$ = $1;
    vec_extend(&$$, &$3);
};

decl_const : decl KW_CONST IDENT '=' CONST_ENTERA ';' {
    $3.type = Constant;
    $3.asoc_type = (DataType){
        .type = Int, 
        .size = 1
    };
    $3.info.cons = (ConstantInfo){
        .addr = addr,
        .value.snum = $5
    };
    addr += 4;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
    $$ = $1;
    Symbol * s = (Symbol *)vec_push(&$$);
    *s = $3;
};
decl_const : decl KW_CONST IDENT '=' CONST_REAL ';' {
    $3.type = Constant;
    $3.asoc_type = (DataType){
        .type = Real, 
        .size = 1
    };
    $3.info.cons = (ConstantInfo){
        .addr = addr,
        .value.real = $5
    };
    addr += 4;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
    $$ = $1;
    Symbol * s = (Symbol *)vec_push(&$$);
    *s = $3;
};
decl_const : decl KW_CONST IDENT '=' CONST_CADENA ';' {
    $3.type = Constant;
    $3.asoc_type = (DataType){
        .type = Str, 
        .size = $5.len
    };
    $3.info.cons = (ConstantInfo){
        .addr = addr,
        .value.str = $5
    };
    addr += 1 * $5.len;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
   
    $$ = $1;
    Symbol * s = (Symbol *)vec_push(&$$);
    *s = $3;
};

/* Tipo */
tipo : estandard_tipo { $$ = $1; }
| KW_ARRAY '[' CONST_ENTERA KW_DOTS CONST_ENTERA ']' KW_OF estandard_tipo {
    $$ = $8;
    $$.size = $$.size * ($5 - $3);
};
estandard_tipo : T_INT { $$ = (DataType){.type = Int, .size = 1}; }
| T_REAL { $$ = (DataType){.type = Real, .size = 1}; }
| T_STR { $$ = (DataType){.type = Str, .size = 1}; }
| T_BOOL { $$ = (DataType){.type = Bool, .size = 1}; };

/* Subprograma */
subprograma_decl : subprograma_decl subprograma_declaracion ';' | ;
subprograma_declaracion
    : subprograma_encabezado decl subprograma_decl instruccion_compuesta {
    scope -= fun_id;
};
subprograma_encabezado : KW_FUNC IDENT {
    $2.type = Function;
    assert_not_sym_exists(&$2);
    hashset_insert(&tabla, &$2);
    fun_id++;
    scope += fun_id;
}
argumentos ':' estandard_tipo ';' {
    Symbol *s = (Symbol *)hashset_get(&tabla, &$2);
    s->asoc_type = $6;
    s->info.fun = (FunctionInfo){.args=$4};
};
subprograma_encabezado : KW_PROCEDURE IDENT {
    $2.type = Procedure;
    assert_not_sym_exists(&$2);
    hashset_insert(&tabla, &$2);
    fun_id++;
    scope += fun_id;
}
argumentos ';'{
    Symbol *s = (Symbol *)hashset_get(&tabla, &$2);
    s->asoc_type = (DataType) { .type = Void, .size = 0};
    s->info.fun = (FunctionInfo){ .args=$4 };
};

/* Argumentos */
argumentos : '(' parametros_lista ')' {
    // printf("Argumentos: %zu\n", $2.len);
    for (size_t i = 0; i < $2.len; i++) {
        Symbol *s = (Symbol *)vec_get(&$2, i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }

    // vec_drop(&$2);
    $$ = $2;
}
| {
    $$ = vec_new(sizeof(Symbol));
};
parametros_lista : ident_lista ':' tipo {
    for (size_t i = 0; i < $1.len; i++) {
        Symbol *s = (Symbol *)vec_get(&$1, i);
        s->type = Variable;
        s->asoc_type = $3;
        s->info.var = (VariableInfo){ .addr = addr };
        addr += data_type_size(&$3);
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
    $$ = $1;
};
parametros_lista : parametros_lista ';' ident_lista ':' tipo {
    $$ = $1;

    for (size_t i = 0; i < $3.len; i++) {
        Symbol *s = (Symbol *)vec_get(&$3, i);
        s->type = Variable;
        s->asoc_type = $5;
        s->info.var = (VariableInfo){ .addr = addr };
        addr += data_type_size(&$5);
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }

    vec_extend(&$$, &$3);
    // vec_drop(&$3);
};

/* Instrucciones */
instruccion_compuesta : KW_BEGIN instrucciones_opcionales KW_END;
instrucciones_opcionales : instrucciones_lista | /* Empty */;
instrucciones_lista : instrucciones | instrucciones_lista ';' instrucciones;
instrucciones: variable_asignacion |
      procedure_instruccion | instruccion_compuesta | if_instruccion |
      repeticion_instruccion | lectura_instruccion | escritura_instruccion;

/* Loops */
repeticion_instruccion: KW_WHILE relop_expresion KW_DO instrucciones {

}
| KW_FOR for_asignacion KW_TO expresion KW_DO instrucciones {

}
| KW_FOR for_asignacion KW_DOWNTO expresion KW_DO instrucciones {

};

/* Lectura */
lectura_instruccion : KW_READ '(' IDENT ')' { 
    assert_sym_exists(&$3);
    
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NRead,
        .asoc_type = Void,
        .value.read = (ReadNode){
            .newline = 0,
            .target_symbol = $3,
        }
    };

    $$ = t;
};
lectura_instruccion : KW_READLN '(' IDENT ')' { 
    assert_sym_exists(&$3); 

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NRead,
        .asoc_type = Void,
        .value.read = (ReadNode){
            .newline = 1,
            .target_symbol = $3,
        }
    };

    $$ = t;
};

/* Escritura */
escritura_instruccion : KW_WRITE '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 0,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    Node * var = (Node *)tree_new_node(&t, NULL);
    *var = (Node) {
        .node_type = NExpr,
        .asoc_type = $5.asoc_type.type,
        .value.expr = (ExprNode) {
            .type = ESymbol,
            .value.symbol = $5,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_new_relation(&t, 0, 2);

    $$ = t;
};
escritura_instruccion : KW_WRITELN '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 1,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    Node * var = (Node *)tree_new_node(&t, NULL);
    *var = (Node) {
        .node_type = NExpr,
        .asoc_type = $5.asoc_type.type,
        .value.expr = (ExprNode) {
            .type = ESymbol,
            .value.symbol = $5,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_new_relation(&t, 0, 2);

    $$ = t;
};
escritura_instruccion : KW_WRITE '(' CONST_CADENA ')'{
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 0,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    tree_new_relation(&t, 0, 1);

    $$ = t;
}  
 | KW_WRITELN '(' CONST_CADENA ')' {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 1,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    tree_new_relation(&t, 0, 1);

    $$ = t;
 }  
 | KW_WRITE '(' CONST_CADENA ',' expresion ')' {
    assert_node_is_printable(tree_get_root(&$5));

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 0,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    tree_new_relation(&t, 0, 1);
    
    tree_extend(&t, &$5, 0);

    $$ = t;
}
 | KW_WRITELN '(' CONST_CADENA ',' expresion ')' {
    assert_node_is_printable(tree_get_root(&$5));

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node) {
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode) {
            .newline = 1,
        }
    };

    Node * str = (Node *)tree_new_node(&t, NULL);
    *str = (Node) {
        .node_type = NConst,
        .asoc_type = Str,
        .value.cons = (ConstNode) { .symbol = $3, .value.str = $3 }
    };

    tree_new_relation(&t, 0, 1);
    tree_extend(&t, &$5, 0);

    $$ = t;                        
};
escritura_instruccion : KW_WRITE '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);

    assert_ident_is_printable(&$5);
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 0,
        }
    };

    Node * var = (Node *)tree_new_node(&t, NULL);
    *var = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    Node * var2 = (Node *)tree_new_node(&t, NULL);
    *var2 = (Node){
        .node_type = NExpr,
        .asoc_type = $5.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $5,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_new_relation(&t, 0, 2);

    $$ = t;

}
escritura_instruccion : KW_WRITELN '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);

    assert_ident_is_printable(&$5);
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 1,
        }
    };

    Node * var = (Node *)tree_new_node(&t, NULL);
    *var = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    Node * var2 = (Node *)tree_new_node(&t, NULL);
    *var2 = (Node){
        .node_type = NExpr,
        .asoc_type = $5.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $5,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_new_relation(&t, 0, 2);


    $$ = t;
};
escritura_instruccion : KW_WRITE '(' IDENT ')' { 
    assert_sym_exists(&$3);
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 0,
        }
    };

    Node * var = (Node *)tree_new_node(&t, NULL);
    *var = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    tree_new_relation(&t, 0, 1);

    $$ = t;
};
escritura_instruccion : KW_WRITELN '(' IDENT ')' { 
    assert_sym_exists(&$3);
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 1,
        }
    };

    Node * ident = (Node *)tree_new_node(&t, NULL);
    *ident = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    tree_new_relation(&t, 0, 1);

    $$ = t;
};
escritura_instruccion : KW_WRITE '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
    assert_node_is_printable(tree_get_root(&$5));
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 0,
        }
    };

    Node * ident = (Node *)tree_new_node(&t, NULL);
    *ident = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_extend(&t, &$5, 0);    

    $$ = t;
};
escritura_instruccion : KW_WRITELN '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
    assert_node_is_printable(tree_get_root(&$5));
    assert_ident_is_printable(&$3);

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NWrite,
        .asoc_type = Void,
        .value.write = (WriteNode){
            .newline = 1,
        }
    };

    Node * ident = (Node *)tree_new_node(&t, NULL);
    *ident = (Node){
        .node_type = NExpr,
        .asoc_type = $3.asoc_type.type,
        .value.expr = (ExprNode){
            .type = ESymbol,
            .value.symbol = $3,
        }
    };

    tree_new_relation(&t, 0, 1);
    tree_extend(&t, &$5, 0);

    $$ = t;
};
if_instruccion : KW_IF relop_expresion KW_THEN instrucciones {
    assert_expr_type(tree_get_root(&$2), tree_get_root(&$4));

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NIf,
        .asoc_type = Void,
        .value.ifn = (IfNode){
            .blocks = 1
        }
    };

    tree_extend(&t, &$2, 0);
    tree_extend(&t, &$4, 0);

    $$ = t;
} |
      KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones {
    
    assert_expr_type(tree_get_root(&$2), tree_get_root(&$4));

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NIf,
        .asoc_type = Void,
        .value.ifn = (IfNode){
            .blocks = 2
        }
    };

    tree_extend(&t, &$2, 0);
    tree_extend(&t, &$4, 0);
    tree_extend(&t, &$6, 0);

    $$ = t;
};

/* Asignacion */
variable_asignacion : variable OP_ASIGN expresion {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * var = tree_get_root(&$1);
    Node * expr = tree_get_root(&$3);

    assert_variable_assigned_type(var, expr);

    Tree assign;
    tree_init(&assign, sizeof(Node));

    Node * n = (Node *)tree_new_node(&assign, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = var->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = OpAssign,
        }
    };

    tree_extend(&assign, &$1, 0);
    tree_extend(&assign, &$3, 0);

    $$ = assign;
};

for_asignacion : variable_asignacion | variable;

variable : IDENT { 
    Symbol * s = assert_sym_exists(&$1);
    
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = ESymbol,
            .value.symbol = *s,
        },
        .asoc_type = s->asoc_type.type
    };

    $$ = t;
};
variable : IDENT '[' CONST_ENTERA ']' { 
    Symbol * s = assert_sym_exists(&$1); 

    size_t arr_size = s->asoc_type.size;
    if ((int64_t)arr_size < $3 || $3 < 0) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Indice fuera de rango: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
        str_push_sizet(&wrn_buff, arr_size);
        str_push(&wrn_buff, " y se intento acceder a la posicion ");
        str_push_sizet(&wrn_buff, $3);
        yyerror(str_as_ref(&wrn_buff));
    }

    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = ESymbolIdx,
            .value.symbol_idx = (IndexedSymbol) {
                .symbol = *s,
                .index = $3
            }
        },
        .asoc_type = s->asoc_type.type
    };

    $$ = t;
};

procedure_instruccion : IDENT { 
    Symbol * s = assert_sym_exists(&$1);
    if (s->type != Procedure) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento llamar a una variable como si fuera una funcion: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }

    if (s->info.fun.args.len != 0) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos sin pasarle argumentos: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }
    
    Tree t;
    tree_init(&t, sizeof(Tree));
    $$ = (FunctionCall) {
        .symbol = $1,
        .args = t
    };
};

procedure_instruccion : IDENT '(' expresion_lista ')' {
    Symbol * s = assert_sym_exists(&$1);

    Vec children = tree_get_childs(&$3, 0);

    // Checamos que los tipos de datos de los argumentos hacen match con los del simbolo 
    if (s->info.fun.args.len != children.len) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento llamar a una funcion con una cantidad de argumentos distinta a la declarada: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        str_push(&wrn_buff, ", se esperaban ");
        str_push_sizet(&wrn_buff, s->info.fun.args.len);
        str_push(&wrn_buff, " argumentos y se pasaron ");
        str_push_sizet(&wrn_buff, children.len);
        yyerror(str_as_ref(&wrn_buff));
    }


    if (children.len > 0) {
        Node * void_root = (Node *)vec_get(&$3.values, 0);

        if (void_root->node_type != NVoid) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos sin pasarle argumentos: ");
            str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
            Node * arg = (Node *)vec_get(&$3.values, *(size_t *)vec_get(&children, n_arg));
            Symbol * arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

            if (arg->asoc_type != arg_sym->asoc_type.type) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos de tipos distintos a los declarados: ");
                str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
                str_push(&wrn_buff, ", el argumento ");
                str_push_sizet(&wrn_buff, n_arg + 1);
                str_push(&wrn_buff, " se esperaba de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(&arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(&arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

        vec_drop(&children);
    }

    $$ = (FunctionCall) {
        .symbol = $1,
        .args = $3,
        .return_type = s->asoc_type.type,
    };
};

/* Relop */
relop_expresion : relop_expresion RELOP_OR relop_and {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$1);
    Node * rhs = tree_get_root(&$3);

    assert_expr_type(lhs, rhs);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = lhs->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = OpOr,
        }
    };

    tree_extend(&t, &$1, 0);
    tree_extend(&t, &$3, 0);

    $$ = t;
} | relop_and;
relop_and : relop_and RELOP_AND relop_not {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$1);
    Node * rhs = tree_get_root(&$3);

    assert_expr_type(lhs, rhs);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = lhs->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = OpAnd,
        }
    };

    tree_extend(&t, &$1, 0);
    tree_extend(&t, &$3, 0);

    $$ = t;
} | relop_not {
    $$ = $1;
};
relop_not : RELOP_NOT relop_not {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$2);
    Node * n = (Node *)tree_new_node(&t, NULL);

    *n = (Node){
        .node_type = NExpr,
        .asoc_type = lhs->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = OpNot,
        }
    };

    tree_extend(&t, &$2, 0);

    $$ = t;
} | relop_paren;
relop_paren : '(' relop_expresion ')' {
    $$ = $2;
} | relop_expresion_simple;
relop_expresion_simple : expresion relop expresion {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$1);
    Node * rhs = tree_get_root(&$3);

    assert_expr_type(lhs, rhs);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = Bool,
        .value.expr = $2,
    };

    tree_extend(&t, &$1, 0);
    tree_extend(&t, &$3, 0);

    $$ = t;
};
relop : RELOP_AND {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpAnd
    };
} | RELOP_OR {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpOr
    };
} | RELOP_BT { 
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpBt
    };
} | RELOP_LT {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpLt
    };
} | RELOP_EBT  {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpEbt
    };
} | RELOP_ELT {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpElt
    };
} | RELOP_EQ {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpEq
    };
} | RELOP_NEQ {
    $$ = (ExprNode) {
        .type = EOp,
        .value.op = OpNeq
    };
};

/* Expresion */
expresion_lista : expresion {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * void_root = (Node *)tree_new_node(&t, NULL);
    *void_root = (Node) {
        .node_type = NVoid,
        .id = 0,
    };

    tree_extend(&t, &$1, 0);

    $$ = t;
} | expresion_lista ',' expresion {
    $$ = $1;
    tree_extend(&$$, &$3, 0);
} | {
    Tree t;
    tree_init(&t, sizeof(Node));

    $$ = t;
};
expresion: termino | expresion ADDOP termino {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$1);
    Node * rhs = tree_get_root(&$3);
    
    assert_expr_type(lhs, rhs);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = lhs->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = $2,
        }
    };

    tree_extend(&t, &$1, 0);
    tree_extend(&t, &$3, 0);

    $$ = t;
};
termino : factor | termino MULOP factor {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * lhs = tree_get_root(&$1);
    Node * rhs = tree_get_root(&$3);

    assert_expr_type(lhs, rhs);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = lhs->asoc_type,
        .value.expr = (ExprNode){
            .type = EOp,
            .value.op = $2,
        }
    };

    tree_extend(&t, &$1, 0);
    tree_extend(&t, &$3, 0);

    $$ = t;
};
llamado_funcion : IDENT '(' expresion_lista ')' { 
    Symbol * s = assert_sym_exists(&$1);
    if (s->type != Function) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento llamar a una variable como si fuera una funcion: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }

    Vec children = tree_get_childs(&$3, 0);

    // Checamos que los tipos de datos de los argumentos hacen match con los del simbolo 
        if (s->info.fun.args.len != children.len) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con una cantidad de argumentos distinta a la declarada: ");
            str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
            str_push(&wrn_buff, ", se esperaban ");
            str_push_sizet(&wrn_buff, s->info.fun.args.len);
            str_push(&wrn_buff, " argumentos y se pasaron ");
            str_push_sizet(&wrn_buff, children.len);
            yyerror(str_as_ref(&wrn_buff));
        }

    if (children.len > 0) {
        Node * void_root = (Node *)vec_get(&$3.values, 0);

        if (void_root->node_type != NVoid) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos sin pasarle argumentos: ");
            str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        
        for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
            Node * arg = tree_get_node(&$3, *(size_t *)vec_get(&children, n_arg));
            Symbol * arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

            if (arg->asoc_type != arg_sym->asoc_type.type) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos de tipos distintos a los declarados: ");
                str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
                str_push(&wrn_buff, ", el argumento ");
                str_push_sizet(&wrn_buff, n_arg + 1);
                str_push(&wrn_buff, " se esperaba de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(&arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(&arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

        vec_drop(&children);
    }

    $$ = (FunctionCall) {
        .symbol = $1,
        .args = $3,
        .return_type = s->asoc_type.type
    };
};
factor : IDENT { 
    Tree t;
    tree_init(&t, sizeof(Node));

    assert_sym_exists(&$1);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = ESymbol,
            .value.symbol = $1,
        },
        .asoc_type = $1.asoc_type.type
    };

    $$ = t;
};
factor : IDENT '[' CONST_ENTERA ']' { 
    Tree t;
    tree_init(&t, sizeof(Node));

    Symbol * s = assert_sym_exists(&$1); 

    size_t arr_size = s->asoc_type.size;
    if ((int64_t)arr_size < $3 || $3 < 0) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Indice fuera de rango: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
        str_push_sizet(&wrn_buff, arr_size);
        str_push(&wrn_buff, " y se intento acceder a la posicion ");
        str_push_sizet(&wrn_buff, $3);
        yyerror(str_as_ref(&wrn_buff));
    }

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = ESymbolIdx,
            .value.symbol_idx = (IndexedSymbol) {
                .symbol = $1,
                .index = $3
            }
        },
        .asoc_type = s->asoc_type.type
    };

    $$ = t;
};
factor : llamado_funcion {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = EFunctionCall,
            .value.function_call = $1,
        },
        .asoc_type = $1.return_type
    };

    if ($1.return_type == Void) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento usar una funcion que devuelve () como expresion: ");
        str_push_n(&wrn_buff, $1.symbol.name.ptr, $1.symbol.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }

    $$ = t;
};
factor : CONST_ENTERA {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = EIntValue,
            .value.int_value = $1,
        },
        .asoc_type = Int
    };

    $$ = t;
};
factor : CONST_REAL {
    Tree t;
    tree_init(&t, sizeof(Node));

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .value.expr = (ExprNode) {
            .type = ERealValue,
            .value.real_value = $1,
        },
        .asoc_type = Real
    };

    $$ = t;
};
factor : ADDOP factor {
    Tree t;

    tree_init(&t, sizeof(Node));

    Node * past_root = tree_get_root(&$2);

    Node * n = (Node *)tree_new_node(&t, NULL);
    *n = (Node){
        .node_type = NExpr,
        .asoc_type = past_root->asoc_type,
        .value.expr = (ExprNode){
            .type = EUnaryOp,
            .value.op = $1,
        }
    };

    tree_extend(&t, &$2, 0);

    $$ = t;
};
factor : '(' expresion ')' {
    $$ = $2;
};
%%
