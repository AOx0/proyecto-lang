%code top {
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>

    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);
}

%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>

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
    char * ident;
    StrSlice slice;
}

/* Ident */
%token <slice> IDENT;

/* Par */
%token <unum> UINT <slice> STRING;

/* Keywords */ 
%token KW_PROG KW_CONST KW_VAR KW_ARRAY KW_OF;

/* Types */
%token T_INT T_REAL T_STR T_BOOL;

%start programa;

%%
programa: KW_PROG IDENT '(' ident_lista ')' ';' decl ident_lista {
    printf("Ident: %.*s\n", $2.len, $2.start);
};
ident_lista: IDENT ',' ident_lista | IDENT {
    printf("Cadena: %.*s\n", $1.len, $1.start);
};

/* Declaration */
decl: decl_var decl | decl_const decl |  ;
decl_var: KW_VAR ident_lista ':' tipo ';' {
    
};
decl_const: KW_CONST IDENT '=' IDENT ';' {
    printf("Const: %.*s\n", $2.len, $2.start);
};

 /* Tipo */
tipo: estandard_tipo | KW_ARRAY '[' T_INT '.' '.' T_INT ']' KW_OF estandard_tipo;
estandard_tipo: T_INT | T_REAL | T_STR | T_BOOL;
%%
