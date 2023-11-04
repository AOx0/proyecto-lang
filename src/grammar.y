%code top {
    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);
}

%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>
    #include "vector.h"

    extern FILE *yyin, *yyout;

    struct StrSlice {
        char * start;
        int len;
    };
    typedef struct StrSlice StrSlice;
}

%union {
    int64_t snum;
    uint64_t unum;
    double fnum;
    char * ident;
    StrSlice slice;
    Vec idents;
}

/* Ident */
%token <slice> IDENT;

/* Par */
%token <fnum> CONST_REAL <snum> CONST_ENTERA <slice> CONST_CADENA;

/* Keywords */ 
%token KW_PROG KW_CONST KW_VAR KW_ARRAY KW_OF;

/* Types */
%token T_INT T_REAL T_STR T_BOOL;

%type <idents> ident_lista;

%destructor { 
    printf("Dropping:  ");
    vec_debug_verbose(&$$);
    vec_drop(&$$);
} <idents>;

%start programa;

%%
programa: KW_PROG IDENT '(' ident_lista ')' ';' decl  {
    printf("Programa: %.*s\n", $2.len, $2.start);

    printf("Entradas: %zu\n", $4.len);
    for (size_t i=0; i < $4.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$4, i);
        printf("    - %.*s\n", str->len, str->start);
    }
};

ident_lista: IDENT ',' ident_lista {
    StrSlice *sl = vec_push(&$3);
    *sl = $1;
    $$ = $3;
};
ident_lista: IDENT {
    $$ = vec_new(sizeof(StrSlice));
    StrSlice *sl = vec_push(&$$);
    *sl = $1;
};

/* Declaration of var and const */
decl: decl_var | decl_const | ;

decl_var: decl KW_VAR ident_lista ':' tipo ';' {
    printf("Variables: %zu\n", $3.len);
    for (size_t i=0; i < $3.len; i++) {
        StrSlice *str = (StrSlice *)vec_get(&$3, i);
        printf("    - %.*s\n", str->len, str->start);
    }
};

decl_const: decl KW_CONST IDENT '=' CONST_ENTERA ';' {
    printf("Constante: %ld\n", $5);
};
decl_const: decl KW_CONST IDENT '=' CONST_REAL ';' {
    printf("Constante: %f\n", $5);
};
decl_const: decl KW_CONST IDENT '=' CONST_CADENA ';' {
    printf("Constante: %.*s\n", $5.len, $5.start);
};

 /* Tipo */
tipo: estandard_tipo | KW_ARRAY '[' CONST_ENTERA '.' '.' CONST_ENTERA ']' KW_OF estandard_tipo;
estandard_tipo: T_INT | T_REAL | T_STR | T_BOOL;
%%
