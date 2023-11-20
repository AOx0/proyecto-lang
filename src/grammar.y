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

    void add_reference_to_sym(Symbol * s, size_t line) {
        size_t *ref = vec_push(&s->refs);
        *ref = line;
    }

    void *assert_sym_exists(Symbol * s) {
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
            char lit[] = "Simbolo no declarado en el scope actual: ";
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
            char lit[] = "Simbolo ya declarado en el mismo scope: ";
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("No existe %zu(%zu):  %.*s\n", s->line, s->scope,
            // (int)s->name.len, s->name.ptr);
        }
        nchar = tnchar;
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
    AddOp addop;
    MulOp mulop;
    Tree subtree;
    ExprNode expr;
    FunctionCall function_call;
}

/* Ident */
%token <symbol>IDENT;

/* Par */
%token <fnum>CONST_REAL <snum>CONST_ENTERA <slice>CONST_CADENA 
    <relop>RELOP <addop>ADDOP <mulop>MULOP;

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
%type <idents>decl;
%type <idents>decl_const;
%type <idents>decl_var;
%type <subtree>expresion;
%type <expr>factor;
%type <function_call>llamado_funcion;

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
        s->info.var = (VariableInfo){.type = (DataType){ .type = Str, .size=1 }, .addr = addr};
        hashset_insert(&tabla, s);
    }

    $3.type = Function;
    $3.info.fun = (FunctionInfo) {
        .args = $5,
        .return_type = Void
    };
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);

    size_t idx;
    Node * node = (Node *)tree_new_node(&ast, &idx);
    *node = (Node) {
        .node_type = NVoid,
        .id = idx,
        .value = {}
    };

    size_t child_idx;
    node = (Node *)tree_new_node(&ast, &child_idx);
    *node = (Node) {
        .node_type = NProgram,
        .id = child_idx,
        .value.fun = (FunctionNode) { .name = $3.name, .args = $5, .return_type = Void },
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


    size_t i = 0;
    while (tabla.elements > i) {
        for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
            Vec *arr = (Vec *)vec_get(&tabla.values, j);
            for (size_t h = 0; h < arr->len; h++) {
                Symbol *s = (Symbol *)vec_get(arr, h);

                #ifdef PRINT_TABLE
                    sym_type_display(s->type);
                    printf(" (%.2zu,%.2zu), name: %10.*s, location: %2zu:%-2zu, "
                           "scope: %zu, refs: { ",
                           j, h, (int)s->name.len, s->name.ptr, s->line, s->nchar,
                           s->scope);
                    for (size_t i = 0; i < s->refs.len; i++) {
                        size_t *ref = (size_t *)vec_get(&s->refs, i);
                        printf("%zu", *ref);
                        if (i + 1 != s->refs.len) {
                            printf(", ");
                        }
                    }
                    printf(" }, info: ");

                    switch (s->type) {
                        case Function: { fun_info_debug(&s->info.fun); break; }
                        case Variable: { var_info_debug(&s->info.var); break; }
                        case Constant: { const_info_debug(&s->info.cons); break; }
                        case Procedure: { fun_info_debug(&s->info.fun); break; }
                        default: { puts("Panic: Invalid SymbolType"); exit(1); } 
                    }
                puts("");
                #endif
                i += 1;
                // vec_drop(&s->refs);
            }
        }
    }

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
decl : decl_var {
    $$ = $1;
} | decl_const {
    $$ = $1;
} | { $$ = vec_new(sizeof(Symbol)); };

decl_var : decl KW_VAR ident_lista ':' tipo ';' {
    for (size_t i = 0; i < $3.len; i++) {
        Symbol *s = vec_get(&$3, i);
        s->type = Variable;
        s->info.var = (VariableInfo){.type = $5, .addr = addr};
        addr += data_type_size(&$5);
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
    }

    $$ = $1;
    vec_extend(&$$, &$3);
};

