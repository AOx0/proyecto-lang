%code top {
    #include "hashset.h"
    #include "str.h"
    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);

    struct Symbol {
        StrSlice name;
        size_t line;
    };

    typedef struct Symbol Symbol;
    HashSet tabla;
    HashIdx hash_symbol(void * s) {
        HashIdx res;
        res.idx = 0;

        Symbol *sy = (Symbol *)s;

        for (size_t i=0; i<sy->name.len; i++) {
            res.idx += sy->name.ptr[i] * (i + 1);
        }
        
        return res;
    }
}

%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>
    #include "vector.h"
    #include "hashset.h"
    #include "str.h"

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
    printf("Dropping:  ");
    vec_debug_verbose(&$$);
    vec_drop(&$$);
} ident_lista;

%start programa;

%%
programa: { 
    printf("Init symbol table\n");
    hashset_init(&tabla, sizeof(Symbol), hash_symbol); 
} KW_PROG  IDENT '(' ident_lista ')' ';' decl subprograma_decl instruccion_compuesta '.' {
    printf("Programa: %.*s\n", $3.len, $3.ptr);
    printf("Entradas: %zu\n", $5.len);
    for (size_t i=0; i < $5.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$5, i);
        printf("    - %.*s\n", (int)str->len, str->ptr);
    }
};

ident_lista: {
    $$ = vec_new(sizeof(StrSlice));
    // vec_debug_verbose(&$$);
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
    printf("Variables: %zu\n", $3.len);
    for (size_t i=0; i < $3.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$3, i);
        Symbol s = (Symbol) { .name = *str, .line = 0 };
        if (hashset_contains(&tabla, &s)) {
            char lit[] = "Simbolo duplicado: ";
            String error_str;
            str_init_from_cstr(&error_str, &lit[0], strlen(&lit[0]));
            str_push_n(&error_str, str->ptr, str->len);
            yyerror(str_as_ref(&error_str));
            str_drop(&error_str);
            YYABORT;
        } 
        hashset_insert(&tabla, &s);
        printf("    - %.*s\n", (int)str->len, str->ptr);
    }
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
} argumentos ':' estandard_tipo ';' 
                                                     | KW_PROCEDURE IDENT {
    
    printf("Declarando procedure %.*s\n", (int)$2.len, $2.ptr);
} argumentos ';' {
    printf("Declarada %.*s\n", (int)$2.len, $2.ptr);
};

/* Argumentos */
argumentos: '(' parametros_lista ')' {
    printf("Argumentos: %zu\n", $2.len);
    for (size_t i=0; i < $2.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$2, i);
        printf("    - %.*s\n", (int)str->len, str->ptr);
    }
} | ;
parametros_lista: ident_lista ':' tipo {
    $$ = $1;
};
parametros_lista: parametros_lista ';' ident_lista ':' tipo {
    
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
lectura_instruccion: KW_READ '(' IDENT ')' | KW_READLN '(' IDENT ')';
escritura_instruccion: KW_WRITE '(' CONST_CADENA ',' IDENT ')' | KW_WRITELN '(' CONST_CADENA ',' IDENT ')'
	| KW_WRITE '(' CONST_CADENA  ')' | KW_WRITELN '(' CONST_CADENA  ')'
	| KW_WRITE '(' CONST_CADENA ',' expresion ')' | KW_WRITELN '(' CONST_CADENA ',' expresion ')';
if_instruccion: KW_IF relop_expresion KW_THEN instrucciones
    | KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones;

 /* Asignacion */
variable_asignacion: variable OP_ASIGN expresion;
for_asignacion: variable_asignacion | variable;
variable: IDENT | IDENT '[' expresion ']';
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
factor: IDENT | IDENT '[' expresion ']' | llamado_funcion | CONST_ENTERA | CONST_REAL | signo factor | '(' expresion ')';
signo: ADDOP | ;
%%
