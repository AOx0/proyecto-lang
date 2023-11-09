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
extern int yylex(void);
extern int main(void);
extern void yyerror(char *s);
extern size_t linea;

struct Symbol {
        StrSlice name;
        size_t scope;
        size_t line;
};

typedef struct Symbol Symbol;
HashSet tabla;
String wrn_buff;

HashIdx hash_symbol(void *s) {
    HashIdx res;
    res.idx = 0;

    Symbol *sy = (Symbol *)s;

    for (size_t i = 0; i < sy->name.len && i < 5; i++) {
        res.idx += sy->name.ptr[i] * (i + 1);
    }

    res.idx *= sy->scope + 1;

    // printf("HASH: %zu\n", res.idx);

    return res;
}

void assert_sym_exists(Symbol *s) {
    if (!hashset_contains(&tabla, s)) {
        str_clear(&wrn_buff);
        char lit[] = "Error: Simbolo no declarado en el scope actual: ";
        str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
        str_push_n(&wrn_buff, s->name.ptr, s->name.len);
        yyerror(str_as_ref(&wrn_buff));
    } else {
        printf("Existe %.*s\n", (int)s->name.len, s->name.ptr);
    }
}

void assert_not_sym_exists(Symbol *s) {
    if (hashset_contains(&tabla, s)) {
        str_clear(&wrn_buff);
        char lit[] = "Error: Simbolo ya declarado en el mismo scope: ";
        str_push_n(&wrn_buff, &lit[0], strlen(&lit[0]));
        str_push_n(&wrn_buff, s->name.ptr, s->name.len);
        yyerror(str_as_ref(&wrn_buff));
    } else {
        printf("No existe %.*s\n", (int)s->name.len, s->name.ptr);
    }
}

/* Copy the first part of user declarations.  */

#ifndef YY_NULL
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULL nullptr
#else
#define YY_NULL 0
#endif
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
#undef YYERROR_VERBOSE
#define YYERROR_VERBOSE 1
#else
#define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "grammar.tab.h".  */
#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
#define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

#include "hashset.h"
#include "str.h"
#include "vector.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin, *yyout;

enum RelOp { And, Or };
typedef enum RelOp RelOp;

enum AddOp { Add, Sub };
typedef enum AddOp AddOp;

enum MulOp { Div, Mod, Mul };
typedef enum MulOp MulOp;

/* Tokens.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
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
    RELOP_AND = 287,
    RELOP_NOT = 288,
    RELOP_OR = 289,
    T_INT = 290,
    T_REAL = 291,
    T_STR = 292,
    T_BOOL = 293
};
#endif

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

        int64_t snum;
        uint64_t unum;
        double fnum;
        char *ident;
        StrSlice slice;
        Vec idents;
        RelOp relop;
        AddOp addop;
        MulOp mulop;

} YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define yystype YYSTYPE /* obsolescent; will be withdrawn */
#define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse(void *YYPARSE_PARAM);
#else
int yyparse();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse(void);
#else
int yyparse();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#ifdef short
#undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||     \
       defined _MSC_VER)
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
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif !defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ ||       \
                            defined __cplusplus || defined _MSC_VER)
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned int
#endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T)-1)

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
#if (!defined __GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#define __attribute__(Spec) /* empty */
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YYUSE(E) ((void)(E))
#else
#define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
#define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static int YYID(int yyi)
#else
static int YYID(yyi)
int yyi;
#endif
{
    return yyi;
}
#endif