decl_const : decl KW_CONST IDENT '=' CONST_ENTERA ';' {
    $3.type = Constant;
    $3.info.cons = (ConstantInfo){
        .type = (DataType){
            .type = Int, 
            .size = 1
        },
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
    $3.info.cons = (ConstantInfo){
        .type = (DataType){
            .type = Real, 
            .size = 1
        },
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
    $3.info.cons = (ConstantInfo){
        .type = (DataType){
            .type = Str, 
            .size = $5.len
        }, 
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
    s->info.fun = (FunctionInfo){.return_type = $6, .args=$4};
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
    s->info.fun = (FunctionInfo){ .return_type = (DataType) { .type = Void, .size = 0 }, .args=$4};
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
        s->info.var = (VariableInfo){.type = $3, .addr = addr};
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
        s->info.var = (VariableInfo){.type = $5, .addr = addr};
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
instrucciones
    : variable_asignacion |
      procedure_instruccion | instruccion_compuesta | if_instruccion |
      repeticion_instruccion | lectura_instruccion | escritura_instruccion;
repeticion_instruccion
    : KW_WHILE relop_expresion KW_DO instrucciones |
      KW_FOR for_asignacion KW_TO expresion KW_DO instrucciones |
      KW_FOR for_asignacion KW_DOWNTO expresion KW_DO instrucciones;
lectura_instruccion : KW_READ '(' IDENT ')' { assert_sym_exists(&$3); };
lectura_instruccion : KW_READLN '(' IDENT ')' { assert_sym_exists(&$3); };
escritura_instruccion : KW_WRITE '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
};
escritura_instruccion : KW_WRITELN '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
};
escritura_instruccion : KW_WRITE '(' CONST_CADENA ')' |
                        KW_WRITELN '(' CONST_CADENA ')' |
                        KW_WRITE '(' CONST_CADENA ',' expresion ')' |
                        KW_WRITELN '(' CONST_CADENA ',' expresion ')';
escritura_instruccion : KW_WRITE '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);
}
escritura_instruccion : KW_WRITELN '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);
};
escritura_instruccion : KW_WRITE '(' IDENT ')' { assert_sym_exists(&$3); };
escritura_instruccion : KW_WRITELN '(' IDENT ')' { assert_sym_exists(&$3); };
escritura_instruccion : KW_WRITE '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
};
escritura_instruccion : KW_WRITELN '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
};
if_instruccion
    : KW_IF relop_expresion KW_THEN instrucciones |
      KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones;

/* Asignacion */
variable_asignacion : variable OP_ASIGN expresion;
for_asignacion : variable_asignacion | variable;
variable : IDENT { assert_sym_exists(&$1); };
variable : IDENT '[' CONST_ENTERA ']' { assert_sym_exists(&$1); };
procedure_instruccion : IDENT { assert_sym_exists(&$1); };
procedure_instruccion : IDENT '(' expresion_lista ')' {
    assert_sym_exists(&$1);
};

/* Relop */
relop_expresion : relop_expresion RELOP_OR relop_and | relop_and;
relop_and : relop_and RELOP_AND relop_not | relop_not;
relop_not : RELOP_NOT relop_not | relop_paren;
relop_paren : '(' relop_expresion ')' | relop_expresion_simple;
relop_expresion_simple : expresion relop expresion;
relop : RELOP_AND | RELOP_OR | RELOP_BT | RELOP_LT | RELOP_EBT | RELOP_ELT |
        RELOP_EQ | RELOP_NEQ;

/* Expresion */
expresion_lista : expresion | expresion_lista ',' expresion;
expresion : termino | expresion ADDOP termino;
termino : factor | termino MULOP factor;
llamado_funcion : IDENT '(' expresion_lista ')' { 
    Symbol * s = (Symbol *)assert_sym_exists(&$1);
    if (s->type != Function) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Se intento llamar a una variable como si fuera una funcion: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        yyerror(str_as_ref(&wrn_buff));
    }

    $$ = (FunctionCall) {
        .symbol = $1,
        .args = vec_new(sizeof(ExprNode))
    };
};
factor : IDENT { 
    assert_sym_exists(&$1);
    $$ = (ExprNode) {
        .type = ESymbol,
        .value.symbol = $1
    };
};
factor : IDENT '[' CONST_ENTERA ']' { 
    Symbol * s = (Symbol *)assert_sym_exists(&$1); 

    size_t arr_size = s->info.var.type.size;
    if (arr_size < $3 || $3 < 0) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Indice fuera de rango: ");
        str_push_n(&wrn_buff, $1.name.ptr, $1.name.len);
        str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
        str_push_sizet(&wrn_buff, arr_size);
        str_push(&wrn_buff, " y se intento acceder a la posicion ");
        str_push_sizet(&wrn_buff, $3);
        yyerror(str_as_ref(&wrn_buff));
    }

    $$ = (ExprNode) {
        .type = ESymbolIdx,
        .value.symbol_idx = (IndexedSymbol) {
            .symbol = $1,
            .index = $3
        }
    };
};
factor : llamado_funcion {
    $$ = (ExprNode) {
        .type = EFunctionCall,
        .value.function_call = $1,
        .asoc_type = $1.symbol.info.fun.return_type.type
    };
};
factor : CONST_ENTERA {
    $$ = (ExprNode) {
        .type = EIntValue,
        .value.int_value = $1,
        .asoc_type = Int
    };
};
factor : CONST_REAL {
    $$ = (ExprNode) {
        .type = ERealValue,
        .value.real_value = $1,
        .asoc_type = Real
    };
};
factor : ADDOP factor {
    $$ = $2;
    switch ($1) {
        case Add: {
            $$.value.int_value = $$.value.int_value * +1;
            break;
        }
        case Sub: {
            $$.value.int_value = $$.value.int_value * -1;
            break;
        }
        default: {
            puts("Panic: Invalid AddOp");
            exit(1);
        }
    }
};
factor : '(' expresion ')' {
    
};
%%
