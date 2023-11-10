%code top {
    #include "hashset.h"
    #include "str.h"
    #include "parser.h"

    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);
    extern size_t linea;
    extern size_t scope;
	size_t fun_id = 0;
	
	#define SYM(s) (Symbol){ .name = s, .scope = scope, .line = linea }

    HashSet tabla;
    String wrn_buff;
    
    HashIdx hash_symbol(void * s) {
        HashIdx res;
        res.idx = 0;

        Symbol *sy = (Symbol *)s;

        for (size_t i=0; i<sy->name.len && i < 5; i++) {
            res.idx += sy->name.ptr[i] * (i + 1);
        }

        res.idx *= sy->scope + 1;

        // printf("HASH: %zu FROM: %zu(%zu): %.*s\n", res.idx, sy->line, sy->scope, (int)sy->name.len, sy->name.ptr);
        
        return res;
    }

    void assert_sym_exists(Symbol * s) {
        if (!hashset_contains(&tabla, s)) {
            str_clear(&wrn_buff);
            char lit[] = "Error: Simbolo no declarado en el scope actual: ";
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope, (int)s->name.len, s->name.ptr);
        }
    }
    
    void assert_not_sym_exists(Symbol * s) {
        if (hashset_contains(&tabla, s)) {
            str_clear(&wrn_buff);
            char lit[] = "Error: Simbolo ya declarado en el mismo scope: ";
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("No existe %zu(%zu):  %.*s\n", s->line, s->scope, (int)s->name.len, s->name.ptr);
        }
    }
}

%code requires {

    #ifndef LNG_PARSERH
    #define LNG_PARSERH
    
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>
    #include "vector.h"
    #include "hashset.h"
    #include "str.h"

    struct Symbol {
        StrSlice name;
        size_t scope;
        size_t line;
    };
    typedef struct Symbol Symbol;

    extern FILE *yyin, *yyout;

    enum RelOp {
        And,
        Or
    };
    typedef enum RelOp RelOp;

    enum AddOp {
        Add,
        Sub
    };
    typedef enum AddOp AddOp;

    enum MulOp {
        Div,
        Mod,
        Mul
    };
    typedef enum MulOp MulOp;

    #endif
}

%union {
    int64_t snum;
    uint64_t unum;
    double fnum;
    char * ident;
    StrSlice slice;
    Vec idents;
    RelOp relop;
    AddOp addop;
    MulOp mulop;
}

/* Ident */
%token <slice> IDENT;

/* Par */
%token <fnum> CONST_REAL <snum> CONST_ENTERA <slice> CONST_CADENA <relop> RELOP <addop> ADDOP <mulop> MULOP;

/* Keywords */ 
%token OP_ASIGN KW_PROCEDURE KW_PROG KW_CONST KW_VAR KW_ARRAY KW_OF KW_FUNC KW_BEGIN KW_END KW_READ KW_READLN KW_WRITE KW_WRITELN KW_WHILE KW_FOR KW_DO KW_TO KW_DOWNTO KW_IF KW_THEN KW_ELSE;

/* OPs */
%token RELOP_AND RELOP_NOT RELOP_OR;

/* Types */
%token T_INT T_REAL T_STR T_BOOL;

%type <idents> ident_lista;
%type <idents> parametros_lista;

%destructor { 
    printf("Dropping ident_lista:  ");
    vec_debug_verbose(&$$);
    vec_drop(&$$);
} ident_lista;

%destructor { 
    printf("Dropping parametros_lista:  ");
    vec_debug_verbose(&$$);
    vec_drop(&$$);
} parametros_lista;

%start programa;

%%
programa: { 
    puts("Init symbol table");
    hashset_init(&tabla, sizeof(Symbol), hash_symbol); 
    puts("Init warning buffer");
    str_init(&wrn_buff);
} KW_PROG  IDENT '(' ident_lista ')' ';' decl subprograma_decl instruccion_compuesta '.' {
    printf("Programa: %.*s\n", $3.len, $3.ptr);
    printf("Entradas: %zu\n", $5.len);
    for (size_t i=0; i < $5.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$5, i);
        printf("    - %.*s\n", (int)str->len, str->ptr);
    }
	
	vec_drop(&$5);
};

ident_lista: IDENT ',' ident_lista {
    $$ = $3;
    // vec_debug_verbose(&$$);
    StrSlice *sl = vec_push(&$$);
    *sl = $1;
};
ident_lista: IDENT {
    $$ = vec_new(sizeof(StrSlice));
    // vec_debug_verbose(&$$);
    StrSlice *sl = vec_push(&$$);
    *sl = $1;
};

/* Declaration of var and const */
decl: decl_var | decl_const | ;

decl_var: decl KW_VAR ident_lista ':' tipo ';' {
    printf("Declarando variables: %zu\n", $3.len);
    for (size_t i=0; i < $3.len; i++) {
        Symbol s = SYM(*(StrSlice *)vec_get(&$3, i));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
    printf("    - %zu: %zu: %.*s\n", linea, scope, (int)s.name.len, s.name.ptr);
    }
	
	vec_drop(&$3);
};

decl_const: decl KW_CONST IDENT '=' CONST_ENTERA ';' {
    printf("Constante: %lld\n", $5);
};
decl_const: decl KW_CONST IDENT '=' CONST_REAL ';' {
    printf("Constante: %f\n", $5);
};
decl_const: decl KW_CONST IDENT '=' CONST_CADENA ';' {
    printf("Constante: %.*s\n", $5.len, $5.ptr);
};

 /* Tipo */
