/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
/* Line 349 of yacc.c  */
#line 1 "src/grammar.y"

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

    int assert_variable_assigned_type(Node *var, Node *val) {
        if (var->asoc_type != val->asoc_type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento asignar una expresion de tipo distinto a una variable: ");
            str_push_n(&wrn_buff, var->value.expr.value.symbol.name.ptr,  var->value.expr.value.symbol.name.len);
            str_push(&wrn_buff, ", la variable es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(var->asoc_type));
            str_push(&wrn_buff, " y la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(val->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_number_of_children(Tree *t, size_t id, size_t len) {
        if (tree_num_child(t, id) != len) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se esperaba una cantidad de hijos distinta a la recibida: ");
            str_push_sizet(&wrn_buff, len);
            str_push(&wrn_buff, " hijos pero se recibieron ");
            str_push_sizet(&wrn_buff, tree_num_child(t, id));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_expr_type(Node *a, Node *b) {
        if (a->asoc_type != b->asoc_type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento realizar operaciones entre expresiones de tipos distintos: ");
            str_push(&wrn_buff, "El primer operando es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(a->asoc_type));
            str_push(&wrn_buff, " y el segundo es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(b->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_node_is_printable(Node *n) {
        if (n->asoc_type == Void || n->asoc_type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir una expresion de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_ident_is_printable(Symbol *s) {
        if (s->asoc_type.type == Void || s->asoc_type.type == Ukw) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento imprimir un identificador de tipo no imprimible, la expresion es de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(s->asoc_type.type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_tree_asoc_type_is(Tree t, DataTypeE type) {
        Node *n = ast_get_root(&t);

        if (n->asoc_type != type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se esperaba una expresion de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(type));
            str_push(&wrn_buff, " pero se recibio una expresion de tipo ");
            str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_tree_node_type_is(Tree t, NodeType type) {
        Node *n = ast_get_root(&t);

        if (n->node_type != type) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se esperaba ");
            str_push(&wrn_buff, node_type_display(type));
            str_push(&wrn_buff, " pero se recibió ");
            str_push(&wrn_buff, node_type_display(n->node_type));
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int assert_sym_is_callable(Symbol *n) {
        if (n->type != Function && n->type != Procedure) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a un identificador que no es una funcion o procedimiento, el identificador es ");
            str_push_n(&wrn_buff, n->name.ptr, n->name.len);
            yyerror(str_as_ref(&wrn_buff));

            return 0;
        }

        return 1;
    }

    int assert_arguments_length(Symbol *s, size_t len) {
        if (s->info.fun.args.len != len) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento referenciar la función ");
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            str_push(&wrn_buff, " con una cantidad de argumentos distinta a la declarada, se esperaban ");
            str_push_sizet(&wrn_buff, s->info.fun.args.len);
            str_push(&wrn_buff, " argumentos pero se recibieron ");
            str_push_sizet(&wrn_buff, len);
            yyerror(str_as_ref(&wrn_buff));
            return 0;
        }

        return 1;
    }

    int bound_check(Symbol *s, int64_t id) {
        size_t idx = (size_t)id;
        size_t arr_size = s->asoc_type.size;

        if (arr_size < idx || idx < 0) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Indice fuera de rango: ");
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
            str_push_sizet(&wrn_buff, arr_size);
            str_push(&wrn_buff, " y se intento acceder a la posicion ");
            str_push_sizet(&wrn_buff, idx);
            yyerror(str_as_ref(&wrn_buff));
        }

        return 1;
    }


/* Line 349 of yacc.c  */
#line 310 "grammar.tab.c"



/* Copy the first part of user declarations.  */

/* Line 371 of yacc.c  */
#line 317 "grammar.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 387 of yacc.c  */
#line 245 "src/grammar.y"


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


/* Line 387 of yacc.c  */
#line 380 "grammar.tab.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     CONST_REAL = 259,
     CONST_ENTERA = 260,
     CONST_CADENA = 261,
     RELOP = 262,
     ADDOP = 263,
     MULOP = 264,
     OP_ASIGN = 265,
     KW_PROCEDURE = 266,
     KW_PROG = 267,
     KW_CONST = 268,
     KW_VAR = 269,
     KW_ARRAY = 270,
     KW_OF = 271,
     KW_FUNC = 272,
     KW_BEGIN = 273,
     KW_END = 274,
     KW_READ = 275,
     KW_READLN = 276,
     KW_WRITE = 277,
     KW_WRITELN = 278,
     KW_WHILE = 279,
     KW_FOR = 280,
     KW_DO = 281,
     KW_TO = 282,
     KW_DOWNTO = 283,
     KW_IF = 284,
     KW_THEN = 285,
     KW_ELSE = 286,
     KW_DOTS = 287,
     RELOP_EQ = 288,
     RELOP_NEQ = 289,
     RELOP_BT = 290,
     RELOP_LT = 291,
     RELOP_EBT = 292,
     RELOP_ELT = 293,
     RELOP_AND = 294,
     RELOP_NOT = 295,
     RELOP_OR = 296,
     T_INT = 297,
     T_REAL = 298,
     T_STR = 299,
     T_BOOL = 300
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 275 "src/grammar.y"

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
    Vec instructions;


/* Line 387 of yacc.c  */
#line 457 "grammar.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 485 "grammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   199

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNRULES -- Number of states.  */
#define YYNSTATES  191

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,     2,     2,    50,     2,    49,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    48,
       2,    52,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    53,     2,    54,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    16,    20,    22,    23,    25,    27,
      28,    35,    42,    49,    56,    58,    67,    69,    71,    73,
      75,    79,    80,    85,    86,    94,    95,   101,   105,   106,
     110,   116,   120,   122,   123,   125,   129,   131,   133,   135,
     137,   139,   141,   143,   148,   155,   162,   167,   172,   177,
     182,   187,   194,   198,   200,   202,   204,   209,   211,   216,
     220,   222,   226,   228,   231,   233,   237,   239,   243,   245,
     247,   249,   251,   253,   255,   257,   259,   261,   265,   266,
     268,   272,   274,   278,   280,   284,   286,   290,   295,   297,
     302,   304,   306,   308,   311
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      56,     0,    -1,    -1,    57,    12,     3,    46,    58,    47,
      48,    59,    64,    71,    49,    -1,     3,    50,    58,    -1,
       3,    -1,    -1,    60,    -1,    61,    -1,    -1,    59,    14,
      58,    51,    62,    48,    -1,    59,    13,     3,    52,     5,
      48,    -1,    59,    13,     3,    52,     4,    48,    -1,    59,
      13,     3,    52,     6,    48,    -1,    63,    -1,    15,    53,
       5,    32,     5,    54,    16,    63,    -1,    42,    -1,    43,
      -1,    44,    -1,    45,    -1,    64,    65,    48,    -1,    -1,
      66,    59,    64,    71,    -1,    -1,    17,     3,    67,    69,
      51,    63,    48,    -1,    -1,    11,     3,    68,    69,    48,
      -1,    46,    70,    47,    -1,    -1,    58,    51,    62,    -1,
      70,    48,    58,    51,    62,    -1,    18,    72,    19,    -1,
      73,    -1,    -1,    74,    -1,    73,    48,    74,    -1,    79,
      -1,    82,    -1,    71,    -1,    78,    -1,    75,    -1,    76,
      -1,    77,    -1,    24,    83,    26,    74,    -1,    25,    80,
      27,    91,    26,    74,    -1,    25,    80,    28,    91,    26,
      74,    -1,    20,    46,     3,    47,    -1,    21,    46,     3,
      47,    -1,    23,    46,    90,    47,    -1,    22,    46,    90,
      47,    -1,    29,    83,    30,    74,    -1,    29,    83,    30,
      74,    31,    74,    -1,    81,    10,    91,    -1,    79,    -1,
      81,    -1,     3,    -1,     3,    53,     5,    54,    -1,     3,
      -1,     3,    46,    89,    47,    -1,    83,    41,    84,    -1,
      84,    -1,    84,    39,    85,    -1,    85,    -1,    40,    85,
      -1,    86,    -1,    46,    83,    47,    -1,    87,    -1,    91,
      88,    91,    -1,    39,    -1,    41,    -1,    35,    -1,    36,
      -1,    37,    -1,    38,    -1,    33,    -1,    34,    -1,    91,
      -1,    89,    50,    91,    -1,    -1,    91,    -1,    90,    50,
      91,    -1,     6,    -1,    90,    50,     6,    -1,    92,    -1,
      91,     8,    92,    -1,    94,    -1,    92,     9,    94,    -1,
       3,    46,    89,    47,    -1,     3,    -1,     3,    53,     5,
      54,    -1,    93,    -1,     5,    -1,     4,    -1,     8,    94,
      -1,    46,    91,    47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   373,   373,   373,   489,   494,   499,   504,   504,   504,
     506,   521,   538,   555,   575,   576,   580,   581,   582,   583,
     586,   589,   595,   631,   631,   644,   644,   659,   672,   675,
     686,   703,   706,   719,   724,   729,   736,   737,   737,   737,
     738,   738,   738,   741,   750,   781,   817,   827,   839,   849,
     860,   870,   884,   914,   925,   927,   938,   954,   976,  1026,
    1043,  1045,  1062,  1064,  1078,  1080,  1080,  1082,  1098,  1099,
    1100,  1101,  1102,  1103,  1104,  1105,  1108,  1114,  1117,  1123,
    1129,  1132,  1142,  1151,  1151,  1169,  1169,  1187,  1249,  1260,
    1276,  1276,  1285,  1294,  1309
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "CONST_REAL", "CONST_ENTERA",
  "CONST_CADENA", "RELOP", "ADDOP", "MULOP", "OP_ASIGN", "KW_PROCEDURE",
  "KW_PROG", "KW_CONST", "KW_VAR", "KW_ARRAY", "KW_OF", "KW_FUNC",
  "KW_BEGIN", "KW_END", "KW_READ", "KW_READLN", "KW_WRITE", "KW_WRITELN",
  "KW_WHILE", "KW_FOR", "KW_DO", "KW_TO", "KW_DOWNTO", "KW_IF", "KW_THEN",
  "KW_ELSE", "KW_DOTS", "RELOP_EQ", "RELOP_NEQ", "RELOP_BT", "RELOP_LT",
  "RELOP_EBT", "RELOP_ELT", "RELOP_AND", "RELOP_NOT", "RELOP_OR", "T_INT",
  "T_REAL", "T_STR", "T_BOOL", "'('", "')'", "';'", "'.'", "','", "':'",
  "'='", "'['", "']'", "$accept", "programa", "$@1", "ident_lista", "decl",
  "decl_var", "decl_const", "tipo", "estandard_tipo", "subprogramas",
  "subprograma_declaracion", "subprograma_encabezado", "$@2", "$@3",
  "argumentos", "parametros_lista", "instruccion_compuesta",
  "instrucciones_opcionales", "instrucciones_lista", "instrucciones",
  "repeticion_instruccion", "lectura_instruccion", "escritura_instruccion",
  "if_instruccion", "variable_asignacion", "for_asignacion", "variable",
  "procedure_instruccion", "relop_expresion", "relop_and", "relop_not",
  "relop_paren", "relop_expresion_simple", "relop", "expresion_lista",
  "expresion_lista_con_cadena", "expresion", "termino", "llamado_funcion",
  "factor", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    40,    41,    59,    46,
      44,    58,    61,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    55,    57,    56,    58,    58,    58,    59,    59,    59,
      60,    61,    61,    61,    62,    62,    63,    63,    63,    63,
      64,    64,    65,    67,    66,    68,    66,    69,    69,    70,
      70,    71,    72,    72,    73,    73,    74,    74,    74,    74,
      74,    74,    74,    75,    75,    75,    76,    76,    77,    77,
      78,    78,    79,    80,    80,    81,    81,    82,    82,    83,
      83,    84,    84,    85,    85,    86,    86,    87,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    89,    90,
      90,    90,    90,    91,    91,    92,    92,    93,    94,    94,
      94,    94,    94,    94,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,    11,     3,     1,     0,     1,     1,     0,
       6,     6,     6,     6,     1,     8,     1,     1,     1,     1,
       3,     0,     4,     0,     7,     0,     5,     3,     0,     3,
       5,     3,     1,     0,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     4,     6,     6,     4,     4,     4,     4,
       4,     6,     3,     1,     1,     1,     4,     1,     4,     3,
       1,     3,     1,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     1,
       3,     1,     3,     1,     3,     1,     3,     4,     1,     4,
       1,     1,     1,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     6,     5,     0,     6,
       0,     4,     9,    21,     7,     8,     0,     6,     0,     0,
       0,     0,     0,    33,     0,     9,     0,     0,     0,    25,
      23,    57,     0,     0,     0,     0,     0,     0,     0,    38,
       0,    32,    34,    40,    41,    42,    39,    36,     0,    37,
      20,    21,     3,     0,     0,     0,     0,    16,    17,    18,
      19,     0,    14,    28,    28,    78,     0,     0,     0,     0,
       0,    88,    92,    91,     0,     0,     0,     0,    60,    62,
      64,    66,     0,    83,    90,    85,    55,    53,     0,    54,
       0,    31,     0,     0,     0,    12,    11,    13,     0,    10,
       6,     0,     0,     0,     0,    76,     0,     0,     0,    81,
       0,    79,     0,    78,     0,    93,    63,     0,     0,     0,
       0,     0,     0,    74,    75,    70,    71,    72,    73,    68,
      69,     0,     0,     0,     0,     0,    35,    52,    22,     0,
       0,     0,    26,     0,     0,    58,     0,    56,    46,    47,
      49,     0,    48,     0,     0,    65,    94,    43,    59,    61,
      84,    67,    86,     0,     0,    50,     0,     0,    27,     6,
       0,    77,    82,    80,    87,    89,     0,     0,     0,     0,
      29,     0,    24,    44,    45,    51,     0,     0,     0,    30,
      15
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     8,    13,    14,    15,    61,    62,    18,
      24,    25,    64,    63,   101,   141,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    88,    48,    49,    77,    78,
      79,    80,    81,   131,   104,   110,    82,    83,    84,    85
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -145
static const yytype_int16 yypact[] =
{
    -145,    41,    39,  -145,    43,    35,   104,    76,    89,   104,
      90,  -145,  -145,   119,  -145,  -145,   134,   104,    62,    88,
      91,   136,   138,   100,    95,  -145,    96,    79,    21,  -145,
    -145,     6,    98,   101,   102,   103,    10,   143,    10,  -145,
     131,   105,  -145,  -145,  -145,  -145,  -145,  -145,   141,  -145,
    -145,   119,  -145,   106,   107,   108,    99,  -145,  -145,  -145,
    -145,   109,  -145,   112,   112,    29,   154,   158,   159,    16,
      16,    -8,  -145,  -145,    29,    10,    10,    31,   124,  -145,
    -145,  -145,    78,   155,  -145,  -145,   113,  -145,    49,   141,
      14,  -145,   100,    29,    62,  -145,  -145,  -145,   160,  -145,
     104,   120,   116,    29,    58,   161,   117,   123,   125,  -145,
      59,   161,    80,    29,   168,  -145,  -145,   -30,    63,   100,
      10,    10,    29,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,    29,    29,    29,    29,   100,  -145,   161,  -145,   142,
     126,    87,  -145,    50,    27,  -145,    29,  -145,  -145,  -145,
    -145,    22,  -145,    81,   121,  -145,  -145,  -145,   124,  -145,
     155,   161,  -145,    32,    34,   145,   173,    21,  -145,   104,
     132,   161,  -145,   161,  -145,  -145,   100,   100,   100,   127,
    -145,   128,  -145,  -145,  -145,  -145,   166,    21,    50,  -145,
    -145
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,  -145,    -9,   162,  -145,  -145,  -144,  -134,   133,
    -145,  -145,  -145,  -145,   122,  -145,   -16,  -145,  -145,   -88,
    -145,  -145,  -145,  -145,   146,  -145,   148,  -145,   -28,    68,
     -68,  -145,  -145,  -145,    77,   129,   -64,    67,  -145,   -71
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -56
static const yytype_int16 yytable[] =
{
      11,   105,    26,   115,   136,   111,   111,   116,    20,   170,
      90,   120,   118,    71,    72,    73,   -55,   155,    74,    71,
      72,    73,   109,   180,    74,    71,    72,    73,   172,   137,
      74,   157,    71,    72,    73,   122,    56,    74,   113,   144,
     122,     3,   122,   189,   135,   114,     5,   165,   117,   105,
      75,     4,    65,   159,   190,   120,    76,   119,   176,    66,
     177,   162,   103,    57,    58,    59,    60,   161,   103,   163,
     164,   122,   120,    21,   156,   103,   133,   134,   138,    22,
      23,     6,   171,    53,    54,    55,   122,   173,   183,   184,
     185,   140,    57,    58,    59,    60,   123,   124,   125,   126,
     127,   128,   129,    31,   130,   145,   150,     7,   146,   151,
     156,   123,   124,   125,   126,   127,   128,   129,    23,   130,
      32,    33,    34,    35,    36,    37,     9,   152,   174,    38,
     151,   146,    16,    17,   168,   169,    10,    19,    12,    29,
      27,    30,    28,    50,    67,    52,    86,    68,    69,    70,
      91,    93,    98,    92,    95,    96,    97,    99,   100,   106,
     181,   107,   108,   121,   132,   139,    66,   143,   142,   122,
     148,   147,   149,   154,   166,   175,   178,   167,   179,   187,
     182,   186,   188,    87,    94,    89,   102,    51,   158,   160,
     153,     0,     0,     0,     0,     0,     0,     0,     0,   112
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-145)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       9,    65,    18,    74,    92,    69,    70,    75,    17,   143,
      38,    41,    76,     3,     4,     5,    10,    47,     8,     3,
       4,     5,     6,   167,     8,     3,     4,     5,     6,    93,
       8,   119,     3,     4,     5,     8,    15,     8,    46,   103,
       8,     0,     8,   187,    30,    53,     3,   135,    76,   113,
      40,    12,    46,   121,   188,    41,    46,    26,    26,    53,
      26,   132,    46,    42,    43,    44,    45,   131,    46,   133,
     134,     8,    41,    11,    47,    46,    27,    28,    94,    17,
      18,    46,   146,     4,     5,     6,     8,   151,   176,   177,
     178,   100,    42,    43,    44,    45,    33,    34,    35,    36,
      37,    38,    39,     3,    41,    47,    47,     3,    50,    50,
      47,    33,    34,    35,    36,    37,    38,    39,    18,    41,
      20,    21,    22,    23,    24,    25,    50,    47,    47,    29,
      50,    50,    13,    14,    47,    48,    47,     3,    48,     3,
      52,     3,    51,    48,    46,    49,     3,    46,    46,    46,
      19,    10,    53,    48,    48,    48,    48,    48,    46,     5,
     169,     3,     3,    39,     9,     5,    53,    51,    48,     8,
      47,    54,    47,     5,    32,    54,    31,    51,     5,    51,
      48,    54,    16,    37,    51,    37,    64,    25,   120,   122,
     113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    56,    57,     0,    12,     3,    46,     3,    58,    50,
      47,    58,    48,    59,    60,    61,    13,    14,    64,     3,
      58,    11,    17,    18,    65,    66,    71,    52,    51,     3,
       3,     3,    20,    21,    22,    23,    24,    25,    29,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    81,    82,
      48,    59,    49,     4,     5,     6,    15,    42,    43,    44,
      45,    62,    63,    68,    67,    46,    53,    46,    46,    46,
      46,     3,     4,     5,     8,    40,    46,    83,    84,    85,
      86,    87,    91,    92,    93,    94,     3,    79,    80,    81,
      83,    19,    48,    10,    64,    48,    48,    48,    53,    48,
      46,    69,    69,    46,    89,    91,     5,     3,     3,     6,
      90,    91,    90,    46,    53,    94,    85,    83,    91,    26,
      41,    39,     8,    33,    34,    35,    36,    37,    38,    39,
      41,    88,     9,    27,    28,    30,    74,    91,    71,     5,
      58,    70,    48,    51,    91,    47,    50,    54,    47,    47,
      47,    50,    47,    89,     5,    47,    47,    74,    84,    85,
      92,    91,    94,    91,    91,    74,    32,    51,    47,    48,
      63,    91,     6,    91,    47,    54,    26,    26,    31,     5,
      62,    58,    48,    74,    74,    74,    54,    51,    16,    62,
      63
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 58: /* ident_lista */
/* Line 1393 of yacc.c  */
#line 346 "src/grammar.y"
        {
    // printf("Dropping ident_lista:  ");
    // vec_debug_verbose(&$$);

    for (size_t i = 0; i < ((*yyvaluep).idents).len; i++) {
        Symbol *s = vec_get(&((*yyvaluep).idents), i);
        vec_drop(&s->refs);
    }

    vec_drop(&((*yyvaluep).idents));
};
/* Line 1393 of yacc.c  */
#line 1587 "grammar.tab.c"
        break;
      case 70: /* parametros_lista */
/* Line 1393 of yacc.c  */
#line 358 "src/grammar.y"
        {
    // printf("Dropping parametros_lista:  ");
    // vec_debug_verbose(&$$);

    for (size_t i = 0; i < ((*yyvaluep).idents).len; i++) {
        Symbol *s = vec_get(&((*yyvaluep).idents), i);
        vec_drop(&s->refs);
    }

    vec_drop(&((*yyvaluep).idents));
};
/* Line 1393 of yacc.c  */
#line 1604 "grammar.tab.c"
        break;

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1802 of yacc.c  */
#line 373 "src/grammar.y"
    {
    hashset_init(&tabla, sizeof(Symbol), hash_symbol);
    str_init(&wrn_buff);
    tree_init(&ast, sizeof(Node));
}
    break;

  case 3:
/* Line 1802 of yacc.c  */
#line 379 "src/grammar.y"
    {

    for (size_t i = 0; i < (yyvsp[(5) - (11)].idents).len; i++) {
        Symbol *s = vec_get(&(yyvsp[(5) - (11)].idents), i);
        assert_not_sym_exists(s);
        s->type = Variable;
        s->asoc_type = (DataType) { .type = Str, .size = 1 };
        s->info.var = (VariableInfo){ .addr = addr};
        hashset_insert(&tabla, s);
    }

    (yyvsp[(3) - (11)].symbol).type = Function;
    (yyvsp[(3) - (11)].symbol).asoc_type = (DataType) { .type = Void, .size = 0 };
    (yyvsp[(3) - (11)].symbol).info.fun = (FunctionInfo) {
        .args = (yyvsp[(5) - (11)].idents),
    };
    assert_not_sym_exists(&(yyvsp[(3) - (11)].symbol));
    hashset_insert(&tabla, &(yyvsp[(3) - (11)].symbol));

    ast_create_node(&ast, NRoot, Void);

    for (size_t i = 0; i < (yyvsp[(8) - (11)].idents).len; i++) {
        Symbol *s = vec_get(&(yyvsp[(8) - (11)].idents), i);
        Node * node = ast_create_node(&ast, NVar, Void);

        if (s->type == Variable) { 
            node->value.var = (VarNode) { .symbol = *s, .statement = 1 };
        } else {
            node->node_type = NConst;
            node->value.cons = (ConstNode) { .symbol = *s, .value.bool = 1 };
        }

        tree_new_relation(&ast, 0, node->id); 
    }

    tree_extend_with_subtree(&ast, &(yyvsp[(9) - (11)].subtree), 0, 0);

    Tree program;
    tree_init(&program, sizeof(Node));

    Node * n = ast_create_node(&program, NFunction, Void);
    n->value.fun = (FunctionNode) {
        .symbol = (yyvsp[(3) - (11)].symbol),
        .args = (yyvsp[(5) - (11)].idents),
    };
    
    tree_extend_with_subtree(&program, &(yyvsp[(10) - (11)].subtree), 0, 0);
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
        tree_debug_id(&ast, &tabla, 0, 0);
    #endif

    if (err == 0) node_display_id(0, OUT_FILE, &ast, &tabla, 0);

    // Al final liberamos la tabla de hashes de memoria
    vec_drop(&(yyvsp[(5) - (11)].idents));
    hashset_drop(&tabla);
    str_drop(&wrn_buff);
    tree_drop(&ast);
}
    break;

  case 4:
/* Line 1802 of yacc.c  */
#line 489 "src/grammar.y"
    {
    (yyval.idents) = (yyvsp[(3) - (3)].idents);
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[(1) - (3)].symbol);
}
    break;

  case 5:
/* Line 1802 of yacc.c  */
#line 494 "src/grammar.y"
    {
    (yyval.idents) = vec_new(sizeof(Symbol));
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[(1) - (1)].symbol);
}
    break;

  case 6:
/* Line 1802 of yacc.c  */
#line 499 "src/grammar.y"
    {
    (yyval.idents) = vec_new(sizeof(Symbol));
}
    break;

  case 9:
/* Line 1802 of yacc.c  */
#line 504 "src/grammar.y"
    { (yyval.idents) = vec_new(sizeof(Symbol)); }
    break;

  case 10:
/* Line 1802 of yacc.c  */
#line 506 "src/grammar.y"
    {
    for (size_t i = 0; i < (yyvsp[(3) - (6)].idents).len; i++) {
        Symbol *s = vec_get(&(yyvsp[(3) - (6)].idents), i);
        s->type = Variable;
        s->asoc_type = (yyvsp[(5) - (6)].type);
        s->info.var = (VariableInfo){.addr = addr};
        addr += data_type_size(&(yyvsp[(5) - (6)].type));
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
    }

    (yyval.idents) = (yyvsp[(1) - (6)].idents);
    vec_extend(&(yyval.idents), &(yyvsp[(3) - (6)].idents));
}
    break;

  case 11:
/* Line 1802 of yacc.c  */
#line 521 "src/grammar.y"
    {
    (yyvsp[(3) - (6)].symbol).type = Constant;
    (yyvsp[(3) - (6)].symbol).asoc_type = (DataType){
        .type = Int, 
        .size = 1
    };
    (yyvsp[(3) - (6)].symbol).info.cons = (ConstantInfo){
        .addr = addr,
        .value.snum = (yyvsp[(5) - (6)].snum)
    };
    addr += 4;
    assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
    hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));
    (yyval.idents) = (yyvsp[(1) - (6)].idents);
    Symbol * s = (Symbol *)vec_push(&(yyval.idents));
    *s = (yyvsp[(3) - (6)].symbol);
}
    break;

  case 12:
/* Line 1802 of yacc.c  */
#line 538 "src/grammar.y"
    {
    (yyvsp[(3) - (6)].symbol).type = Constant;
    (yyvsp[(3) - (6)].symbol).asoc_type = (DataType){
        .type = Real, 
        .size = 1
    };
    (yyvsp[(3) - (6)].symbol).info.cons = (ConstantInfo){
        .addr = addr,
        .value.real = (yyvsp[(5) - (6)].fnum)
    };
    addr += 4;
    assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
    hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));
    (yyval.idents) = (yyvsp[(1) - (6)].idents);
    Symbol * s = (Symbol *)vec_push(&(yyval.idents));
    *s = (yyvsp[(3) - (6)].symbol);
}
    break;

  case 13:
/* Line 1802 of yacc.c  */
#line 555 "src/grammar.y"
    {
    (yyvsp[(3) - (6)].symbol).type = Constant;
    (yyvsp[(3) - (6)].symbol).asoc_type = (DataType){
        .type = Str, 
        .size = (yyvsp[(5) - (6)].slice).len
    };
    (yyvsp[(3) - (6)].symbol).info.cons = (ConstantInfo){
        .addr = addr,
        .value.str = (yyvsp[(5) - (6)].slice)
    };
    addr += 1 * (yyvsp[(5) - (6)].slice).len;
    assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
    hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));
   
    (yyval.idents) = (yyvsp[(1) - (6)].idents);
    Symbol * s = (Symbol *)vec_push(&(yyval.idents));
    *s = (yyvsp[(3) - (6)].symbol);
}
    break;

  case 14:
/* Line 1802 of yacc.c  */
#line 575 "src/grammar.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); }
    break;

  case 15:
/* Line 1802 of yacc.c  */
#line 576 "src/grammar.y"
    {
    (yyval.type) = (yyvsp[(8) - (8)].type);
    (yyval.type).size = (yyval.type).size * ((yyvsp[(5) - (8)].snum) - (yyvsp[(3) - (8)].snum));
}
    break;

  case 16:
/* Line 1802 of yacc.c  */
#line 580 "src/grammar.y"
    { (yyval.type) = (DataType){.type = Int, .size = 1}; }
    break;

  case 17:
/* Line 1802 of yacc.c  */
#line 581 "src/grammar.y"
    { (yyval.type) = (DataType){.type = Real, .size = 1}; }
    break;

  case 18:
/* Line 1802 of yacc.c  */
#line 582 "src/grammar.y"
    { (yyval.type) = (DataType){.type = Str, .size = 1}; }
    break;

  case 19:
/* Line 1802 of yacc.c  */
#line 583 "src/grammar.y"
    { (yyval.type) = (DataType){.type = Bool, .size = 1}; }
    break;

  case 20:
/* Line 1802 of yacc.c  */
#line 586 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(1) - (3)].subtree);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (3)].subtree), 0, 0);
}
    break;

  case 21:
/* Line 1802 of yacc.c  */
#line 589 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);
}
    break;

  case 22:
/* Line 1802 of yacc.c  */
#line 595 "src/grammar.y"
    {
    scope -= fun_id;

    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NFunction, Void);
    n->value.fun = (FunctionNode) {
        .symbol = (yyvsp[(1) - (4)].symbol),
        .args = (yyvsp[(2) - (4)].idents),
    };

    Tree decls;
    tree_init(&decls, sizeof(Node));

    ast_create_node(&decls, NVoid, Void);

    for (size_t i = 0; i < (yyvsp[(2) - (4)].idents).len; i++) {
        Symbol *s = vec_get(&(yyvsp[(2) - (4)].idents), i);
        Node * node = ast_create_node(&decls, NVar, Void);

        if (s->type == Variable) { 
            node->value.var = (VarNode) { .symbol = *s, .statement = 1 };
        } else {
            node->node_type = NConst;
            node->value.cons = (ConstNode) { .symbol = *s, .value.bool = 1 };
        }

        tree_new_relation(&decls, 0, node->id); 
    }

    vec_drop(&(yyvsp[(2) - (4)].idents));

    tree_extend_with_subtree(&(yyval.subtree), &decls, 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0, 0);
}
    break;

  case 23:
