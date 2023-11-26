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

    Node * ast_get_root(Tree *t) {
        return (Node *)vec_get(&t->values, 0);
    }

    Node * ast_get_node(Tree *t, size_t id) {
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
            str_push(&wrn_buff, data_type_e_display_return(var->asoc_type));
            str_push(&wrn_buff, " y la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(val->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_expr_type(Node *a, Node *b) {
        if (a->asoc_type != b->asoc_type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento realizar operaciones entre expresiones de tipos distintos: ");
            str_push(&wrn_buff, "El primer operando es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(a->asoc_type));
            str_push(&wrn_buff, " y el segundo es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(b->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_node_is_printable(Node *n) {
        if (n->asoc_type == Void || n->asoc_type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir una expresion de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_ident_is_printable(Symbol *s) {
        if (s->asoc_type.type == Void || s->asoc_type.type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir un identificador de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(s->asoc_type.type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_tree_asoc_type_is(Tree t, DataTypeE type) {
        Node *n = ast_get_root(&t);

        if (n->asoc_type != type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se esperaba una expresion de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(type));
            str_push(&wrn_buff, " pero se recibio una expresion de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
        }
    }

    void assert_tree_node_type_is(Tree t, NodeType type) {
        Node *n = ast_get_root(&t);

        if (n->node_type != type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se esperaba ");
            str_push(&wrn_buff, node_type_display(type));
            str_push(&wrn_buff, " pero se recibió ");
            str_push(&wrn_buff, node_type_display(n->node_type));
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
    Vec instructions;
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
%type <subtree>procedure_instruccion;
%type <subtree>instrucciones_opcionales;
%type <instructions>instrucciones_lista;
%type <subtree>subprogramas;
%type <subtree>subprograma_declaracion;
%type <symbol>subprograma_encabezado;

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
KW_PROG IDENT '(' ident_lista ')' ';' decl subprogramas
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

    Node * root = ast_create_node(&ast);
    root->node_type = NRoot;
    root->asoc_type = Void;

    for (size_t i = 0; i < $8.len; i++) {
        Symbol *s = vec_get(&$8, i);
        Node * node = ast_create_node(&ast);

        if (s->type == Variable) { 
            node->node_type = NVar;
            node->value.var = (VarNode) { .symbol = *s };
        } else {
            node->node_type = NConst;
            node->value.cons = (ConstNode) { .symbol = *s, .value.bool = 1 };
        }

        tree_new_relation(&ast, 0, node->id); 
    }

    tree_extend_with_subtree(&ast, &$9, 0, 0);

    Tree program;
    tree_init(&program, sizeof(Node));

    Node * n = ast_create_node(&program);
    n->node_type = NFunction;
    n->asoc_type = Void;
    n->value.fun = (FunctionNode) {
        .symbol = $3,
        .args = $5,
    };
    
    tree_extend_with_subtree(&program, &$10, 0, 0);
    tree_extend_with_subtree(&ast, &program, 0, 0);
   
    

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

    flip_tree_relations(&ast);

    #ifdef PRINT_TREE
        tree_debug(&ast, &tabla);
    #endif

    if (err == 0)  node_display_id(0, OUT_FILE, &ast, &tabla, 0);

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
subprogramas : subprogramas subprograma_declaracion ';' {
    $$ = $1;
    tree_extend_with_subtree(&$$, &$2, 0, 0);
} | {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NVoid;
    n->asoc_type = Void;
};
subprograma_declaracion
    : subprograma_encabezado decl subprogramas instruccion_compuesta {
    scope -= fun_id;

    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NFunction;
    n->asoc_type = Void;
    n->value.fun = (FunctionNode) {
        .symbol = $1,
        .args = $2,
    };

    tree_extend_with_subtree(&$$, &$3, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
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
    $$ = *s;
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
    $$ = *s;
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
instruccion_compuesta : KW_BEGIN instrucciones_opcionales KW_END {
    $$ = $2;
};
instrucciones_opcionales : instrucciones_lista {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NVoid;
    n->asoc_type = Void;

    for (size_t i = 0; i < $1.len; i++) {
        Tree *t2 = (Tree *)vec_get(&$1, i);

        if (t2->values.len == 0)
            continue;

        tree_extend_with_subtree(&$$, t2, 0, 0);
    }
} | {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NVoid;
    n->asoc_type = Void;
}
instrucciones_lista : instrucciones {
    vec_init(&$$, sizeof(Tree));

    Tree * sub = vec_push(&$$);
    *sub = $1;
} | instrucciones_lista ';' instrucciones {
    $$ = $1;
    vec_push(&$$);

    Tree * sub = vec_push(&$$);
    *sub = $3;
};
instrucciones: variable_asignacion |
      procedure_instruccion | instruccion_compuesta | if_instruccion |
      repeticion_instruccion | lectura_instruccion | escritura_instruccion;

/* Loops */
repeticion_instruccion: KW_WHILE relop_expresion KW_DO instrucciones {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWhile;
    n->asoc_type = Void;

    assert_tree_asoc_type_is($2, Bool);

    tree_extend_with_subtree(&$$, &$2, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
} | KW_FOR for_asignacion KW_TO expresion KW_DO instrucciones {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NFor;
    n->asoc_type = Void;
    n->value.forn = (ForNode) {
        .down = 0,
    };

    tree_extend_with_subtree(&$$, &$2, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
} | KW_FOR for_asignacion KW_DOWNTO expresion KW_DO instrucciones {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NFor;
    n->asoc_type = Void;
    n->value.forn = (ForNode) {
        .down = 1,
    };

    tree_extend_with_subtree(&$$, &$2, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
};

/* Lectura */
lectura_instruccion : KW_READ '(' IDENT ')' { 
    assert_sym_exists(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NRead;
    n->asoc_type = Void;
    n->value.read = (ReadNode){
        .newline = 0,
        .target_symbol = $3,
    };
};
lectura_instruccion : KW_READLN '(' IDENT ')' { 
    assert_sym_exists(&$3); 
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NRead;
    n->asoc_type = Void;
    n->value.read = (ReadNode){
        .newline = 1,
        .target_symbol = $3,
    };
};

/* Escritura */
escritura_instruccion : KW_WRITE '(' CONST_CADENA ',' IDENT ')' {
    Symbol * s = assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 0,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    Node * var = ast_create_node(&$$);
    var->node_type = NExpr;
    var->asoc_type = $5.asoc_type.type;
    var->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_new_relation(&$$, 0, 2);
};
escritura_instruccion : KW_WRITELN '(' CONST_CADENA ',' IDENT ')' {
    Symbol * s = assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 1,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    Node * var = ast_create_node(&$$);
    var->node_type = NExpr;
    var->asoc_type = $5.asoc_type.type;
    var->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_new_relation(&$$, 0, 2);
};

escritura_instruccion : KW_WRITE '(' CONST_CADENA ')'{
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 0,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    tree_new_relation(&$$, 0, 1);
} | KW_WRITELN '(' CONST_CADENA ')' {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 1,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    tree_new_relation(&$$, 0, 1);
 } | KW_WRITE '(' CONST_CADENA ',' expresion ')' {
    assert_node_is_printable(ast_get_root(&$5));
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 0,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    tree_new_relation(&$$, 0, 1);
    tree_extend_with_subtree(&$$, &$5, 0, 0);
}
 | KW_WRITELN '(' CONST_CADENA ',' expresion ')' {
    assert_node_is_printable(ast_get_root(&$5));
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode) {
        .newline = 1,
    };

    Node * str = ast_create_node(&$$);
    str->node_type = NConst;
    str->asoc_type = Str;
    str->value.cons = (ConstNode) { .symbol = $3, .value.str = $3 };

    tree_new_relation(&$$, 0, 1);
    tree_extend_with_subtree(&$$, &$5, 0, 0);
};

escritura_instruccion : KW_WRITE '(' IDENT ',' IDENT ')' {
    Symbol * s3 = assert_sym_exists(&$3);
    Symbol * s = assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);
    assert_ident_is_printable(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 0,
    };

    Node * var = ast_create_node(&$$);
    var->node_type = NExpr;
    var->asoc_type = $3.asoc_type.type;
    var->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s3,
    };

    Node * var2 = ast_create_node(&$$);
    var2->node_type = NExpr;
    var2->asoc_type = $5.asoc_type.type;
    var2->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_new_relation(&$$, 0, 2);
};

escritura_instruccion : KW_WRITELN '(' IDENT ',' IDENT ')' {
    Symbol * s3 =assert_sym_exists(&$3);
    Symbol * s = assert_sym_exists(&$5);
    assert_ident_is_printable(&$5);
    assert_ident_is_printable(&$3);

    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 1,
    };

    Node * var = ast_create_node(&$$);
    var->node_type = NExpr;
    var->asoc_type = $3.asoc_type.type;
    var->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s3,
    };

    Node * var2 = ast_create_node(&$$);
    var2->node_type = NExpr;
    var2->asoc_type = $5.asoc_type.type;
    var2->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_new_relation(&$$, 0, 2);
};

escritura_instruccion : KW_WRITE '(' IDENT ')' { 
    Symbol * s = assert_sym_exists(&$3);
    assert_ident_is_printable(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 0,
    };

    Node * var = ast_create_node(&$$);
    var->node_type = NExpr;
    var->asoc_type = $3.asoc_type.type;
    var->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
};

escritura_instruccion : KW_WRITELN '(' IDENT ')' { 
    Symbol * s = assert_sym_exists(&$3);
    assert_ident_is_printable(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 1,
    };


    Node * ident = ast_create_node(&$$);
    ident->node_type = NExpr;
    ident->asoc_type = $3.asoc_type.type;
    ident->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
};

escritura_instruccion : KW_WRITE '(' IDENT ',' expresion ')' {
    Symbol * s = assert_sym_exists(&$3);
    assert_node_is_printable(ast_get_root(&$5));
    assert_ident_is_printable(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 0,
    };

    Node * ident = ast_create_node(&$$);
    ident->node_type = NExpr;
    ident->asoc_type = $3.asoc_type.type;
    ident->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_extend_with_subtree(&$$, &$5, 0, 0);    
};
escritura_instruccion : KW_WRITELN '(' IDENT ',' expresion ')' {
    Symbol * s = assert_sym_exists(&$3);
    assert_node_is_printable(ast_get_root(&$5));
    assert_ident_is_printable(&$3);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NWrite;
    n->asoc_type = Void;
    n->value.write = (WriteNode){
        .newline = 1,
    };

    Node * ident = ast_create_node(&$$);
    ident->node_type = NExpr;
    ident->asoc_type = $3.asoc_type.type;
    ident->value.expr = (ExprNode){
        .type = ESymbol,
        .value.symbol = *s,
    };

    tree_new_relation(&$$, 0, 1);
    tree_extend_with_subtree(&$$, &$5, 0, 0);
};
if_instruccion : KW_IF relop_expresion KW_THEN instrucciones {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NIf;
    n->asoc_type = Void;
    n->value.ifn = (IfNode){
        .blocks = 1
    };

    tree_extend_with_subtree(&$$, &$2, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
} | KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NIf;
    n->asoc_type = Void;
    n->value.ifn = (IfNode){
        .blocks = 2
    };

    tree_extend_with_subtree(&$$, &$2, 0, 0);
    tree_extend_with_subtree(&$$, &$4, 0, 0);
    tree_extend_with_subtree(&$$, &$6, 0, 0);
};

/* Asignacion */
variable_asignacion : variable OP_ASIGN expresion {
    Node * var = ast_get_root(&$1);
    Node * expr = ast_get_root(&$3);

    assert_variable_assigned_type(var, expr);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NAssign;
    n->asoc_type = Void;

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
};

for_asignacion : variable_asignacion | variable;

variable : IDENT { 
    Symbol * s = assert_sym_exists(&$1);
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = s->asoc_type.type;
    n->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };
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

    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->value.expr = (ExprNode) {
        .type = ESymbolIdx,
        .value.symbol_idx = (IndexedSymbol) {
            .symbol = *s,
            .index = $3
        }
    };
    n->asoc_type = s->asoc_type.type;
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

    tree_init(&$$, sizeof(Node));

    Tree args;
    tree_init(&args, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NCall;
    n->asoc_type = s->asoc_type.type;
    n->value.call = (FunctionCall){
        .args = args,
        .symbol = *s,
        .return_type = s->asoc_type.type,
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
                str_push(&wrn_buff, data_type_e_display_return(arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

        vec_drop(&children);
    }

    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NCall;
    n->asoc_type = s->asoc_type.type;
    n->value.call = (FunctionCall){
        .symbol = *s,
        .args = $3,
    };
};

/* Relop */
relop_expresion : relop_expresion RELOP_OR relop_and {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$1);
    Node * rhs = ast_get_root(&$3);
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = lhs->asoc_type;
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = OpOr,
    };

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
} | relop_and;

relop_and : relop_and RELOP_AND relop_not {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$1);
    Node * rhs = ast_get_root(&$3);
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = lhs->asoc_type;
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = OpAnd,
    };

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
} | relop_not;

relop_not : RELOP_NOT relop_not {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$2);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = lhs->asoc_type;
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = OpNot,
    };

    tree_extend_with_subtree(&$$, &$2, 0, 0);
} | relop_paren;

relop_paren : '(' relop_expresion ')' { $$ = $2; } | relop_expresion_simple;

relop_expresion_simple : expresion relop expresion {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$1);
    Node * rhs = ast_get_root(&$3);
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = Bool;
    n->value.expr = $2;

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
};

relop : RELOP_AND { $$ = (ExprNode) { .type = EOp, .value.op = OpAnd }; } 
 | RELOP_OR { $$ = (ExprNode) { .type = EOp, .value.op = OpOr }; } 
 | RELOP_BT { $$ = (ExprNode) { .type = EOp, .value.op = OpBt }; } 
 | RELOP_LT { $$ = (ExprNode) { .type = EOp, .value.op = OpLt }; } 
 | RELOP_EBT  { $$ = (ExprNode) { .type = EOp, .value.op = OpEbt }; } 
 | RELOP_ELT { $$ = (ExprNode) { .type = EOp, .value.op = OpElt }; } 
 | RELOP_EQ { $$ = (ExprNode) { .type = EOp, .value.op = OpEq };} 
 | RELOP_NEQ { $$ = (ExprNode) { .type = EOp, .value.op = OpNeq }; };

/* Expresion */
expresion_lista : expresion {
    tree_init(&$$, sizeof(Node));

    Node * void_root = ast_create_node(&$$);
    *void_root = (Node) {
        .node_type = NVoid
    };

    tree_extend_with_subtree(&$$, &$1, 0, 0);
} | expresion_lista ',' expresion {
    $$ = $1;
    tree_extend_with_subtree(&$$, &$3, 0, 0);
} | {
    tree_init(&$$, sizeof(Node));
};
expresion: termino | expresion ADDOP termino {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$1);
    Node * rhs = ast_get_root(&$3);
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = lhs->asoc_type;
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = $2,
    };

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
};
termino : factor | termino MULOP factor {
    tree_init(&$$, sizeof(Node));

    Node * lhs = ast_get_root(&$1);
    Node * rhs = ast_get_root(&$3);
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = lhs->asoc_type;
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = $2,
    };

    tree_extend_with_subtree(&$$, &$1, 0, 0);
    tree_extend_with_subtree(&$$, &$3, 0, 0);
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
            Node * arg = ast_get_node(&$3, *(size_t *)vec_get(&children, n_arg));
            Symbol * arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

            if (arg->asoc_type != arg_sym->asoc_type.type) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos de tipos distintos a los declarados: ");
                str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
                str_push(&wrn_buff, ", el argumento ");
                str_push_sizet(&wrn_buff, n_arg + 1);
                str_push(&wrn_buff, " se esperaba de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

        vec_drop(&children);
    }

    $$ = (FunctionCall) {
        .symbol = *s,
        .args = $3,
        .return_type = s->asoc_type.type
    };
};
factor : IDENT { 
    tree_init(&$$, sizeof(Node));

    Symbol * s = assert_sym_exists(&$1);

    Node * n = ast_create_node(&$$);
    n->asoc_type = $1.asoc_type.type;
    n->node_type = NExpr;
    n->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };
};
factor : IDENT '[' CONST_ENTERA ']' { 
    tree_init(&$$, sizeof(Node));

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

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = s->asoc_type.type;
    n->value.expr = (ExprNode) {
        .type = ESymbolIdx,
        .value.symbol_idx = (IndexedSymbol) {
            .symbol = *s,
            .index = $3
        }
    };
};
factor : llamado_funcion {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = $1.return_type;
    n->value.expr = (ExprNode) {
        .type = EFunctionCall,
        .value.function_call = $1,
    };

    if ($1.return_type == Void) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento usar una funcion que devuelve () como expresion: ");
        str_push_n(&wrn_buff, $1.symbol.name.ptr, $1.symbol.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }
};
factor : CONST_ENTERA {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = Int;
    n->value.expr = (ExprNode) {
        .type = EIntValue,
        .value.int_value = $1,
    };
};
factor : CONST_REAL {
    tree_init(&$$, sizeof(Node));

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = Real;
    n->value.expr = (ExprNode) {
        .type = ERealValue,
        .value.real_value = $1,
    };
};
factor : ADDOP factor {
    tree_init(&$$, sizeof(Node));

    Node * past_root = ast_get_root(&$2);

    Node * n = ast_create_node(&$$);
    n->node_type = NExpr;
    n->asoc_type = past_root->asoc_type;
    n->value.expr = (ExprNode){
        .type = EUnaryOp,
        .value.op = $1,
    };

    tree_root_extend(&$$, &$2);
};
factor : '(' expresion ')' {
    $$ = $2;
};
%%