#if !defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#ifdef YYSTACK_USE_ALLOCA
#if YYSTACK_USE_ALLOCA
#ifdef __GNUC__
#define YYSTACK_ALLOC __builtin_alloca
#elif defined __BUILTIN_VA_ARG_INCR
#include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#elif defined _AIX
#define YYSTACK_ALLOC __alloca
#elif defined _MSC_VER
#include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#define alloca _alloca
#else
#define YYSTACK_ALLOC alloca
#if !defined _ALLOCA_H && !defined EXIT_SUCCESS &&                             \
    (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#endif
#endif
#endif

#ifdef YYSTACK_ALLOC
/* Pacify GCC's `empty if-body' warning.  */
#define YYSTACK_FREE(Ptr)                                                      \
    do { /* empty */                                                           \
        ;                                                                      \
    } while (YYID(0))
#ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#endif
#else
#define YYSTACK_ALLOC YYMALLOC
#define YYSTACK_FREE YYFREE
#ifndef YYSTACK_ALLOC_MAXIMUM
#define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#endif
#if (defined __cplusplus && !defined EXIT_SUCCESS &&                           \
     !((defined YYMALLOC || defined malloc) &&                                 \
       (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined EXIT_SUCCESS &&                                \
    (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined EXIT_SUCCESS &&                                  \
    (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
void free(void *);      /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */

#if (!defined yyoverflow &&                                                    \
     (!defined __cplusplus ||                                                  \
      (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
        yytype_int16 yyss_alloc;
        YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (sizeof(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N)                                                       \
    ((N) * (sizeof(yytype_int16) + sizeof(YYSTYPE)) + YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                                   \
    do {                                                                       \
        YYSIZE_T yynewbytes;                                                   \
        YYCOPY(&yyptr->Stack_alloc, Stack, yysize);                            \
        Stack = &yyptr->Stack_alloc;                                           \
        yynewbytes = yystacksize * sizeof(*Stack) + YYSTACK_GAP_MAXIMUM;       \
        yyptr += yynewbytes / sizeof(*yyptr);                                  \
    } while (YYID(0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count)                                                \
    __builtin_memcpy(Dst, Src, (Count) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)                                                \
    do {                                                                       \
        YYSIZE_T yyi;                                                          \
        for (yyi = 0; yyi < (Count); yyi++)                                    \
            (Dst)[yyi] = (Src)[yyi];                                           \
    } while (YYID(0))
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 39
/* YYNRULES -- Number of rules.  */
#define YYNRULES 92
/* YYNRULES -- Number of states.  */
#define YYNSTATES 205

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 293

#define YYTRANSLATE(YYX)                                                       \
    ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  39, 40, 2,  2,  43, 2,  42, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  44, 41, 2,  45, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  46, 2,  47, 2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,   0,   3,   4,   16,  20,  22,  24,  26,  27,  34,  41,  48,  55,
    57,  67,  69,  71,  73,  75,  79,  80,  85,  86,  94,  95,  101, 105,
    106, 110, 116, 120, 122, 123, 125, 129, 131, 133, 135, 137, 139, 141,
    143, 148, 155, 162, 167, 172, 179, 186, 191, 196, 203, 210, 217, 224,
    229, 234, 241, 248, 253, 260, 264, 266, 268, 270, 275, 277, 282, 286,
    288, 292, 294, 297, 299, 303, 305, 309, 311, 313, 315, 319, 321, 325,
    327, 331, 336, 338, 343, 345, 347, 349, 352};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] = {
    49, 0,  -1, -1, 50, 12, 3,  39, 51, 40, 41, 52, 57, 64, 42, -1, 3,  43, 51,
    -1, 3,  -1, 53, -1, 54, -1, -1, 52, 14, 51, 44, 55, 41, -1, 52, 13, 3,  45,
    5,  41, -1, 52, 13, 3,  45, 4,  41, -1, 52, 13, 3,  45, 6,  41, -1, 56, -1,
    15, 46, 5,  42, 42, 5,  47, 16, 56, -1, 35, -1, 36, -1, 37, -1, 38, -1, 57,
    58, 41, -1, -1, 59, 52, 57, 64, -1, -1, 17, 3,  60, 62, 44, 56, 41, -1, -1,
    11, 3,  61, 62, 41, -1, 39, 63, 40, -1, -1, 51, 44, 55, -1, 63, 41, 51, 44,
    55, -1, 18, 65, 19, -1, 66, -1, -1, 67, -1, 66, 41, 67, -1, 72, -1, 75, -1,
    64, -1, 71, -1, 68, -1, 69, -1, 70, -1, 24, 76, 26, 67, -1, 25, 73, 27, 83,
    26, 67, -1, 25, 73, 28, 83, 26, 67, -1, 20, 39, 3,  40, -1, 21, 39, 3,  40,
    -1, 22, 39, 6,  43, 3,  40, -1, 23, 39, 6,  43, 3,  40, -1, 22, 39, 6,  40,
    -1, 23, 39, 6,  40, -1, 22, 39, 6,  43, 83, 40, -1, 23, 39, 6,  43, 83, 40,
    -1, 22, 39, 3,  43, 3,  40, -1, 23, 39, 3,  43, 3,  40, -1, 22, 39, 3,  40,
    -1, 23, 39, 3,  40, -1, 22, 39, 3,  43, 83, 40, -1, 23, 39, 3,  43, 83, 40,
    -1, 29, 76, 30, 67, -1, 29, 76, 30, 67, 31, 67, -1, 74, 10, 83, -1, 72, -1,
    74, -1, 3,  -1, 3,  46, 83, 47, -1, 3,  -1, 3,  39, 82, 40, -1, 76, 34, 77,
    -1, 77, -1, 77, 32, 78, -1, 78, -1, 33, 78, -1, 79, -1, 39, 76, 40, -1, 80,
    -1, 83, 81, 83, -1, 32, -1, 34, -1, 83, -1, 82, 43, 83, -1, 84, -1, 83, 8,
    84, -1, 86, -1, 84, 9,  86, -1, 3,  39, 82, 40, -1, 3,  -1, 3,  46, 83, 47,
    -1, 85, -1, 5,  -1, 4,  -1, 8,  86, -1, 39, 83, 40, -1};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   137, 137, 137, 151, 157, 165, 165, 165, 167, 177, 180, 183, 188,
    188, 189, 189, 189, 189, 192, 192, 193, 194, 194, 196, 196, 203, 209,
    210, 213, 221, 222, 222, 223, 223, 224, 224, 224, 225, 225, 225, 225,
    227, 228, 229, 231, 235, 239, 239, 243, 243, 244, 244, 245, 245, 251,
    255, 259, 263, 267, 268, 271, 272, 272, 273, 273, 277, 277, 280, 280,
    281, 281, 282, 282, 283, 283, 284, 285, 285, 288, 288, 289, 289, 290,
    290, 291, 292, 292, 292, 292, 292, 292, 292};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] = {"$end",
                                      "error",
                                      "$undefined",
                                      "IDENT",
                                      "CONST_REAL",
                                      "CONST_ENTERA",
                                      "CONST_CADENA",
                                      "RELOP",
                                      "ADDOP",
                                      "MULOP",
                                      "OP_ASIGN",
                                      "KW_PROCEDURE",
                                      "KW_PROG",
                                      "KW_CONST",
                                      "KW_VAR",
                                      "KW_ARRAY",
                                      "KW_OF",
                                      "KW_FUNC",
                                      "KW_BEGIN",
                                      "KW_END",
                                      "KW_READ",
                                      "KW_READLN",
                                      "KW_WRITE",
                                      "KW_WRITELN",
                                      "KW_WHILE",
                                      "KW_FOR",
                                      "KW_DO",
                                      "KW_TO",
                                      "KW_DOWNTO",
                                      "KW_IF",
                                      "KW_THEN",
                                      "KW_ELSE",
                                      "RELOP_AND",
                                      "RELOP_NOT",
                                      "RELOP_OR",
                                      "T_INT",
                                      "T_REAL",
                                      "T_STR",
                                      "T_BOOL",
                                      "'('",
                                      "')'",
                                      "';'",
                                      "'.'",
                                      "','",
                                      "':'",
                                      "'='",
                                      "'['",
                                      "']'",
                                      "$accept",
                                      "programa",
                                      "$@1",
                                      "ident_lista",
                                      "decl",
                                      "decl_var",
                                      "decl_const",
                                      "tipo",
                                      "estandard_tipo",
                                      "subprograma_decl",
                                      "subprograma_declaracion",
                                      "subprograma_encabezado",
                                      "$@2",
                                      "$@3",
                                      "argumentos",
                                      "parametros_lista",
                                      "instruccion_compuesta",
                                      "instrucciones_opcionales",
                                      "instrucciones_lista",
                                      "instrucciones",
                                      "repeticion_instruccion",
                                      "lectura_instruccion",
                                      "escritura_instruccion",
                                      "if_instruccion",
                                      "variable_asignacion",
                                      "for_asignacion",
                                      "variable",
                                      "procedure_instruccion",
                                      "relop_expresion",
                                      "relop_and",
                                      "relop_not",
                                      "relop_paren",
                                      "relop_expresion_simple",
                                      "relop",
                                      "expresion_lista",
                                      "expresion",
                                      "termino",
                                      "llamado_funcion",
                                      "factor",
                                      YY_NULL};
#endif

#ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] = {
    0,   256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266,
    267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278,
    279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290,
    291, 292, 293, 40,  41,  59,  46,  44,  58,  61,  91,  93};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  48, 50, 49, 51, 51, 52, 52, 52, 53, 54, 54, 54, 55, 55, 56, 56, 56, 56,
    57, 57, 58, 60, 59, 61, 59, 62, 62, 63, 63, 64, 65, 65, 66, 66, 67, 67, 67,
    67, 67, 67, 67, 68, 68, 68, 69, 69, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
    70, 70, 71, 71, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79,
    80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 86, 86, 86, 86, 86, 86, 86};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 0, 11, 3, 1, 1, 1, 0, 6, 6, 6, 6, 1, 9, 1, 1, 1, 1, 3, 0, 4, 0, 7,
    0, 5, 3, 0,  3, 5, 3, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 4, 6, 6, 4, 4, 6,
    6, 4, 4, 6,  6, 6, 6, 4, 4, 6, 6, 4, 6, 3, 1, 1, 1, 4, 1, 4, 3, 1, 3, 1,
    2, 1, 3, 1,  3, 1, 1, 1, 3, 1, 3, 1, 3, 4, 1, 4, 1, 1, 1, 2, 3};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    2,  0,  0,  1,  0,  0,  0,  5,  0,  0,  0,  4,  8,  20, 6,  7,  0,  0,  0,
    0,  0,  0,  0,  32, 0,  8,  0,  0,  0,  24, 22, 66, 0,  0,  0,  0,  0,  0,
    0,  37, 0,  31, 33, 39, 40, 41, 38, 35, 0,  36, 19, 20, 3,  0,  0,  0,  0,
    15, 16, 17, 18, 0,  13, 27, 27, 0,  0,  0,  0,  0,  0,  86, 90, 89, 0,  0,
    0,  0,  69, 71, 73, 75, 0,  81, 88, 83, 64, 62, 0,  63, 0,  30, 0,  0,  0,
    11, 10, 12, 0,  9,  0,  0,  0,  0,  0,  79, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  91, 72, 0,  0,  0,  0,  0,  0,  77, 78, 0,  0,  0,  0,  0,  34, 61, 21,
    0,  0,  0,  25, 0,  0,  67, 0,  65, 45, 46, 55, 0,  49, 0,  56, 0,  50, 0,
    0,  0,  74, 92, 42, 68, 70, 82, 76, 84, 0,  0,  59, 0,  0,  26, 0,  0,  80,
    86, 0,  86, 0,  86, 0,  86, 0,  85, 87, 0,  0,  0,  0,  28, 0,  23, 53, 57,
    47, 51, 54, 58, 48, 52, 43, 44, 60, 0,  0,  0,  29, 0,  14};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1, 1,   2,   8,  13, 14, 15, 61,  62,  18, 24, 25, 64,
    63, 101, 135, 39, 40, 41, 42, 43,  44,  45, 46, 47, 88,
    48, 49,  77,  78, 79, 80, 81, 125, 104, 82, 83, 84, 85};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -142
static const yytype_int16 yypact[] = {
    -142, 79,   16,   -142, 100,  117,  131,  102,  128,  131,  129,  -142,
    -142, 108,  -142, -142, 166,  131,  112,  127,  130,  168,  170,  95,
    134,  -142, 135,  106,  90,   -142, -142, 14,   137,  139,  140,  141,
    12,   178,  12,   -142, 163,  142,  -142, -142, -142, -142, -142, -142,
    174,  -142, -142, 108,  -142, 144,  145,  146,  143,  -142, -142, -142,
    -142, 147,  -142, 151,  151,  18,   18,   188,  189,  35,   59,   43,
    -142, -142, 18,   12,   12,   -7,   161,  -142, -142, -142, 3,    185,
    -142, -142, 149,  -142, 125,  174,  -16,  -142, 95,   18,   112,  -142,
    -142, -142, 191,  -142, 131,  156,  154,  18,   103,  192,  -1,   159,
    162,  114,  115,  120,  121,  18,   18,   -142, -142, 74,   67,   95,
    12,   12,   18,   -142, -142, 18,   18,   18,   18,   95,   -142, 192,
    -142, 164,  157,  126,  -142, 113,  26,   -142, 18,   -142, -142, -142,
    -142, 28,   -142, 39,   -142, 65,   -142, 92,   122,  5,    -142, -142,
    -142, 161,  -142, 185,  192,  -142, 64,   76,   172,  165,  90,   -142,
    131,  167,  192,  93,   46,   96,   48,   98,   66,   101,  69,   -142,
    -142, 95,   95,   95,   199,  -142, 169,  -142, -142, -142, -142, -142,
    -142, -142, -142, -142, -142, -142, -142, 158,  90,   193,  -142, 113,
    -142};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -142, -142, -142, -9,  186,  -142, -142, -141, -132, 171,  -142, -142, -142,
    -142, 148,  -142, -14, -142, -142, -89,  -142, -142, -142, -142, 173,  -142,
    177,  -142, -28,  97,  -66,  -142, -142, -142, 105,  -64,  94,   -142, -68};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int16 yytable[] = {
    11,  105, 106, 130, 26,  169, 115, 122, 20,  116, 90,  122, 118, 122, 129,
    71,  72,  73,  120, 119, 74,  71,  72,  73,  -64, 185, 74,  120, 4,   131,
    156, 171, 72,  73,  122, 123, 74,  124, 109, 138, 164, 110, 173, 72,  73,
    75,  141, 74,  117, 105, 153, 76,  180, 65,  122, 158, 122, 103, 161, 202,
    66,  160, 111, 162, 163, 112, 155, 103, 175, 72,  73,  204, 122, 74,  122,
    122, 170, 122, 103, 3,   132, 172, 113, 174, 122, 176, 189, 178, 191, 114,
    181, 134, 196, 197, 198, 177, 72,  73,  31,  123, 74,  124, 182, 5,   103,
    56,  193, 155, 120, 195, 53,  54,  55,  23,  154, 32,  33,  34,  35,  36,
    37,  16,  17,  21,  38,  57,  58,  59,  60,  22,  23,  103, 113, 188, 7,
    113, 190, 113, 192, 114, 113, 194, 114, 139, 114, 9,   140, 114, 57,  58,
    59,  60,  127, 128, 144, 146, 6,   145, 147, 186, 148, 150, 179, 149, 151,
    140, 167, 168, 10,  19,  12,  29,  27,  30,  28,  50,  67,  52,  68,  69,
    70,  86,  91,  92,  93,  95,  96,  97,  99,  98,  100, 107, 108, 121, 126,
    66,  133, 136, 137, 142, 122, 166, 143, 183, 199, 201, 165, 184, 187, 203,
    87,  51,  102, 200, 89,  0,   159, 157, 152, 0,   0,   0,   94};

