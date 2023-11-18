%code top {
    #include "hashset.h"
    #include "str.h"
    #include "parser.h"
	#include "symbol.h"

    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);
    extern size_t linea;
    extern size_t nchar;
	size_t tnchar;
    extern size_t yyleng;
    size_t scope = 0;
	size_t fun_id = 0;
	
    HashSet tabla;
    String wrn_buff;

    void add_reference_to_sym(Symbol * s, size_t line) {
        size_t * ref = vec_push(&s->refs);
        *ref = line;
    }

    void * assert_sym_exists(Symbol * s) {
		tnchar = nchar;
        size_t orig_scope = s->scope;
        int found = 0;
		nchar = s->nchar;
        Symbol * res = NULL;
        
        for (size_t i = orig_scope; i >= 0; i-=orig_scope) {
            s->scope = i;
            //printf("Looking %.*s in scope %zu\n", (int)s->name.len, s->name.ptr , i);
            if (hashset_contains(&tabla, s)) {
                found = 1;
                res = (Symbol *)hashset_get(&tabla, s);
                add_reference_to_sym(res, s->line);
                break;
            }
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
            // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope, (int)s->name.len, s->name.ptr);
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
            // printf("No existe %zu(%zu):  %.*s\n", s->line, s->scope, (int)s->name.len, s->name.ptr);
        }
		nchar = tnchar;
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
	#include "symbol.h"
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

    #endif
}

%union {
    int64_t snum;
    uint64_t unum;
    double fnum;
    char * ident;
    StrSlice slice;
    DataType type;
    Symbol symbol;
    Vec idents;
    RelOp relop;
    AddOp addop;
    MulOp mulop;
}

/* Ident */
%token <symbol> IDENT;

/* Par */
%token <fnum> CONST_REAL <snum> CONST_ENTERA <slice> CONST_CADENA <relop> RELOP <addop> ADDOP <mulop> MULOP;

/* Keywords */ 
%token OP_ASIGN KW_PROCEDURE KW_PROG KW_CONST KW_VAR KW_ARRAY KW_OF KW_FUNC KW_BEGIN KW_END KW_READ KW_READLN KW_WRITE KW_WRITELN KW_WHILE KW_FOR KW_DO KW_TO KW_DOWNTO KW_IF KW_THEN KW_ELSE;

/* OPs */
%token RELOP_EQ RELOP_NEQ RELOP_BT RELOP_LT RELOP_EBT RELOP_ELT RELOP_AND RELOP_NOT RELOP_OR;

/* Types */
%token T_INT T_REAL T_STR T_BOOL;

%type <idents> ident_lista;
%type <idents> parametros_lista;
%type <type> estandard_tipo;

%destructor { 
    // printf("Dropping ident_lista:  ");
    // vec_debug_verbose(&$$);
	
	for (size_t i=0; i < $$.len; i++) {
        Symbol * s = vec_get(&$$, i);
        vec_drop(&s->refs);
    }
	
    vec_drop(&$$);
} ident_lista;

%destructor { 
    // printf("Dropping parametros_lista:  ");
    // vec_debug_verbose(&$$);
	
	for (size_t i=0; i < $$.len; i++) {
        Symbol * s = vec_get(&$$, i);
        vec_drop(&s->refs);
    }
	
    vec_drop(&$$);
} parametros_lista;

%start programa;

%%
programa: { 
    hashset_init(&tabla, sizeof(Symbol), hash_symbol); 
    str_init(&wrn_buff);
} KW_PROG  IDENT '(' ident_lista ')' ';' decl subprograma_decl instruccion_compuesta '.' {
    $3.type = Function;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);

    for (size_t i=0; i < $5.len; i++) {
        Symbol * s = vec_get(&$5, i);
        assert_not_sym_exists(s);
        s->type = Variable;
        hashset_insert(&tabla, s);
    }
	
	vec_drop(&$5);
};

ident_lista: IDENT ',' ident_lista {
    $$ = $3;
    Symbol *s = vec_push(&$$);
    *s = $1;
};
ident_lista: IDENT {
    $$ = vec_new(sizeof(Symbol));
    Symbol *s = vec_push(&$$);
    *s = $1;
};

/* Declaration of var and const */
decl: decl_var | decl_const | ;