tipo: estandard_tipo | KW_ARRAY '[' CONST_ENTERA '.' '.' CONST_ENTERA ']' KW_OF estandard_tipo;
estandard_tipo: T_INT | T_REAL | T_STR | T_BOOL;

 /* Subprograma */
subprograma_decl: subprograma_decl subprograma_declaracion ';' | ;
subprograma_declaracion: subprograma_encabezado decl subprograma_decl instruccion_compuesta;
subprograma_encabezado: KW_FUNC IDENT {
    printf("Declarando funcion %.*s\n", (int)$2.len, $2.ptr);
    fun_id++;
	scope+=fun_id;
	Symbol s = SYM($2);
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
} argumentos ':' estandard_tipo ';' {
    scope-=fun_id;
    printf("Declarada %.*s\n", (int)$2.len, $2.ptr);
} ;
subprograma_encabezado: KW_PROCEDURE IDENT {
    fun_id++;
	scope+=fun_id;
    printf("Declarando procedure %.*s\n", (int)$2.len, $2.ptr);
	Symbol s = SYM($2);
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
} argumentos ';' {
    scope-=fun_id;
    printf("Declarada %.*s\n", (int)$2.len, $2.ptr);
};

/* Argumentos */
argumentos: '(' parametros_lista ')' {
    printf("Argumentos: %zu\n", $2.len);
    for (size_t i=0; i < $2.len; i++) {
        Symbol s = SYM(*(StrSlice *)vec_get(&$2, i));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
		printf("    - %.*s\n", (int)s.name.len, s.name.ptr);
    }
	
	vec_drop(&$2);
} | ;
parametros_lista: ident_lista ':' tipo {
    $$ = $1;
};
parametros_lista: parametros_lista ';' ident_lista ':' tipo {
    $$ = $1;
    vec_extend(&$$, &$3);
    vec_drop(&$3);  
}; 


 /* Instrucciones */
instruccion_compuesta: KW_BEGIN instrucciones_opcionales KW_END;
instrucciones_opcionales: instrucciones_lista | /* Empty */ ;
instrucciones_lista: instrucciones | instrucciones_lista ';' instrucciones;
instrucciones: variable_asignacion | procedure_instruccion | instruccion_compuesta 
    | if_instruccion | repeticion_instruccion | lectura_instruccion | escritura_instruccion
;
repeticion_instruccion: KW_WHILE relop_expresion KW_DO instrucciones
    | KW_FOR for_asignacion KW_TO expresion KW_DO instrucciones
	| KW_FOR for_asignacion KW_DOWNTO expresion KW_DO instrucciones
;
lectura_instruccion: KW_READ '(' IDENT ')' {
    Symbol s = SYM($3);
    assert_sym_exists(&s);
}; 
lectura_instruccion: KW_READLN '(' IDENT ')' { 
    Symbol s = SYM($3);
    assert_sym_exists(&s);
};
escritura_instruccion: KW_WRITE '(' CONST_CADENA ',' IDENT ')' | KW_WRITELN '(' CONST_CADENA ',' IDENT ')' {
    Symbol s = SYM($5);
    assert_sym_exists(&s);
};
escritura_instruccion: KW_WRITE '(' CONST_CADENA  ')' | KW_WRITELN '(' CONST_CADENA  ')'
	| KW_WRITE '(' CONST_CADENA ',' expresion ')' | KW_WRITELN '(' CONST_CADENA ',' expresion ')';
escritura_instruccion: KW_WRITE '(' IDENT ',' IDENT ')' | KW_WRITELN '(' IDENT ',' IDENT ')' {
    Symbol s = SYM($3);
    Symbol s1 = SYM($5);
    assert_sym_exists(&s);
    assert_sym_exists(&s1);
};
escritura_instruccion: KW_WRITE '(' IDENT  ')' {
    Symbol s = SYM($3);
    assert_sym_exists(&s);
};
escritura_instruccion: KW_WRITELN '(' IDENT  ')' {
    Symbol s = SYM($3);
    assert_sym_exists(&s);
};
escritura_instruccion: KW_WRITE '(' IDENT ',' expresion ')' {
    Symbol s = SYM($3);
    assert_sym_exists(&s);
};
escritura_instruccion: KW_WRITELN '(' IDENT ',' expresion ')' {
    Symbol s = SYM($3);
    assert_sym_exists(&s);
};
if_instruccion: KW_IF relop_expresion KW_THEN instrucciones
    | KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones;

 /* Asignacion */
variable_asignacion: variable OP_ASIGN expresion; 
for_asignacion: variable_asignacion | variable;
variable: IDENT | IDENT '[' expresion ']' {
    Symbol s = SYM($1);
    assert_sym_exists(&s);
};
procedure_instruccion : IDENT | IDENT '(' expresion_lista ')';

 /* Relop */
relop_expresion: relop_expresion RELOP_OR relop_and | relop_and;
relop_and: relop_and RELOP_AND relop_not | relop_not ;
relop_not: RELOP_NOT relop_not | relop_paren;
relop_paren: '(' relop_expresion ')' | relop_expresion_simple;
relop_expresion_simple: expresion relop expresion;
relop: RELOP_AND | RELOP_OR ;

 /* Expresion */
expresion_lista: expresion | expresion_lista ',' expresion;
expresion: termino | expresion ADDOP termino;
termino: factor | termino MULOP factor;
llamado_funcion : IDENT '(' expresion_lista ')';
factor: IDENT | IDENT '[' expresion ']' | llamado_funcion | CONST_ENTERA | CONST_REAL | ADDOP factor | '(' expresion ')';
%%