#define yypact_value_is_default(Yystate) (!!((Yystate) == (-142)))

#define yytable_value_is_error(Yytable_value) YYID(0)

static const yytype_int16 yycheck[] = {
    9,   65,  66,  92,  18,  137, 74,  8,   17,  75,  38,  8,   76,  8,   30,
    3,   4,   5,   34,  26,  8,   3,   4,   5,   10,  166, 8,   34,  12,  93,
    119, 3,   4,   5,   8,   32,  8,   34,  3,   103, 129, 6,   3,   4,   5,
    33,  47,  8,   76,  113, 114, 39,  47,  39,  8,   121, 8,   39,  126, 200,
    46,  125, 3,   127, 128, 6,   40,  39,  3,   4,   5,   203, 8,   8,   8,
    8,   140, 8,   39,  0,   94,  145, 39,  147, 8,   149, 40,  151, 40,  46,
    26,  100, 181, 182, 183, 3,   4,   5,   3,   32,  8,   34,  26,  3,   39,
    15,  40,  40,  34,  40,  4,   5,   6,   18,  40,  20,  21,  22,  23,  24,
    25,  13,  14,  11,  29,  35,  36,  37,  38,  17,  18,  39,  39,  40,  3,
    39,  40,  39,  40,  46,  39,  40,  46,  40,  46,  43,  43,  46,  35,  36,
    37,  38,  27,  28,  40,  40,  39,  43,  43,  168, 40,  40,  40,  43,  43,
    43,  40,  41,  40,  3,   41,  3,   45,  3,   44,  41,  39,  42,  39,  39,
    39,  3,   19,  41,  10,  41,  41,  41,  41,  46,  39,  3,   3,   32,  9,
    46,  5,   41,  44,  40,  8,   44,  40,  31,  5,   47,  42,  42,  41,  16,
    37,  25,  64,  44,  37,  -1,  122, 120, 113, -1,  -1,  -1,  51};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  49, 50, 0,  12, 3,  39, 3,  51, 43, 40, 51, 41, 52, 53, 54, 13, 14, 57,
    3,  51, 11, 17, 18, 58, 59, 64, 45, 44, 3,  3,  3,  20, 21, 22, 23, 24, 25,
    29, 64, 65, 66, 67, 68, 69, 70, 71, 72, 74, 75, 41, 52, 42, 4,  5,  6,  15,
    35, 36, 37, 38, 55, 56, 61, 60, 39, 46, 39, 39, 39, 39, 3,  4,  5,  8,  33,
    39, 76, 77, 78, 79, 80, 83, 84, 85, 86, 3,  72, 73, 74, 76, 19, 41, 10, 57,
    41, 41, 41, 46, 41, 39, 62, 62, 39, 82, 83, 83, 3,  3,  3,  6,  3,  6,  39,
    46, 86, 78, 76, 83, 26, 34, 32, 8,  32, 34, 81, 9,  27, 28, 30, 67, 83, 64,
    5,  51, 63, 41, 44, 83, 40, 43, 47, 40, 40, 40, 43, 40, 43, 40, 43, 40, 43,
    82, 83, 40, 40, 67, 77, 78, 84, 83, 86, 83, 83, 67, 42, 44, 40, 41, 56, 83,
    3,  83, 3,  83, 3,  83, 3,  83, 40, 47, 26, 26, 31, 42, 55, 51, 41, 40, 40,
    40, 40, 40, 40, 40, 40, 67, 67, 67, 5,  44, 47, 55, 16, 56};

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY (-2)
#define YYEOF 0

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL goto yyerrlab
#if defined YYFAIL
/* This is here to suppress warnings from the GCC cpp's
   -Wunused-macros.  Normally we don't worry about that warning, but
   some users do, and we want to make it easy for users to remove
   YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                                 \
    do                                                                         \
        if (yychar == YYEMPTY) {                                               \
            yychar = (Token);                                                  \
            yylval = (Value);                                                  \
            YYPOPSTACK(yylen);                                                 \
            yystate = *yyssp;                                                  \
            goto yybackup;                                                     \
        } else {                                                               \
            yyerror(YY_("syntax error: cannot back up"));                      \
            YYERROR;                                                           \
        }                                                                      \
    while (YYID(0))

/* Error token number */
#define YYTERROR 1
#define YYERRCODE 256

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
#define YY_LOCATION_PRINT(File, Loc) ((void)0)
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
#define YYLEX yylex(YYLEX_PARAM)
#else
#define YYLEX yylex()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