/* Line 1802 of yacc.c  */
#line 631 "src/grammar.y"
    {
    (yyvsp[(2) - (2)].symbol).type = Function;
    assert_not_sym_exists(&(yyvsp[(2) - (2)].symbol));
    hashset_insert(&tabla, &(yyvsp[(2) - (2)].symbol));
    fun_id++;
    scope += fun_id;
}
    break;

  case 24:
/* Line 1802 of yacc.c  */
#line 638 "src/grammar.y"
    {
    Symbol *s = (Symbol *)hashset_get(&tabla, &(yyvsp[(2) - (7)].symbol));
    s->asoc_type = (yyvsp[(6) - (7)].type);
    s->info.fun = (FunctionInfo){.args=(yyvsp[(4) - (7)].idents)};
    (yyval.symbol) = *s;
}
    break;

  case 25:
/* Line 1802 of yacc.c  */
#line 644 "src/grammar.y"
    {
    (yyvsp[(2) - (2)].symbol).type = Procedure;
    assert_not_sym_exists(&(yyvsp[(2) - (2)].symbol));
    hashset_insert(&tabla, &(yyvsp[(2) - (2)].symbol));
    fun_id++;
    scope += fun_id;
}
    break;

  case 26:
/* Line 1802 of yacc.c  */
#line 651 "src/grammar.y"
    {
    Symbol *s = (Symbol *)hashset_get(&tabla, &(yyvsp[(2) - (5)].symbol));
    s->asoc_type = (DataType) { .type = Void, .size = 0};
    s->info.fun = (FunctionInfo){ .args=(yyvsp[(4) - (5)].idents) };
    (yyval.symbol) = *s;
}
    break;

  case 27:
/* Line 1802 of yacc.c  */
#line 659 "src/grammar.y"
    {
    // printf("Argumentos: %zu\n", $2.len);
    for (size_t i = 0; i < (yyvsp[(2) - (3)].idents).len; i++) {
        Symbol *s = (Symbol *)vec_get(&(yyvsp[(2) - (3)].idents), i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }

    // vec_drop(&$2);
    (yyval.idents) = (yyvsp[(2) - (3)].idents);
}
    break;

  case 28:
/* Line 1802 of yacc.c  */
#line 672 "src/grammar.y"
    {
    (yyval.idents) = vec_new(sizeof(Symbol));
}
    break;

  case 29:
/* Line 1802 of yacc.c  */
#line 675 "src/grammar.y"
    {
    for (size_t i = 0; i < (yyvsp[(1) - (3)].idents).len; i++) {
        Symbol *s = (Symbol *)vec_get(&(yyvsp[(1) - (3)].idents), i);
        s->type = Variable;
        s->asoc_type = (yyvsp[(3) - (3)].type);
        s->info.var = (VariableInfo){ .addr = addr };
        addr += data_type_size(&(yyvsp[(3) - (3)].type));
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
    (yyval.idents) = (yyvsp[(1) - (3)].idents);
}
    break;

  case 30:
/* Line 1802 of yacc.c  */
#line 686 "src/grammar.y"
    {
    (yyval.idents) = (yyvsp[(1) - (5)].idents);

    for (size_t i = 0; i < (yyvsp[(3) - (5)].idents).len; i++) {
        Symbol *s = (Symbol *)vec_get(&(yyvsp[(3) - (5)].idents), i);
        s->type = Variable;
        s->asoc_type = (yyvsp[(5) - (5)].type);
        s->info.var = (VariableInfo){ .addr = addr };
        addr += data_type_size(&(yyvsp[(5) - (5)].type));
        // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }

    vec_extend(&(yyval.idents), &(yyvsp[(3) - (5)].idents));
    // vec_drop(&$3);
}
    break;

  case 31:
/* Line 1802 of yacc.c  */
#line 703 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(2) - (3)].subtree);
}
    break;

  case 32:
/* Line 1802 of yacc.c  */
#line 706 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);

    for (size_t i = 0; i < (yyvsp[(1) - (1)].instructions).len; i++) {
        Tree *t2 = (Tree *)vec_get(&(yyvsp[(1) - (1)].instructions), i);

        if (t2->values.len == 0)
            continue;

        tree_extend_with_subtree(&(yyval.subtree), t2, 0, 0);
    }
}
    break;

  case 33:
/* Line 1802 of yacc.c  */
#line 719 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);
}
    break;

  case 34:
/* Line 1802 of yacc.c  */
#line 724 "src/grammar.y"
    {
    vec_init(&(yyval.instructions), sizeof(Tree));

    Tree * sub = vec_push(&(yyval.instructions));
    *sub = (yyvsp[(1) - (1)].subtree);
}
    break;

  case 35:
/* Line 1802 of yacc.c  */
#line 729 "src/grammar.y"
    {
    (yyval.instructions) = (yyvsp[(1) - (3)].instructions);
    vec_push(&(yyval.instructions));

    Tree * sub = vec_push(&(yyval.instructions));
    *sub = (yyvsp[(3) - (3)].subtree);
}
    break;

  case 43:
/* Line 1802 of yacc.c  */
#line 741 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NWhile, Void);

    assert_tree_asoc_type_is((yyvsp[(2) - (4)].subtree), Bool);

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (4)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0, 0);
}
    break;

  case 44:
/* Line 1802 of yacc.c  */
#line 750 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NFor, Void);
    Node * var = ast_get_root(&(yyvsp[(2) - (6)].subtree));

    switch (var->node_type) {
        case NExpr: {
            n->value.forn = (ForNode) {
                .down = 0,
                .is_assign = 0,
                .symbol = var->value.expr.value.symbol,
            };
            break;
        }
        case NAssign: {
            n->value.forn = (ForNode) {
                .down = 0,
                .is_assign = 1,
                .symbol = var->value.var.symbol,
            };
            break;
        }
        default: {
            panic("Invalid node type");
        }
    }

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(6) - (6)].subtree), 0, 0);
}
    break;

  case 45:
/* Line 1802 of yacc.c  */
#line 781 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NFor, Void);
    Node * var = ast_get_root(&(yyvsp[(2) - (6)].subtree));

    switch (var->node_type) {
        case NExpr: {
            n->value.forn = (ForNode) {
                .down = 1,
                .is_assign = 0,
                .symbol = var->value.expr.value.symbol,
            };
            break;
        }
        case NAssign: {
            n->value.forn = (ForNode) {
                .down = 1,
                .is_assign = 1,
                .symbol = var->value.var.symbol,
            };
            
            break;
        }
        default: {
            panic("Invalid node type");
        }
    }
    

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(6) - (6)].subtree), 0, 0);
}
    break;

  case 46:
/* Line 1802 of yacc.c  */
#line 817 "src/grammar.y"
    { 
    assert_sym_exists(&(yyvsp[(3) - (4)].symbol));
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NRead, Void);
    n->value.read = (ReadNode){
        .newline = 0,
        .target_symbol = (yyvsp[(3) - (4)].symbol),
    };
}
    break;

  case 47:
/* Line 1802 of yacc.c  */
#line 827 "src/grammar.y"
    { 
    assert_sym_exists(&(yyvsp[(3) - (4)].symbol)); 
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NRead, Void);
    n->value.read = (ReadNode){
        .newline = 1,
        .target_symbol = (yyvsp[(3) - (4)].symbol),
    };
}
    break;

  case 48:
/* Line 1802 of yacc.c  */
#line 839 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NWrite, Void);
    n->value.write = (WriteNode) {
        .newline = 1,
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0, 0);

}
    break;

  case 49:
/* Line 1802 of yacc.c  */
#line 849 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NWrite, Void);
    n->value.write = (WriteNode) {
        .newline = 0,
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0, 0);
}
    break;

  case 50:
/* Line 1802 of yacc.c  */
#line 860 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NIf, Void);
    n->value.ifn = (IfNode){
        .blocks = 1
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (4)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0, 0);
}
    break;

  case 51:
/* Line 1802 of yacc.c  */
#line 870 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NIf, Void);
    n->value.ifn = (IfNode){
        .blocks = 2
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(6) - (6)].subtree), 0, 0);
}
    break;

  case 52:
/* Line 1802 of yacc.c  */
#line 884 "src/grammar.y"
    {
    Node * var = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * expr = ast_get_root(&(yyvsp[(3) - (3)].subtree));

    assert_variable_assigned_type(var, expr);
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NAssign, Void);
    
    switch (var->value.expr.type) {
        case ESymbol: {
            n->value.var = (VarNode) {
                .symbol = var->value.expr.value.symbol,
                .statement = 1,
            };
            break;
        }
        case ESymbolIdx: {
            n->value.var = (VarNode) {
                .symbol = var->value.expr.value.symbol_idx.symbol,
                .statement = 1,
            };
            break;
        }
    }

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);
}
    break;

  case 53:
/* Line 1802 of yacc.c  */
#line 914 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(1) - (1)].subtree);
    
    Node * n = ast_get_root(&(yyvsp[(1) - (1)].subtree));
    if (n->node_type != NAssign) {
        panic("Invalid node type");
    }

    n->value.var.statement = 0;
    

}
    break;

  case 55:
/* Line 1802 of yacc.c  */
#line 927 "src/grammar.y"
    { 
    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (1)].symbol));
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, s->asoc_type.type);
    n->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };
}
    break;

  case 56:
/* Line 1802 of yacc.c  */
#line 938 "src/grammar.y"
    { 
    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol)); 
    bound_check(s, (yyvsp[(3) - (4)].snum));

    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, s->asoc_type.type);
    n->value.expr = (ExprNode) {
        .type = ESymbolIdx,
        .value.symbol_idx = (IndexedSymbol) {
            .symbol = *s,
            .index = (yyvsp[(3) - (4)].snum)
        }
    };
}
    break;

  case 57:
/* Line 1802 of yacc.c  */
#line 954 "src/grammar.y"
    { 
    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (1)].symbol));
    assert_sym_is_callable(s);
    assert_arguments_length(s, 0);

    tree_init(&(yyval.subtree), sizeof(Node));

    Tree args;
    tree_init(&args, sizeof(Node));

    ast_create_node(&args, NVoid, Void);

    Node * n = ast_create_node(&(yyval.subtree), NCall, s->asoc_type.type);
    n->value.call = (FunctionCall){
        .args = 0,
        .symbol = *s,
        .return_type = s->asoc_type.type,
    };

    tree_extend_with_subtree(&(yyval.subtree), &args, 0, 0);
}
    break;

  case 58:
/* Line 1802 of yacc.c  */
#line 976 "src/grammar.y"
    {
    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));
    assert_sym_is_callable(s);

    Vec children = tree_get_children(&(yyvsp[(3) - (4)].subtree), 0);
    if (assert_arguments_length(s, children.len) == 1 && children.len > 0) {
        Node * void_root = (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values, 0);

        if (void_root->node_type != NVoid) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos sin pasarle argumentos: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr, (yyvsp[(1) - (4)].symbol).name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
            Node * arg = (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values, *(size_t *)vec_get(&children, n_arg));
            Symbol * arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

            if (arg->asoc_type != arg_sym->asoc_type.type) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos de tipos distintos a los declarados: ");
                str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr, (yyvsp[(1) - (4)].symbol).name.len);
                str_push(&wrn_buff, ", el argumento ");
                str_push_sizet(&wrn_buff, n_arg + 1);
                str_push(&wrn_buff, " se esperaba de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

        
    }

    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NCall, s->asoc_type.type);
    n->value.call = (FunctionCall){
        .symbol = *s,
        .args = children.len,
    };

    vec_drop(&children);

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0, 0);
}
    break;

  case 59:
/* Line 1802 of yacc.c  */
#line 1026 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * rhs = ast_get_root(&(yyvsp[(3) - (3)].subtree));
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&(yyval.subtree), NExpr, lhs->asoc_type);
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = OpOr,
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 2);
}
    break;

  case 61:
/* Line 1802 of yacc.c  */
#line 1045 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * rhs = ast_get_root(&(yyvsp[(3) - (3)].subtree));
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&(yyval.subtree), NExpr, lhs->asoc_type);
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = OpAnd,
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 2);
}
    break;

  case 63:
/* Line 1802 of yacc.c  */
#line 1064 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(2) - (2)].subtree));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, lhs->asoc_type);
    n->value.expr = (ExprNode){
        .type = EUnaryOp,
        .value.op = OpNot,
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(2) - (2)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 1);
}
    break;

  case 65:
/* Line 1802 of yacc.c  */
#line 1080 "src/grammar.y"
    { (yyval.subtree) = (yyvsp[(2) - (3)].subtree); }
    break;

  case 67:
/* Line 1802 of yacc.c  */
#line 1082 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * rhs = ast_get_root(&(yyvsp[(3) - (3)].subtree));
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&(yyval.subtree), NExpr, Bool);
    n->value.expr = (yyvsp[(2) - (3)].expr);

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 2);
}
    break;

  case 68:
/* Line 1802 of yacc.c  */
#line 1098 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpAnd }; }
    break;

  case 69:
/* Line 1802 of yacc.c  */
#line 1099 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpOr }; }
    break;

  case 70:
/* Line 1802 of yacc.c  */
#line 1100 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpBt }; }
    break;

  case 71:
/* Line 1802 of yacc.c  */
#line 1101 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpLt }; }
    break;

  case 72:
/* Line 1802 of yacc.c  */
#line 1102 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpEbt }; }
    break;

  case 73:
/* Line 1802 of yacc.c  */
#line 1103 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpElt }; }
    break;

  case 74:
/* Line 1802 of yacc.c  */
#line 1104 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpEq };}
    break;

  case 75:
/* Line 1802 of yacc.c  */
#line 1105 "src/grammar.y"
    { (yyval.expr) = (ExprNode) { .type = EOp, .value.op = OpNeq }; }
    break;

  case 76:
/* Line 1802 of yacc.c  */
#line 1108 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (1)].subtree), 0, 0);
}
    break;

  case 77:
/* Line 1802 of yacc.c  */
#line 1114 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(1) - (3)].subtree);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);
}
    break;

  case 78:
/* Line 1802 of yacc.c  */
#line 1117 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);
}
    break;

  case 79:
/* Line 1802 of yacc.c  */
#line 1123 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (1)].subtree), 0, 0);
}
    break;

  case 80:
/* Line 1802 of yacc.c  */
#line 1129 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(1) - (3)].subtree);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);
}
    break;

  case 81:
/* Line 1802 of yacc.c  */
#line 1132 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    ast_create_node(&(yyval.subtree), NVoid, Void);

    Node * n = ast_create_node(&(yyval.subtree), NStr, Str);
    n->value.sl = (yyvsp[(1) - (1)].slice);

    tree_new_relation(&(yyval.subtree), 0, n->id);

}
    break;

  case 82:
/* Line 1802 of yacc.c  */
#line 1142 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(1) - (3)].subtree);

    Node * n = ast_create_node(&(yyval.subtree), NStr, Str);
    n->value.sl = (yyvsp[(3) - (3)].slice);

    tree_new_relation(&(yyval.subtree), 0, n->id);
}
    break;

  case 84:
/* Line 1802 of yacc.c  */
#line 1151 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * rhs = ast_get_root(&(yyvsp[(3) - (3)].subtree));
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&(yyval.subtree), NExpr, lhs->asoc_type);
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = (yyvsp[(2) - (3)].op),
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 2);
}
    break;

  case 86:
/* Line 1802 of yacc.c  */
#line 1169 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * lhs = ast_get_root(&(yyvsp[(1) - (3)].subtree));
    Node * rhs = ast_get_root(&(yyvsp[(3) - (3)].subtree));
    assert_expr_type(lhs, rhs);

    Node * n = ast_create_node(&(yyval.subtree), NExpr, lhs->asoc_type);
    n->value.expr = (ExprNode){
        .type = EOp,
        .value.op = (yyvsp[(2) - (3)].op),
    };

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0, 0);
    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0, 0);

    assert_number_of_children(&(yyval.subtree), 0, 2);
}
    break;

  case 87:
/* Line 1802 of yacc.c  */
#line 1187 "src/grammar.y"
    { 
    tree_init(&(yyval.subtree), sizeof(Node));

    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));
    assert_sym_is_callable(s);

    Vec children = tree_get_children(&(yyvsp[(3) - (4)].subtree), 0);
    if (assert_arguments_length(s, children.len) == 1 && children.len > 0) {
        Node * void_root = (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values, 0);

        if (void_root->node_type != NVoid) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos sin pasarle argumentos: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr, (yyvsp[(1) - (4)].symbol).name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        
        for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
            Node * arg = ast_get_node(&(yyvsp[(3) - (4)].subtree), *(size_t *)vec_get(&children, n_arg));
            Symbol * arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

            if (arg->asoc_type != arg_sym->asoc_type.type) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion con argumentos de tipos distintos a los declarados: ");
                str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr, (yyvsp[(1) - (4)].symbol).name.len);
                str_push(&wrn_buff, ", el argumento ");
                str_push_sizet(&wrn_buff, n_arg + 1);
                str_push(&wrn_buff, " se esperaba de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg_sym->asoc_type.type));
                str_push(&wrn_buff, " y se paso de tipo ");
                str_push(&wrn_buff, data_type_e_display_return(arg->asoc_type));
                yyerror(str_as_ref(&wrn_buff));
            }
        }

       
    }

    Node * n = ast_create_node(&(yyval.subtree), NExpr, s->asoc_type.type);
    n->value.expr = (ExprNode) {
        .type = EFunctionCall,
        .value.function_call = (FunctionCall) {
            .symbol = *s,
            .args = children.len,
            .return_type = s->asoc_type.type
        }
    };

    vec_drop(&children);   

    tree_extend_with_subtree(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0, 0);

    if (s->asoc_type.type == Void) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento usar una funcion que devuelve () como expresion: ");
        str_push_n(&wrn_buff, s->name.ptr, s->name.len);
        yyerror(str_as_ref(&wrn_buff));
    }

    assert_number_of_children(&(yyval.subtree), 0, 1);
}
    break;

  case 88:
/* Line 1802 of yacc.c  */
#line 1249 "src/grammar.y"
    { 
    tree_init(&(yyval.subtree), sizeof(Node));

    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (1)].symbol));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, s->asoc_type.type);
    n->value.expr = (ExprNode) {
        .type = ESymbol,
        .value.symbol = *s,
    };
}
    break;

  case 89:
/* Line 1802 of yacc.c  */
#line 1260 "src/grammar.y"
    { 
    tree_init(&(yyval.subtree), sizeof(Node));

    Symbol * s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol)); 

    bound_check(s, (yyvsp[(3) - (4)].snum));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, s->asoc_type.type);
    n->value.expr = (ExprNode) {
        .type = ESymbolIdx,
        .value.symbol_idx = (IndexedSymbol) {
            .symbol = *s,
            .index = (yyvsp[(3) - (4)].snum)
        }
    };
}
    break;

  case 91:
/* Line 1802 of yacc.c  */
#line 1276 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, Int);
    n->value.expr = (ExprNode) {
        .type = EIntValue,
        .value.int_value = (yyvsp[(1) - (1)].snum),
    };
}
    break;

  case 92:
/* Line 1802 of yacc.c  */
#line 1285 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, Real);
    n->value.expr = (ExprNode) {
        .type = ERealValue,
        .value.real_value = (yyvsp[(1) - (1)].fnum),
    };
}
    break;

  case 93:
/* Line 1802 of yacc.c  */
#line 1294 "src/grammar.y"
    {
    tree_init(&(yyval.subtree), sizeof(Node));

    Node * past_root = ast_get_root(&(yyvsp[(2) - (2)].subtree));

    Node * n = ast_create_node(&(yyval.subtree), NExpr, past_root->asoc_type);
    n->value.expr = (ExprNode){
        .type = EUnaryOp,
        .value.op = (yyvsp[(1) - (2)].op),
    };

    tree_root_extend(&(yyval.subtree), &(yyvsp[(2) - (2)].subtree));

    assert_number_of_children(&(yyval.subtree), 0, 1);
}
    break;

  case 94:
/* Line 1802 of yacc.c  */
#line 1309 "src/grammar.y"
    {
    (yyval.subtree) = (yyvsp[(2) - (3)].subtree);
}
    break;


/* Line 1802 of yacc.c  */
#line 3182 "grammar.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1312 "src/grammar.y"