decl_var: decl KW_VAR ident_lista ':' tipo ';' {
    for (size_t i=0; i < $3.len; i++) {
        Symbol * s = vec_get(&$3, i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
    }

	vec_drop(&$3);
};

decl_const: decl KW_CONST IDENT '=' CONST_ENTERA ';' {
    $3.type = Constant;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
};
decl_const: decl KW_CONST IDENT '=' CONST_REAL ';' {
    $3.type = Constant;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
};
decl_const: decl KW_CONST IDENT '=' CONST_CADENA ';' {
    $3.type = Constant;
    assert_not_sym_exists(&$3);
    hashset_insert(&tabla, &$3);
};

 /* Tipo */
tipo: estandard_tipo | KW_ARRAY '[' CONST_ENTERA '.' '.' CONST_ENTERA ']' KW_OF estandard_tipo;
estandard_tipo: T_INT { $$ = Int; } | T_REAL { $$ = Real; } | T_STR { $$ = Str; } | T_BOOL { $$ = Bool; };

 /* Subprograma */
subprograma_decl: subprograma_decl subprograma_declaracion ';' | ;
subprograma_declaracion: subprograma_encabezado decl subprograma_decl instruccion_compuesta {
    scope-= fun_id;
};
subprograma_encabezado: KW_FUNC IDENT {
    $2.type = Function;
	assert_not_sym_exists(&$2);
	hashset_insert(&tabla, &$2);
    fun_id++;
	scope+=fun_id;
} argumentos ':' estandard_tipo ';' {
	Symbol * s = (Symbol *)hashset_get(&tabla, &$2);
    s->info.fun = (FunctionInfo) { .return_type = $6 };
} ;
subprograma_encabezado: KW_PROCEDURE IDENT {
    $2.type = Procedure;
	assert_not_sym_exists(&$2);
	hashset_insert(&tabla, &$2);
    fun_id++;
	scope += fun_id;
} argumentos ';' ;

/* Argumentos */
argumentos: '(' parametros_lista ')' {
    // printf("Argumentos: %zu\n", $2.len);
    for (size_t i=0; i < $2.len; i++) {
        Symbol * s = (Symbol *)vec_get(&$2, i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
		// printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
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
    assert_sym_exists(&$3);
}; 
lectura_instruccion: KW_READLN '(' IDENT ')' { 
    assert_sym_exists(&$3);
};
escritura_instruccion: KW_WRITE '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
};
escritura_instruccion: KW_WRITELN '(' CONST_CADENA ',' IDENT ')' {
    assert_sym_exists(&$5);
};
escritura_instruccion: KW_WRITE '(' CONST_CADENA  ')' | KW_WRITELN '(' CONST_CADENA  ')'
	| KW_WRITE '(' CONST_CADENA ',' expresion ')' | KW_WRITELN '(' CONST_CADENA ',' expresion ')';
escritura_instruccion: KW_WRITE '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);
}
escritura_instruccion: KW_WRITELN '(' IDENT ',' IDENT ')' {
    assert_sym_exists(&$3);
    assert_sym_exists(&$5);
};
escritura_instruccion: KW_WRITE '(' IDENT  ')' {
    assert_sym_exists(&$3);
};
escritura_instruccion: KW_WRITELN '(' IDENT  ')' {
    assert_sym_exists(&$3);
};
escritura_instruccion: KW_WRITE '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
};
escritura_instruccion: KW_WRITELN '(' IDENT ',' expresion ')' {
    assert_sym_exists(&$3);
};
if_instruccion: KW_IF relop_expresion KW_THEN instrucciones
    | KW_IF relop_expresion KW_THEN instrucciones KW_ELSE instrucciones;

 /* Asignacion */
variable_asignacion: variable OP_ASIGN expresion; 
for_asignacion: variable_asignacion | variable;
variable: IDENT {
    assert_sym_exists(&$1);
};
variable: IDENT '[' CONST_ENTERA ']' {
    assert_sym_exists(&$1);
};
procedure_instruccion : IDENT {
    assert_sym_exists(&$1);
};
procedure_instruccion : IDENT '(' expresion_lista ')' {
    assert_sym_exists(&$1);
};

 /* Relop */
relop_expresion: relop_expresion RELOP_OR relop_and | relop_and;
relop_and: relop_and RELOP_AND relop_not | relop_not ;
relop_not: RELOP_NOT relop_not | relop_paren;
relop_paren: '(' relop_expresion ')' | relop_expresion_simple;
relop_expresion_simple: expresion relop expresion;
relop: RELOP_AND | RELOP_OR | RELOP_BT | RELOP_LT | RELOP_EBT | RELOP_ELT | RELOP_EQ | RELOP_NEQ;

 /* Expresion */
expresion_lista: expresion | expresion_lista ',' expresion;
expresion: termino | expresion ADDOP termino;
termino: factor | termino MULOP factor;
llamado_funcion : IDENT '(' expresion_lista ')' {
    assert_sym_exists(&$1);
};
factor: IDENT {
    assert_sym_exists(&$1);
}; 
factor: IDENT '[' expresion ']' {
	assert_sym_exists(&$1);
};
factor: llamado_funcion | CONST_ENTERA | CONST_REAL | ADDOP factor | '(' expresion ')';
%%