#ifndef YYFPRINTF
#include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)                                                        \
    do {                                                                       \
        if (yydebug)                                                           \
            YYFPRINTF Args;                                                    \
    } while (YYID(0))

#define YY_SYMBOL_PRINT(Title, Type, Value, Location)                          \
    do {                                                                       \
        if (yydebug) {                                                         \
            YYFPRINTF(stderr, "%s ", Title);                                   \
            yy_symbol_print(stderr, Type, Value);                              \
            YYFPRINTF(stderr, "\n");                                           \
        }                                                                      \
    } while (YYID(0))

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static void yy_symbol_value_print(FILE *yyoutput, int yytype,
                                  YYSTYPE const *const yyvaluep)
#else
static void yy_symbol_value_print(yyoutput, yytype, yyvaluep) FILE *yyoutput;
int yytype;
YYSTYPE const *const yyvaluep;
#endif
{
    FILE *yyo = yyoutput;
    YYUSE(yyo);
    if (!yyvaluep)
        return;
#ifdef YYPRINT
    if (yytype < YYNTOKENS)
        YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
#else
    YYUSE(yyoutput);
#endif
    YYUSE(yytype);
}

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static void yy_symbol_print(FILE *yyoutput, int yytype,
                            YYSTYPE const *const yyvaluep)
