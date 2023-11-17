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


    #include "hashset.h"
    #include "str.h"
    #include "parser.h"

    extern int yylex(void);
    extern int main(void);
    extern void yyerror(char *s);
    extern size_t linea;
    extern size_t nchar;
    extern size_t yyleng;
    size_t scope = 0;
	size_t fun_id = 0;
	
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

    void add_reference_to_sym(Symbol * s) {
        size_t * ref = vec_push(&s->refs);
        *ref = linea;
    }

    void * assert_sym_exists(Symbol * s) {
        size_t orig_scope = s->scope;
        int found = 0;
        Symbol * res = NULL;
        
        for (size_t i = orig_scope; i >= 0; i-=orig_scope) {
            s->scope = i;
            printf("Looking %.*s in scope %zu\n", (int)s->name.len, s->name.ptr , i);
            if (hashset_contains(&tabla, s)) {
                found = 1;
                res = (Symbol *)hashset_get(&tabla, s);
                add_reference_to_sym(res);
                break;
            }
        }

        if (!found) {
            str_clear(&wrn_buff);
            char lit[] = "Error: Simbolo no declarado en el scope actual: ";
            printf("Scope: %zu\n", scope);
            str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
            str_push_n(&wrn_buff, s->name.ptr, s->name.len);
            yyerror(str_as_ref(&wrn_buff));
        } else {
            // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope, (int)s->name.len, s->name.ptr);
        }
        s->scope = orig_scope;
        return res;
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






/* Copy the first part of user declarations.  */



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
   by #include "grammar.tab.h".  */
#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */



    #ifndef LNG_PARSERH
    #define LNG_PARSERH
    
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <inttypes.h>
    #include "vector.h"
    #include "hashset.h"
    #include "str.h"

    enum SymbolType {
        Function,
        Variable,
        Constant,
        Procedure,
    };
    typedef enum SymbolType SymbolType;

    struct Symbol {
        StrSlice name;
        SymbolType type;
        size_t scope;
        size_t line;
        size_t nchar;
        Vec refs;
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
     RELOP_EQ = 287,
     RELOP_NEQ = 288,
     RELOP_BT = 289,
     RELOP_LT = 290,
     RELOP_EBT = 291,
     RELOP_ELT = 292,
     RELOP_AND = 293,
     RELOP_NOT = 294,
     RELOP_OR = 295,
     T_INT = 296,
     T_REAL = 297,
     T_STR = 298,
     T_BOOL = 299
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    int64_t snum;
    uint64_t unum;
    double fnum;
    char * ident;
    StrSlice slice;
    Symbol symbol;
    Vec idents;
    RelOp relop;
    AddOp addop;
    MulOp mulop;



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
#define YYLAST   230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNRULES -- Number of states.  */
#define YYNSTATES  211

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      45,    46,     2,     2,    49,     2,    48,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,    47,
       2,    51,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,    16,    20,    22,    24,    26,    27,
      34,    41,    48,    55,    57,    67,    69,    71,    73,    75,
      79,    80,    85,    86,    94,    95,   101,   105,   106,   110,
     116,   120,   122,   123,   125,   129,   131,   133,   135,   137,
     139,   141,   143,   148,   155,   162,   167,   172,   179,   186,
     191,   196,   203,   210,   217,   224,   229,   234,   241,   248,
     253,   260,   264,   266,   268,   270,   275,   277,   282,   286,
     288,   292,   294,   297,   299,   303,   305,   309,   311,   313,
     315,   317,   319,   321,   323,   325,   327,   331,   333,   337,
     339,   343,   348,   350,   355,   357,   359,   361,   364
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    -1,    56,    12,     3,    45,    57,    46,
      47,    58,    63,    70,    48,    -1,     3,    49,    57,    -1,
       3,    -1,    59,    -1,    60,    -1,    -1,    58,    14,    57,
      50,    61,    47,    -1,    58,    13,     3,    51,     5,    47,
      -1,    58,    13,     3,    51,     4,    47,    -1,    58,    13,
       3,    51,     6,    47,    -1,    62,    -1,    15,    52,     5,
      48,    48,     5,    53,    16,    62,    -1,    41,    -1,    42,
      -1,    43,    -1,    44,    -1,    63,    64,    47,    -1,    -1,
      65,    58,    63,    70,    -1,    -1,    17,     3,    66,    68,
      50,    62,    47,    -1,    -1,    11,     3,    67,    68,    47,
      -1,    45,    69,    46,    -1,    -1,    57,    50,    61,    -1,
      69,    47,    57,    50,    61,    -1,    18,    71,    19,    -1,
      72,    -1,    -1,    73,    -1,    72,    47,    73,    -1,    78,
      -1,    81,    -1,    70,    -1,    77,    -1,    74,    -1,    75,
      -1,    76,    -1,    24,    82,    26,    73,    -1,    25,    79,
      27,    89,    26,    73,    -1,    25,    79,    28,    89,    26,
      73,    -1,    20,    45,     3,    46,    -1,    21,    45,     3,
      46,    -1,    22,    45,     6,    49,     3,    46,    -1,    23,
      45,     6,    49,     3,    46,    -1,    22,    45,     6,    46,
      -1,    23,    45,     6,    46,    -1,    22,    45,     6,    49,
      89,    46,    -1,    23,    45,     6,    49,    89,    46,    -1,
      22,    45,     3,    49,     3,    46,    -1,    23,    45,     3,
      49,     3,    46,    -1,    22,    45,     3,    46,    -1,    23,
      45,     3,    46,    -1,    22,    45,     3,    49,    89,    46,
      -1,    23,    45,     3,    49,    89,    46,    -1,    29,    82,
      30,    73,    -1,    29,    82,    30,    73,    31,    73,    -1,
      80,    10,    89,    -1,    78,    -1,    80,    -1,     3,    -1,
       3,    52,    89,    53,    -1,     3,    -1,     3,    45,    88,
      46,    -1,    82,    40,    83,    -1,    83,    -1,    83,    38,
      84,    -1,    84,    -1,    39,    84,    -1,    85,    -1,    45,
      82,    46,    -1,    86,    -1,    89,    87,    89,    -1,    38,
      -1,    40,    -1,    34,    -1,    35,    -1,    36,    -1,    37,
      -1,    32,    -1,    33,    -1,    89,    -1,    88,    49,    89,
      -1,    90,    -1,    89,     8,    90,    -1,    92,    -1,    90,
       9,    92,    -1,     3,    45,    88,    46,    -1,     3,    -1,
       3,    52,    89,    53,    -1,    91,    -1,     5,    -1,     4,
      -1,     8,    92,    -1,    45,    89,    46,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   187,   187,   187,   206,   212,   220,   220,   220,   222,
     235,   242,   249,   258,   258,   259,   259,   259,   259,   262,
     262,   263,   266,   266,   277,   277,   290,   301,   302,   305,
     313,   314,   314,   315,   315,   316,   316,   316,   317,   317,
     317,   317,   319,   320,   321,   323,   327,   331,   335,   339,
     339,   340,   340,   341,   347,   353,   357,   361,   365,   369,
     370,   373,   374,   374,   375,   379,   383,   388,   395,   395,
     396,   396,   397,   397,   398,   398,   399,   400,   400,   400,
     400,   400,   400,   400,   400,   403,   403,   404,   404,   405,
     405,   406,   411,   411,   411,   411,   411,   411,   411
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
  "KW_ELSE", "RELOP_EQ", "RELOP_NEQ", "RELOP_BT", "RELOP_LT", "RELOP_EBT",
  "RELOP_ELT", "RELOP_AND", "RELOP_NOT", "RELOP_OR", "T_INT", "T_REAL",
  "T_STR", "T_BOOL", "'('", "')'", "';'", "'.'", "','", "':'", "'='",
  "'['", "']'", "$accept", "programa", "$@1", "ident_lista", "decl",
  "decl_var", "decl_const", "tipo", "estandard_tipo", "subprograma_decl",
  "subprograma_declaracion", "subprograma_encabezado", "$@2", "$@3",
  "argumentos", "parametros_lista", "instruccion_compuesta",
  "instrucciones_opcionales", "instrucciones_lista", "instrucciones",
  "repeticion_instruccion", "lectura_instruccion", "escritura_instruccion",
  "if_instruccion", "variable_asignacion", "for_asignacion", "variable",
  "procedure_instruccion", "relop_expresion", "relop_and", "relop_not",
  "relop_paren", "relop_expresion_simple", "relop", "expresion_lista",
  "expresion", "termino", "llamado_funcion", "factor", YY_NULL
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
     295,   296,   297,   298,   299,    40,    41,    59,    46,    44,
      58,    61,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    56,    55,    57,    57,    58,    58,    58,    59,
      60,    60,    60,    61,    61,    62,    62,    62,    62,    63,
      63,    64,    66,    65,    67,    65,    68,    68,    69,    69,
      70,    71,    71,    72,    72,    73,    73,    73,    73,    73,
      73,    73,    74,    74,    74,    75,    75,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    77,
      77,    78,    79,    79,    80,    80,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    86,    87,    87,    87,
      87,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    91,    92,    92,    92,    92,    92,    92,    92
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,    11,     3,     1,     1,     1,     0,     6,
       6,     6,     6,     1,     9,     1,     1,     1,     1,     3,
       0,     4,     0,     7,     0,     5,     3,     0,     3,     5,
       3,     1,     0,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     4,     6,     6,     4,     4,     6,     6,     4,
       4,     6,     6,     6,     6,     4,     4,     6,     6,     4,
       6,     3,     1,     1,     1,     4,     1,     4,     3,     1,
       3,     1,     2,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       3,     4,     1,     4,     1,     1,     1,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     5,     0,     0,
       0,     4,     8,    20,     6,     7,     0,     0,     0,     0,
       0,     0,     0,    32,     0,     8,     0,     0,     0,    24,
      22,    66,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    31,    33,    39,    40,    41,    38,    35,     0,    36,
      19,    20,     3,     0,     0,     0,     0,    15,    16,    17,
      18,     0,    13,    27,    27,     0,     0,     0,     0,     0,
       0,    92,    96,    95,     0,     0,     0,     0,    69,    71,
      73,    75,     0,    87,    94,    89,    64,    62,     0,    63,
       0,    30,     0,     0,     0,    11,    10,    12,     0,     9,
       0,     0,     0,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,    72,     0,     0,     0,
       0,     0,     0,    83,    84,    79,    80,    81,    82,    77,
      78,     0,     0,     0,     0,     0,    34,    61,    21,     0,
       0,     0,    25,     0,     0,    67,     0,    65,    45,    46,
      55,     0,    49,     0,    56,     0,    50,     0,     0,     0,
      74,    98,    42,    68,    70,    88,    76,    90,     0,     0,
      59,     0,     0,    26,     0,     0,    86,    92,     0,    92,
       0,    92,     0,    92,     0,    91,    93,     0,     0,     0,
       0,    28,     0,    23,    53,    57,    47,    51,    54,    58,
      48,    52,    43,    44,    60,     0,     0,     0,    29,     0,
      14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     8,    13,    14,    15,    61,    62,    18,
      24,    25,    64,    63,   101,   141,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    88,    48,    49,    77,    78,
      79,    80,    81,   131,   104,    82,    83,    84,    85
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -162
static const yytype_int16 yypact[] =
{
    -162,    26,    23,  -162,    71,    42,    97,    56,   109,    97,
      90,  -162,  -162,   122,  -162,  -162,   143,    97,   121,    66,
     113,   164,   170,   127,   141,  -162,   126,   181,   100,  -162,
    -162,     4,   144,   145,   146,   147,    13,   190,    13,  -162,
     175,   148,  -162,  -162,  -162,  -162,  -162,  -162,   186,  -162,
    -162,   122,  -162,   150,   151,   152,   149,  -162,  -162,  -162,
    -162,   153,  -162,   157,   157,    19,    19,   200,   201,     9,
      98,   114,  -162,  -162,    19,    13,    13,    -6,   167,  -162,
    -162,  -162,    91,   197,  -162,  -162,   155,  -162,    94,   186,
      63,  -162,   127,    19,   121,  -162,  -162,  -162,   203,  -162,
      97,   162,   160,    19,    20,   204,     1,   165,   168,    70,
     129,   130,   131,    19,    19,  -162,  -162,    49,    74,   127,
      13,    13,    19,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,    19,    19,    19,    19,   127,  -162,   204,  -162,   169,
     163,   136,  -162,   128,    11,  -162,    19,  -162,  -162,  -162,
    -162,    28,  -162,    34,  -162,    57,  -162,    73,   135,     2,
    -162,  -162,  -162,   167,  -162,   197,   204,  -162,    17,    33,
     184,   171,   100,  -162,    97,   173,   204,    88,    21,   108,
      38,   112,    39,   116,    40,  -162,  -162,   127,   127,   127,
     211,  -162,   172,  -162,  -162,  -162,  -162,  -162,  -162,  -162,
    -162,  -162,  -162,  -162,  -162,   174,   100,   202,  -162,   128,
    -162
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -162,  -162,  -162,    -9,   196,  -162,  -162,  -161,  -137,   177,
    -162,  -162,  -162,  -162,   159,  -162,   -14,  -162,  -162,   -91,
    -162,  -162,  -162,  -162,   187,  -162,   188,  -162,    37,   106,
     -68,  -162,  -162,  -162,   117,   -63,   107,  -162,   -69
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int16 yytable[] =
{
      11,   136,   105,   106,    26,   115,   175,   116,    20,   122,
     122,   191,   109,   118,   -64,   110,    71,    72,    73,   122,
     119,    74,    71,    72,    73,   122,     3,    74,   162,   122,
     137,   177,    72,    73,   120,     4,    74,   179,    72,    73,
     144,   122,    74,   187,   170,   208,   122,   122,   122,    65,
     105,   159,    75,   164,   147,   186,    66,   161,    76,   188,
     181,    72,    73,   167,   103,    74,   145,   195,   166,   146,
     168,   169,   210,   103,     5,    90,   183,    72,    73,   103,
     138,    74,   122,   176,   197,   199,   201,     6,   178,   120,
     180,   140,   182,   135,   184,   160,   202,   203,   204,   122,
       7,   111,   103,   120,   112,     9,   123,   124,   125,   126,
     127,   128,   129,   117,   130,    56,   150,    27,   103,   151,
     161,   133,   134,   123,   124,   125,   126,   127,   128,   129,
      31,   130,    21,   113,   194,    16,    17,    12,    22,    23,
     114,    57,    58,    59,    60,    23,    19,    32,    33,    34,
      35,    36,    37,   113,   196,    10,    38,   113,   198,   113,
     114,   113,   200,    28,   114,   192,   114,    29,   114,    57,
      58,    59,    60,    30,    52,   152,   154,   156,   153,   155,
     157,   185,   173,   174,   146,    53,    54,    55,    50,    67,
      68,    69,    70,    86,    91,    92,    93,    95,    96,    97,
      99,    98,   100,   107,   108,   121,   132,    66,   139,   142,
     143,   148,   122,   172,   149,   189,   205,   171,   209,   190,
     193,    51,   206,   102,    87,    89,   163,   207,    94,   165,
     158
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-162)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
       9,    92,    65,    66,    18,    74,   143,    75,    17,     8,
       8,   172,     3,    76,    10,     6,     3,     4,     5,     8,
      26,     8,     3,     4,     5,     8,     0,     8,   119,     8,
      93,     3,     4,     5,    40,    12,     8,     3,     4,     5,
     103,     8,     8,    26,   135,   206,     8,     8,     8,    45,
     113,   114,    39,   121,    53,    53,    52,    46,    45,    26,
       3,     4,     5,   132,    45,     8,    46,    46,   131,    49,
     133,   134,   209,    45,     3,    38,     3,     4,     5,    45,
      94,     8,     8,   146,    46,    46,    46,    45,   151,    40,
     153,   100,   155,    30,   157,    46,   187,   188,   189,     8,
       3,     3,    45,    40,     6,    49,    32,    33,    34,    35,
      36,    37,    38,    76,    40,    15,    46,    51,    45,    49,
      46,    27,    28,    32,    33,    34,    35,    36,    37,    38,
       3,    40,    11,    45,    46,    13,    14,    47,    17,    18,
      52,    41,    42,    43,    44,    18,     3,    20,    21,    22,
      23,    24,    25,    45,    46,    46,    29,    45,    46,    45,
      52,    45,    46,    50,    52,   174,    52,     3,    52,    41,
      42,    43,    44,     3,    48,    46,    46,    46,    49,    49,
      49,    46,    46,    47,    49,     4,     5,     6,    47,    45,
      45,    45,    45,     3,    19,    47,    10,    47,    47,    47,
      47,    52,    45,     3,     3,    38,     9,    52,     5,    47,
      50,    46,     8,    50,    46,    31,     5,    48,    16,    48,
      47,    25,    50,    64,    37,    37,   120,    53,    51,   122,
     113
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    55,    56,     0,    12,     3,    45,     3,    57,    49,
      46,    57,    47,    58,    59,    60,    13,    14,    63,     3,
      57,    11,    17,    18,    64,    65,    70,    51,    50,     3,
       3,     3,    20,    21,    22,    23,    24,    25,    29,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    80,    81,
      47,    58,    48,     4,     5,     6,    15,    41,    42,    43,
      44,    61,    62,    67,    66,    45,    52,    45,    45,    45,
      45,     3,     4,     5,     8,    39,    45,    82,    83,    84,
      85,    86,    89,    90,    91,    92,     3,    78,    79,    80,
      82,    19,    47,    10,    63,    47,    47,    47,    52,    47,
      45,    68,    68,    45,    88,    89,    89,     3,     3,     3,
       6,     3,     6,    45,    52,    92,    84,    82,    89,    26,
      40,    38,     8,    32,    33,    34,    35,    36,    37,    38,
      40,    87,     9,    27,    28,    30,    73,    89,    70,     5,
      57,    69,    47,    50,    89,    46,    49,    53,    46,    46,
      46,    49,    46,    49,    46,    49,    46,    49,    88,    89,
      46,    46,    73,    83,    84,    90,    89,    92,    89,    89,
      73,    48,    50,    46,    47,    62,    89,     3,    89,     3,
      89,     3,    89,     3,    89,    46,    53,    26,    26,    31,
      48,    61,    57,    47,    46,    46,    46,    46,    46,    46,
      46,    46,    73,    73,    73,     5,    50,    53,    61,    16,
      62
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
      case 57: /* ident_lista */

        { 
    printf("Dropping ident_lista:  ");
    vec_debug_verbose(&((*yyvaluep).idents));
    vec_drop(&((*yyvaluep).idents));
};

        break;
      case 69: /* parametros_lista */

        { 
    printf("Dropping parametros_lista:  ");
    vec_debug_verbose(&((*yyvaluep).idents));
    vec_drop(&((*yyvaluep).idents));
};

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

    { 
    puts("Init symbol table");
    hashset_init(&tabla, sizeof(Symbol), hash_symbol); 
    puts("Init warning buffer");
    str_init(&wrn_buff);
}
    break;

  case 3:

    {
    printf("Programa: %.*s\n", (int)(yyvsp[(3) - (11)].symbol).name.len, (yyvsp[(3) - (11)].symbol).name.ptr);
    printf("Entradas: %zu\n", (yyvsp[(5) - (11)].idents).len);
    for (size_t i=0; i < (yyvsp[(5) - (11)].idents).len; i++) {
        Symbol * s = vec_get(&(yyvsp[(5) - (11)].idents), i);
        assert_not_sym_exists(s);
        s->type = Variable;
        hashset_insert(&tabla, s);
        printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[(5) - (11)].idents));
}
    break;

  case 4:

    {
    (yyval.idents) = (yyvsp[(3) - (3)].idents);
    // vec_debug_verbose(&$$);
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[(1) - (3)].symbol);
}
    break;

  case 5:

    {
    (yyval.idents) = vec_new(sizeof(Symbol));
    // vec_debug_verbose(&$$);
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[(1) - (1)].symbol);
}
    break;

  case 9:

    {
    printf("Declarando variables: %zu\n", (yyvsp[(3) - (6)].idents).len);
    for (size_t i=0; i < (yyvsp[(3) - (6)].idents).len; i++) {
        Symbol * s = vec_get(&(yyvsp[(3) - (6)].idents), i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
        printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[(3) - (6)].idents));
}
    break;

  case 10:

    {
    printf("Declarando constante: %lld\n", (yyvsp[(5) - (6)].snum));
    Symbol s = (yyvsp[(3) - (6)].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 11:

    {
    printf("Declarando constante: %f\n", (yyvsp[(5) - (6)].fnum));
    Symbol s = (yyvsp[(3) - (6)].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 12:

    {
    printf("Declarando constante: %.*s\n", (yyvsp[(5) - (6)].slice).len, (yyvsp[(5) - (6)].slice).ptr);
    Symbol s = (yyvsp[(3) - (6)].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 21:

    {
    scope-=fun_id;
}
    break;

  case 22:

    {
    printf("Declarando funcion %.*s\n", (int)(yyvsp[(2) - (2)].symbol).name.len, (yyvsp[(2) - (2)].symbol).name.ptr);
	Symbol s = (yyvsp[(2) - (2)].symbol);
    s.type = Function;
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
    fun_id++;
	scope+=fun_id;
}
    break;

  case 23:

    {
    printf("Declarada %.*s\n", (int)(yyvsp[(2) - (7)].symbol).name.len, (yyvsp[(2) - (7)].symbol).name.ptr);
}
    break;

  case 24:

    {
    printf("Declarando procedure %.*s\n", (int)(yyvsp[(2) - (2)].symbol).name.len, (yyvsp[(2) - (2)].symbol).name.ptr);
	Symbol s = (yyvsp[(2) - (2)].symbol);
    s.type = Procedure;
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
    fun_id++;
	scope+=fun_id;
}
    break;

  case 25:

    {
    printf("Declarada %.*s\n", (int)(yyvsp[(2) - (5)].symbol).name.len, (yyvsp[(2) - (5)].symbol).name.ptr);
}
    break;

  case 26:

    {
    printf("Argumentos: %zu\n", (yyvsp[(2) - (3)].idents).len);
    for (size_t i=0; i < (yyvsp[(2) - (3)].idents).len; i++) {
        Symbol * s = (Symbol *)vec_get(&(yyvsp[(2) - (3)].idents), i);
        s->type = Constant;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
		printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[(2) - (3)].idents));
}
    break;

  case 28:

    {
    (yyval.idents) = (yyvsp[(1) - (3)].idents);
}
    break;

  case 29:

    {
    (yyval.idents) = (yyvsp[(1) - (5)].idents);
    vec_extend(&(yyval.idents), &(yyvsp[(3) - (5)].idents));
    vec_drop(&(yyvsp[(3) - (5)].idents));  
}
    break;

  case 45:

    {
    Symbol s = (yyvsp[(3) - (4)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 46:

    { 
    Symbol s = (yyvsp[(3) - (4)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 47:

    {
    Symbol s = (yyvsp[(5) - (6)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 48:

    {
    Symbol s = (yyvsp[(5) - (6)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 53:

    {
    Symbol s = (yyvsp[(3) - (6)].symbol);
    Symbol s1 = (yyvsp[(5) - (6)].symbol);
    assert_sym_exists(&s);
    assert_sym_exists(&s1);
}
    break;

  case 54:

    {
    Symbol s = (yyvsp[(3) - (6)].symbol);
    Symbol s1 = (yyvsp[(5) - (6)].symbol);
    assert_sym_exists(&s);
    assert_sym_exists(&s1);
}
    break;

  case 55:

    {
    Symbol s = (yyvsp[(3) - (4)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 56:

    {
    Symbol s = (yyvsp[(3) - (4)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 57:

    {
    Symbol s = (yyvsp[(3) - (6)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 58:

    {
    Symbol s = (yyvsp[(3) - (6)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 64:

    {
    Symbol s = (yyvsp[(1) - (1)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 65:

    {
    Symbol s = (yyvsp[(1) - (4)].symbol);
    assert_sym_exists(&s);
}
    break;

  case 66:

    {
    Symbol s = (yyvsp[(1) - (1)].symbol);
    printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
    assert_sym_exists(&s);
}
    break;

  case 67:

    {
    Symbol s = (yyvsp[(1) - (4)].symbol);
    printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
    assert_sym_exists(&s);
}
    break;

  case 91:

    {
    Symbol s = (yyvsp[(1) - (4)].symbol);
    printf("Llamando funcion: %.*s\n", (int)s.name.len, s.name.ptr);
    assert_sym_exists(&s);
}
    break;



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