#else
static void yy_symbol_print(yyoutput, yytype, yyvaluep) FILE *yyoutput;
int yytype;
YYSTYPE const *const yyvaluep;
#endif
{
    if (yytype < YYNTOKENS)
        YYFPRINTF(yyoutput, "token %s (", yytname[yytype]);
    else
        YYFPRINTF(yyoutput, "nterm %s (", yytname[yytype]);

    yy_symbol_value_print(yyoutput, yytype, yyvaluep);
    YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static void yy_stack_print(yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void yy_stack_print(yybottom, yytop) yytype_int16 *yybottom;
yytype_int16 *yytop;
#endif
{
    YYFPRINTF(stderr, "Stack now");
    for (; yybottom <= yytop; yybottom++) {
        int yybot = *yybottom;
        YYFPRINTF(stderr, " %d", yybot);
    }
    YYFPRINTF(stderr, "\n");
}

#define YY_STACK_PRINT(Bottom, Top)                                            \
    do {                                                                       \
        if (yydebug)                                                           \
            yy_stack_print((Bottom), (Top));                                   \
    } while (YYID(0))

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static void yy_reduce_print(YYSTYPE *yyvsp, int yyrule)
#else
static void yy_reduce_print(yyvsp, yyrule) YYSTYPE *yyvsp;
int yyrule;
#endif
{
    int yynrhs = yyr2[yyrule];
    int yyi;
    unsigned long int yylno = yyrline[yyrule];
    YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n", yyrule - 1,
              yylno);
    /* The symbols being reduced.  */
    for (yyi = 0; yyi < yynrhs; yyi++) {
        YYFPRINTF(stderr, "   $%d = ", yyi + 1);
        yy_symbol_print(stderr, yyrhs[yyprhs[yyrule] + yyi],
                        &(yyvsp[(yyi + 1) - (yynrhs)]));
        YYFPRINTF(stderr, "\n");
    }
}

#define YY_REDUCE_PRINT(Rule)                                                  \
    do {                                                                       \
        if (yydebug)                                                           \
            yy_reduce_print(yyvsp, Rule);                                      \
    } while (YYID(0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#define YYDPRINTF(Args)
#define YY_SYMBOL_PRINT(Title, Type, Value, Location)
#define YY_STACK_PRINT(Bottom, Top)
#define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#if YYERROR_VERBOSE

#ifndef yystrlen
#if defined __GLIBC__ && defined _STRING_H
#define yystrlen strlen
#else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static YYSIZE_T yystrlen(const char *yystr)
#else
static YYSIZE_T yystrlen(yystr) const char *yystr;
#endif
{
    YYSIZE_T yylen;
    for (yylen = 0; yystr[yylen]; yylen++)
        continue;
    return yylen;
}
#endif
#endif

#ifndef yystpcpy
#if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#define yystpcpy stpcpy
#else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static char *yystpcpy(char *yydest, const char *yysrc)
#else
static char *yystpcpy(yydest, yysrc)
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
#endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T yytnamerr(char *yyres, const char *yystr) {
    if (*yystr == '"') {
        YYSIZE_T yyn = 0;
        char const *yyp = yystr;

        for (;;)
            switch (*++yyp) {
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
    do_not_strip_quotes:;
    }

    if (!yyres)
        return yystrlen(yystr);

    return yystpcpy(yyres, yystr) - yyres;
}
#endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int yysyntax_error(YYSIZE_T *yymsg_alloc, char **yymsg,
                          yytype_int16 *yyssp, int yytoken) {
    YYSIZE_T yysize0 = yytnamerr(YY_NULL, yytname[yytoken]);
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
    if (yytoken != YYEMPTY) {
        int yyn = yypact[*yyssp];
        yyarg[yycount++] = yytname[yytoken];
        if (!yypact_value_is_default(yyn)) {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            /* Stay within bounds of both yycheck and yytname.  */
            int yychecklim = YYLAST - yyn + 1;
            int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
            int yyx;

            for (yyx = yyxbegin; yyx < yyxend; ++yyx)
                if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR &&
                    !yytable_value_is_error(yytable[yyx + yyn])) {
                    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM) {
                        yycount = 1;
                        yysize = yysize0;
                        break;
                    }
                    yyarg[yycount++] = yytname[yyx];
                    {
                        YYSIZE_T yysize1 =
                            yysize + yytnamerr(YY_NULL, yytname[yyx]);
                        if (!(yysize <= yysize1 &&
                              yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                            return 2;
                        yysize = yysize1;
                    }
                }
        }
    }

    switch (yycount) {
#define YYCASE_(N, S)                                                          \
    case N:                                                                    \
        yyformat = S;                                                          \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4,
                YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(
            5,
            YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

    {
        YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
        if (!(yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
            return 2;
        yysize = yysize1;
    }

    if (*yymsg_alloc < yysize) {
        *yymsg_alloc = 2 * yysize;
        if (!(yysize <= *yymsg_alloc && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
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
            if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount) {
                yyp += yytnamerr(yyp, yyarg[yyi++]);
                yyformat += 2;
            } else {
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
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
static void yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void yydestruct(yymsg, yytype, yyvaluep) const char *yymsg;
int yytype;
YYSTYPE *yyvaluep;
#endif
{
    YYUSE(yyvaluep);

    if (!yymsg)
        yymsg = "Deleting";
    YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype) {
    case 51: /* ident_lista */

    {
        printf("Dropping ident_lista:  ");
        vec_debug_verbose(&((*yyvaluep).idents));
        vec_drop(&((*yyvaluep).idents));
    };

        break;
    case 63: /* parametros_lista */

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
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;

/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
int yyparse(void *YYPARSE_PARAM)
#else
int yyparse(YYPARSE_PARAM) void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
int yyparse(void)
#else
int yyparse()

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

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N))

    /* The number of symbols on the RHS of the reduced rule.
       Keep to zero when no symbol should be popped.  */
    int yylen = 0;

    yyssp = yyss = yyssa;
    yyvsp = yyvs = yyvsa;
    yystacksize = YYINITDEPTH;

    YYDPRINTF((stderr, "Starting parse\n"));

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

    if (yyss + yystacksize - 1 <= yyssp) {
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
            yyoverflow(YY_("memory exhausted"), &yyss1, yysize * sizeof(*yyssp),
                       &yyvs1, yysize * sizeof(*yyvsp), &yystacksize);

            yyss = yyss1;
            yyvs = yyvs1;
        }
#else /* no yyoverflow */
#ifndef YYSTACK_RELOCATE
        goto yyexhaustedlab;
#else
        /* Extend the stack our own way.  */
        if (YYMAXDEPTH <= yystacksize)
            goto yyexhaustedlab;
        yystacksize *= 2;
        if (YYMAXDEPTH < yystacksize)
            yystacksize = YYMAXDEPTH;

        {
            yytype_int16 *yyss1 = yyss;
            union yyalloc *yyptr =
                (union yyalloc *)YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
            if (!yyptr)
                goto yyexhaustedlab;
            YYSTACK_RELOCATE(yyss_alloc, yyss);
            YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#undef YYSTACK_RELOCATE
            if (yyss1 != yyssa)
                YYSTACK_FREE(yyss1);
        }
#endif
#endif /* no yyoverflow */

        yyssp = yyss + yysize - 1;
        yyvsp = yyvs + yysize - 1;

        YYDPRINTF((stderr, "Stack size increased to %lu\n",
                   (unsigned long int)yystacksize));

        if (yyss + yystacksize - 1 <= yyssp)
            YYABORT;
    }

    YYDPRINTF((stderr, "Entering state %d\n", yystate));

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
    if (yypact_value_is_default(yyn))
        goto yydefault;

    /* Not known => get a lookahead token if don't already have one.  */

    /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
    if (yychar == YYEMPTY) {
        YYDPRINTF((stderr, "Reading a token: "));
        yychar = YYLEX;
    }

    if (yychar <= YYEOF) {
        yychar = yytoken = YYEOF;
        YYDPRINTF((stderr, "Now at end of input.\n"));
    } else {
        yytoken = YYTRANSLATE(yychar);
        YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
    }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
        goto yydefault;
    yyn = yytable[yyn];
    if (yyn <= 0) {
        if (yytable_value_is_error(yyn))
            goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
    }

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus)
        yyerrstatus--;

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

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
    yyval = yyvsp[1 - yylen];

    YY_REDUCE_PRINT(yyn);
    switch (yyn) {
    case 2:

    {
        puts("Init symbol table");
        hashset_init(&tabla, sizeof(Symbol), hash_symbol);
        puts("Init warning buffer");
        str_init(&wrn_buff);
    } break;

    case 3:

    {
        printf("Programa: %.*s\n", (yyvsp[(3) - (11)].slice).len,
               (yyvsp[(3) - (11)].slice).ptr);
        printf("Entradas: %zu\n", (yyvsp[(5) - (11)].idents).len);
        for (size_t i = 0; i < (yyvsp[(5) - (11)].idents).len; i++) {
            StrSlice *str = (StrSlice *)vec_get(&(yyvsp[(5) - (11)].idents), i);
            printf("    - %.*s\n", (int)str->len, str->ptr);
        }
    } break;

    case 4:

    {
        (yyval.idents) = (yyvsp[(3) - (3)].idents);
        // vec_debug_verbose(&$$);
        StrSlice *sl = vec_push(&(yyval.idents));
        *sl = (yyvsp[(1) - (3)].slice);
    } break;

    case 5:

    {
        (yyval.idents) = vec_new(sizeof(StrSlice));
        // vec_debug_verbose(&$$);
        StrSlice *sl = vec_push(&(yyval.idents));
        *sl = (yyvsp[(1) - (1)].slice);
    } break;

    case 9:

    {
        printf("Declarando variables: %zu\n", (yyvsp[(3) - (6)].idents).len);
        for (size_t i = 0; i < (yyvsp[(3) - (6)].idents).len; i++) {
            Symbol s = (Symbol){
                .name = *(StrSlice *)vec_get(&(yyvsp[(3) - (6)].idents), i),
                .scope = 0,
                .line = linea};
            assert_not_sym_exists(&s);
            hashset_insert(&tabla, &s);
            printf("    - %zu: %.*s\n", linea, (int)s.name.len, s.name.ptr);
        }
    } break;

    case 10:

    {
        printf("Constante: %lld\n", (yyvsp[(5) - (6)].snum));
    } break;

    case 11:

    {
        printf("Constante: %f\n", (yyvsp[(5) - (6)].fnum));
    } break;

    case 12:

    {
        printf("Constante: %.*s\n", (yyvsp[(5) - (6)].slice).len,
               (yyvsp[(5) - (6)].slice).ptr);
    } break;

    case 22:

    {
        printf("Declarando funcion %.*s\n", (int)(yyvsp[(2) - (2)].slice).len,
               (yyvsp[(2) - (2)].slice).ptr);
    } break;

    case 24:

    {
        printf("Declarando procedure %.*s\n", (int)(yyvsp[(2) - (2)].slice).len,
               (yyvsp[(2) - (2)].slice).ptr);
    } break;

    case 25:

    {
        printf("Declarada %.*s\n", (int)(yyvsp[(2) - (5)].slice).len,
               (yyvsp[(2) - (5)].slice).ptr);
    } break;

    case 26:

    {
        printf("Argumentos: %zu\n", (yyvsp[(2) - (3)].idents).len);
        for (size_t i = 0; i < (yyvsp[(2) - (3)].idents).len; i++) {
            StrSlice *str = (StrSlice *)vec_get(&(yyvsp[(2) - (3)].idents), i);
            printf("    - %.*s\n", (int)str->len, str->ptr);
        }
    } break;

    case 28:

    {
        (yyval.idents) = (yyvsp[(1) - (3)].idents);
    } break;

    case 29:

    {
        (yyval.idents) = (yyvsp[(1) - (5)].idents);
        vec_extend(&(yyval.idents), &(yyvsp[(3) - (5)].idents));
        vec_drop(&(yyvsp[(3) - (5)].idents));
    } break;

    case 45:

    {
        Symbol s =
            (Symbol){.name = (yyvsp[(3) - (4)].slice), .scope = 0, .line = 0};
        assert_sym_exists(&s);
    } break;

    case 46:

    {
        Symbol s =
            (Symbol){.name = (yyvsp[(3) - (4)].slice), .scope = 0, .line = 0};
        assert_sym_exists(&s);
    } break;

    case 48:

    {
        Symbol s =
            (Symbol){.name = (yyvsp[(5) - (6)].slice), .scope = 0, .line = 0};
        assert_sym_exists(&s);
    } break;

    case 54:

    {
        Symbol s =
            (Symbol){.name = (yyvsp[(3) - (6)].slice), .scope = 0, .line = 0};
        Symbol s1 =
            (Symbol){.name = (yyvsp[(5) - (6)].slice), .scope = 0, .line = 0};
        assert_sym_exists(&s);
        assert_sym_exists(&s1);
    } break;

    case 55:

    {
        Symbol s = (Symbol){.name = (yyvsp[(3) - (4)].slice), .line = 0};
        assert_sym_exists(&s);
    } break;

    case 56:

    {
        Symbol s = (Symbol){.name = (yyvsp[(3) - (4)].slice), .line = 0};
        assert_sym_exists(&s);
    } break;

    case 57:

    {
        Symbol s = (Symbol){.name = (yyvsp[(3) - (6)].slice), .line = 0};
        assert_sym_exists(&s);
    } break;

    case 58:

    {
        Symbol s = (Symbol){.name = (yyvsp[(3) - (6)].slice), .line = 0};
        assert_sym_exists(&s);
    } break;

    case 65:

    {
        Symbol s = (Symbol){.name = (yyvsp[(1) - (4)].slice), .line = 0};
        assert_sym_exists(&s);
    } break;

    default:
        break;
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
    YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);

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
    yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus) {
        ++yynerrs;
#if !YYERROR_VERBOSE
        yyerror(YY_("syntax error"));
#else
#define YYSYNTAX_ERROR yysyntax_error(&yymsg_alloc, &yymsg, yyssp, yytoken)
        {
            char const *yymsgp = YY_("syntax error");
            int yysyntax_error_status;
            yysyntax_error_status = YYSYNTAX_ERROR;
            if (yysyntax_error_status == 0)
                yymsgp = yymsg;
            else if (yysyntax_error_status == 1) {
                if (yymsg != yymsgbuf)
                    YYSTACK_FREE(yymsg);
                yymsg = (char *)YYSTACK_ALLOC(yymsg_alloc);
                if (!yymsg) {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                    yysyntax_error_status = 2;
                } else {
                    yysyntax_error_status = YYSYNTAX_ERROR;
                    yymsgp = yymsg;
                }
            }
            yyerror(yymsgp);
            if (yysyntax_error_status == 2)
                goto yyexhaustedlab;
        }
#undef YYSYNTAX_ERROR
#endif
    }

    if (yyerrstatus == 3) {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        if (yychar <= YYEOF) {
            /* Return failure if at end of input.  */
            if (yychar == YYEOF)
                YYABORT;
        } else {
            yydestruct("Error: discarding", yytoken, &yylval);
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
    YYPOPSTACK(yylen);
    yylen = 0;
    YY_STACK_PRINT(yyss, yyssp);
    yystate = *yyssp;
    goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
    yyerrstatus = 3; /* Each real token shifted decrements this.  */

    for (;;) {
        yyn = yypact[yystate];
        if (!yypact_value_is_default(yyn)) {
            yyn += YYTERROR;
            if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR) {
                yyn = yytable[yyn];
                if (0 < yyn)
                    break;
            }
        }

        /* Pop the current state because it cannot handle the error token.  */
        if (yyssp == yyss)
            YYABORT;

        yydestruct("Error: popping", yystos[yystate], yyvsp);
        YYPOPSTACK(1);
        yystate = *yyssp;
        YY_STACK_PRINT(yyss, yyssp);
    }

    YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    *++yyvsp = yylval;
    YY_IGNORE_MAYBE_UNINITIALIZED_END

    /* Shift the error token.  */
    YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

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
    yyerror(YY_("memory exhausted"));
    yyresult = 2;
    /* Fall through.  */
#endif

yyreturn:
    if (yychar != YYEMPTY) {
        /* Make sure we have latest lookahead translation.  See comments at
           user semantic actions for why this is necessary.  */
        yytoken = YYTRANSLATE(yychar);
        yydestruct("Cleanup: discarding lookahead", yytoken, &yylval);
    }
    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    YYPOPSTACK(yylen);
    YY_STACK_PRINT(yyss, yyssp);
    while (yyssp != yyss) {
        yydestruct("Cleanup: popping", yystos[*yyssp], yyvsp);
        YYPOPSTACK(1);
    }
#ifndef yyoverflow
    if (yyss != yyssa)
        YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
    if (yymsg != yymsgbuf)
        YYSTACK_FREE(yymsg);
#endif
    /* Make sure YYID is used.  */
    return YYID(yyresult);
}
