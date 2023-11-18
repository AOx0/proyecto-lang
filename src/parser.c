
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* "%code top" blocks.  */

#include "parser.h"
#include "hashset.h"
#include "str.h"
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

void add_reference_to_sym(Symbol *s, size_t line) {
    size_t *ref = vec_push(&s->refs);
    *ref = line;
}

void *assert_sym_exists(Symbol *s) {
    tnchar = nchar;
    size_t orig_scope = s->scope;
    int found = 0;
    nchar = s->nchar;
    Symbol *res = NULL;

    for (size_t i = orig_scope; i >= 0; i -= orig_scope) {
        s->scope = i;
        // printf("Looking %.*s in scope %zu\n", (int)s->name.len, s->name.ptr ,
        // i);
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
        // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope,
        // (int)s->name.len, s->name.ptr);
    }
    s->scope = orig_scope;
    nchar = tnchar;
    return res;
}

void assert_not_sym_exists(Symbol *s) {
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

/* Copy the first part of user declarations.  */

/* Enabling traces.  */
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
#undef YYERROR_VERBOSE
#define YYERROR_VERBOSE 1
#else
#define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
#define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

#ifndef LNG_PARSERH
#define LNG_PARSERH

#include "hashset.h"
#include "vector.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"
#include "str.h"

extern FILE *yyin, *yyout;

enum RelOp { And, Or };
typedef enum RelOp RelOp;

enum AddOp { Add, Sub };
typedef enum AddOp AddOp;

enum MulOp { Div, Mod, Mul };
typedef enum MulOp MulOp;

#endif

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

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

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

} YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define yystype YYSTYPE /* obsolescent; will be withdrawn */
#define YYSTYPE_IS_DECLARED 1
#endif

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
#if YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(msgid) dgettext("bison-runtime", msgid)
#endif
#endif
#ifndef YY_
#define YY_(msgid) msgid
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YYUSE(e) ((void)(e))
#else
#define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
#define YYID(n) (n)
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
#if !defined _ALLOCA_H && !defined _STDLIB_H &&                                \
    (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef _STDLIB_H
#define _STDLIB_H 1
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
#if (defined __cplusplus && !defined _STDLIB_H &&                              \
     !((defined YYMALLOC || defined malloc) &&                                 \
       (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef _STDLIB_H
#define _STDLIB_H 1
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined _STDLIB_H &&                                   \
    (defined __STDC__ || defined __C99__FUNC__ || defined __cplusplus ||       \
     defined _MSC_VER)
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined _STDLIB_H &&                                     \
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(To, From, Count)                                                \
    __builtin_memcpy(To, From, (Count) * sizeof(*(From)))
#else
#define YYCOPY(To, From, Count)                                                \
    do {                                                                       \
        YYSIZE_T yyi;                                                          \
        for (yyi = 0; yyi < (Count); yyi++)                                    \
            (To)[yyi] = (From)[yyi];                                           \
    } while (YYID(0))
#endif
#endif

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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 230

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 39
/* YYNRULES -- Number of rules.  */
#define YYNRULES 98
/* YYNRULES -- Number of states.  */
#define YYNSTATES 211

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 299

#define YYTRANSLATE(YYX)                                                       \
    ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  45, 46, 2,  2,  49, 2,  48, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  50, 47, 2,  51, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  52, 2,  53, 2,
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
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,   0,   3,   4,   16,  20,  22,  24,  26,  27,  34,  41,  48,  55,  57,
    67,  69,  71,  73,  75,  79,  80,  85,  86,  94,  95,  101, 105, 106, 110,
    116, 120, 122, 123, 125, 129, 131, 133, 135, 137, 139, 141, 143, 148, 155,
    162, 167, 172, 179, 186, 191, 196, 203, 210, 217, 224, 229, 234, 241, 248,
    253, 260, 264, 266, 268, 270, 275, 277, 282, 286, 288, 292, 294, 297, 299,
    303, 305, 309, 311, 313, 315, 317, 319, 321, 323, 325, 327, 331, 333, 337,
    339, 343, 348, 350, 355, 357, 359, 361, 364};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] = {
    55, 0,  -1, -1, 56, 12, 3,  45, 57, 46, 47, 58, 63, 70, 48, -1, 3,  49, 57,
    -1, 3,  -1, 59, -1, 60, -1, -1, 58, 14, 57, 50, 61, 47, -1, 58, 13, 3,  51,
    5,  47, -1, 58, 13, 3,  51, 4,  47, -1, 58, 13, 3,  51, 6,  47, -1, 62, -1,
    15, 52, 5,  48, 48, 5,  53, 16, 62, -1, 41, -1, 42, -1, 43, -1, 44, -1, 63,
    64, 47, -1, -1, 65, 58, 63, 70, -1, -1, 17, 3,  66, 68, 50, 62, 47, -1, -1,
    11, 3,  67, 68, 47, -1, 45, 69, 46, -1, -1, 57, 50, 61, -1, 69, 47, 57, 50,
    61, -1, 18, 71, 19, -1, 72, -1, -1, 73, -1, 72, 47, 73, -1, 78, -1, 81, -1,
    70, -1, 77, -1, 74, -1, 75, -1, 76, -1, 24, 82, 26, 73, -1, 25, 79, 27, 89,
    26, 73, -1, 25, 79, 28, 89, 26, 73, -1, 20, 45, 3,  46, -1, 21, 45, 3,  46,
    -1, 22, 45, 6,  49, 3,  46, -1, 23, 45, 6,  49, 3,  46, -1, 22, 45, 6,  46,
    -1, 23, 45, 6,  46, -1, 22, 45, 6,  49, 89, 46, -1, 23, 45, 6,  49, 89, 46,
    -1, 22, 45, 3,  49, 3,  46, -1, 23, 45, 3,  49, 3,  46, -1, 22, 45, 3,  46,
    -1, 23, 45, 3,  46, -1, 22, 45, 3,  49, 89, 46, -1, 23, 45, 3,  49, 89, 46,
    -1, 29, 82, 30, 73, -1, 29, 82, 30, 73, 31, 73, -1, 80, 10, 89, -1, 78, -1,
    80, -1, 3,  -1, 3,  52, 5,  53, -1, 3,  -1, 3,  45, 88, 46, -1, 82, 40, 83,
    -1, 83, -1, 83, 38, 84, -1, 84, -1, 39, 84, -1, 85, -1, 45, 82, 46, -1, 86,
    -1, 89, 87, 89, -1, 38, -1, 40, -1, 34, -1, 35, -1, 36, -1, 37, -1, 32, -1,
    33, -1, 89, -1, 88, 49, 89, -1, 90, -1, 89, 8,  90, -1, 92, -1, 90, 9,  92,
    -1, 3,  45, 88, 46, -1, 3,  -1, 3,  52, 89, 53, -1, 91, -1, 5,  -1, 4,  -1,
    8,  92, -1, 45, 89, 46, -1};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   174, 174, 174, 196, 202, 210, 210, 210, 212, 225, 232, 239, 248, 248,
    249, 250, 251, 252, 255, 255, 256, 259, 259, 272, 272, 285, 296, 297, 300,
    308, 309, 309, 310, 310, 311, 311, 311, 312, 312, 312, 312, 314, 315, 316,
    318, 322, 326, 330, 334, 334, 335, 335, 336, 342, 348, 352, 356, 360, 364,
    365, 368, 369, 369, 370, 374, 378, 383, 390, 390, 391, 391, 392, 392, 393,
    393, 394, 395, 395, 395, 395, 395, 395, 395, 395, 398, 398, 399, 399, 400,
    400, 401, 406, 410, 414, 414, 414, 414, 414};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
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
                                      "RELOP_EQ",
                                      "RELOP_NEQ",
                                      "RELOP_BT",
                                      "RELOP_LT",
                                      "RELOP_EBT",
                                      "RELOP_ELT",
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
                                      0};
#endif

#ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] = {
    0,   256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268,
    269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282,
    283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296,
    297, 298, 299, 40,  41,  59,  46,  44,  58,  61,  91,  93};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  54, 56, 55, 57, 57, 58, 58, 58, 59, 60, 60, 60, 61, 61, 62, 62,
    62, 62, 63, 63, 64, 66, 65, 67, 65, 68, 68, 69, 69, 70, 71, 71, 72,
    72, 73, 73, 73, 73, 73, 73, 73, 74, 74, 74, 75, 75, 76, 76, 76, 76,
    76, 76, 76, 76, 76, 76, 76, 76, 77, 77, 78, 79, 79, 80, 80, 81, 81,
    82, 82, 83, 83, 84, 84, 85, 85, 86, 87, 87, 87, 87, 87, 87, 87, 87,
    88, 88, 89, 89, 90, 90, 91, 92, 92, 92, 92, 92, 92, 92};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 0, 11, 3, 1, 1, 1, 0, 6, 6, 6, 6, 1, 9, 1, 1, 1, 1, 3, 0, 4, 0, 7, 0,
    5, 3, 0, 3,  5, 3, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 4, 6, 6, 4, 4, 6, 6, 4,
    4, 6, 6, 6,  6, 4, 4, 6, 6, 4, 6, 3, 1, 1, 1, 4, 1, 4, 3, 1, 3, 1, 2, 1, 3,
    1, 3, 1, 1,  1, 1, 1, 1, 1, 1, 1, 3, 1, 3, 1, 3, 4, 1, 4, 1, 1, 1, 2, 3};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    2,  0,  0,  1,  0,  0,  0,  5,  0,  0,  0,  4,  8,  20, 6,  7,  0,  0,
    0,  0,  0,  0,  0,  32, 0,  8,  0,  0,  0,  24, 22, 66, 0,  0,  0,  0,
    0,  0,  0,  37, 0,  31, 33, 39, 40, 41, 38, 35, 0,  36, 19, 20, 3,  0,
    0,  0,  0,  15, 16, 17, 18, 0,  13, 27, 27, 0,  0,  0,  0,  0,  0,  92,
    96, 95, 0,  0,  0,  0,  69, 71, 73, 75, 0,  87, 94, 89, 64, 62, 0,  63,
    0,  30, 0,  0,  0,  11, 10, 12, 0,  9,  0,  0,  0,  0,  0,  85, 0,  0,
    0,  0,  0,  0,  0,  0,  0,  97, 72, 0,  0,  0,  0,  0,  0,  83, 84, 79,
    80, 81, 82, 77, 78, 0,  0,  0,  0,  0,  34, 61, 21, 0,  0,  0,  25, 0,
    0,  67, 0,  65, 45, 46, 55, 0,  49, 0,  56, 0,  50, 0,  0,  0,  74, 98,
    42, 68, 70, 88, 76, 90, 0,  0,  59, 0,  0,  26, 0,  0,  86, 92, 0,  92,
    0,  92, 0,  92, 0,  91, 93, 0,  0,  0,  0,  28, 0,  23, 53, 57, 47, 51,
    54, 58, 48, 52, 43, 44, 60, 0,  0,  0,  29, 0,  14};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1, 1,   2,   8,  13, 14, 15, 61,  62,  18, 24, 25, 64,
    63, 101, 141, 39, 40, 41, 42, 43,  44,  45, 46, 47, 88,
    48, 49,  77,  78, 79, 80, 81, 131, 104, 82, 83, 84, 85};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -147
static const yytype_int16 yypact[] = {
    -147, 37,   35,   -147, 62,   42,   79,   85,   96,   79,   124,  -147,
    -147, 165,  -147, -147, 150,  79,   122,  131,  118,  172,  180,  125,
    137,  -147, 138,  109,  94,   -147, -147, 4,    140,  142,  143,  144,
    7,    187,  7,    -147, 173,  146,  -147, -147, -147, -147, -147, -147,
    181,  -147, -147, 165,  -147, 147,  148,  149,  145,  -147, -147, -147,
    -147, 151,  -147, 154,  154,  13,   195,  198,  199,  36,   63,   29,
    -147, -147, 13,   7,    7,    8,    166,  -147, -147, -147, 87,   194,
    -147, -147, 153,  -147, 73,   181,  5,    -147, 125,  13,   122,  -147,
    -147, -147, 201,  -147, 79,   160,  158,  13,   95,   202,  156,  167,
    168,  115,  121,  123,  127,  13,   13,   -147, -147, 117,  70,   125,
    7,    7,    13,   -147, -147, -147, -147, -147, -147, -147, -147, 13,
    13,   13,   13,   125,  -147, 202,  -147, 163,  162,  134,  -147, 88,
    -3,   -147, 13,   -147, -147, -147, -147, 19,   -147, 28,   -147, 72,
    -147, 81,   128,  1,    -147, -147, -147, 166,  -147, 194,  202,  -147,
    12,   33,   184,  169,  94,   -147, 79,   171,  202,  66,   11,   106,
    17,   110,  21,   114,  47,   -147, -147, 125,  125,  125,  211,  -147,
    170,  -147, -147, -147, -147, -147, -147, -147, -147, -147, -147, -147,
    -147, 174,  94,   203,  -147, 88,   -147};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -147, -147, -147, -9,  196,  -147, -147, -146, -137, 175,  -147, -147, -147,
    -147, 159,  -147, -15, -147, -147, -91,  -147, -147, -147, -147, 185,  -147,
    188,  -147, 23,   104, -68,  -147, -147, -147, 116,  -63,  108,  -147, -70};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int16 yytable[] = {
    11,  136, 105, 26,  115, 122, 175, 116, 20,  122, 71,  72,  73,  118, -64,
    74,  71,  72,  73,  122, 122, 74,  177, 72,  73,  122, 191, 74,  162, 122,
    137, 179, 72,  73,  119, 135, 74,  3,   187, 109, 144, 122, 110, 161, 170,
    120, 75,  4,   120, 65,  105, 159, 76,  164, 186, 122, 66,  195, 103, 188,
    208, 90,  167, 197, 103, 5,   111, 199, 166, 112, 168, 169, 210, 103, 113,
    181, 72,  73,  122, 138, 74,  114, 7,   176, 183, 72,  73,  6,   178, 74,
    180, 140, 182, 201, 184, 122, 202, 203, 204, 117, 133, 134, 123, 124, 125,
    126, 127, 128, 129, 56,  130, 113, 194, 53,  54,  55,  161, 103, 114, 123,
    124, 125, 126, 127, 128, 129, 103, 130, 31,  57,  58,  59,  60,  21,  9,
    57,  58,  59,  60,  22,  23,  145, 10,  23,  146, 32,  33,  34,  35,  36,
    37,  113, 196, 19,  38,  113, 198, 120, 114, 113, 200, 150, 114, 160, 151,
    192, 114, 152, 28,  154, 153, 12,  155, 156, 185, 29,  157, 146, 16,  17,
    173, 174, 27,  30,  50,  67,  52,  68,  69,  70,  86,  93,  91,  92,  95,
    96,  97,  98,  99,  100, 106, 107, 108, 132, 121, 66,  139, 142, 143, 147,
    122, 171, 172, 148, 149, 189, 205, 190, 193, 209, 206, 51,  87,  102, 163,
    89,  94,  207, 0,   158, 165};

static const yytype_int16 yycheck[] = {
    9,   92,  65,  18,  74,  8,   143, 75,  17,  8,  3,   4,   5,   76, 10,  8,
    3,   4,   5,   8,   8,   8,   3,   4,   5,   8,  172, 8,   119, 8,  93,  3,
    4,   5,   26,  30,  8,   0,   26,  3,   103, 8,  6,   46,  135, 40, 39,  12,
    40,  45,  113, 114, 45,  121, 53,  8,   52,  46, 45,  26,  206, 38, 132, 46,
    45,  3,   3,   46,  131, 6,   133, 134, 209, 45, 45,  3,   4,   5,  8,   94,
    8,   52,  3,   146, 3,   4,   5,   45,  151, 8,  153, 100, 155, 46, 157, 8,
    187, 188, 189, 76,  27,  28,  32,  33,  34,  35, 36,  37,  38,  15, 40,  45,
    46,  4,   5,   6,   46,  45,  52,  32,  33,  34, 35,  36,  37,  38, 45,  40,
    3,   41,  42,  43,  44,  11,  49,  41,  42,  43, 44,  17,  18,  46, 46,  18,
    49,  20,  21,  22,  23,  24,  25,  45,  46,  3,  29,  45,  46,  40, 52,  45,
    46,  46,  52,  46,  49,  174, 52,  46,  50,  46, 49,  47,  49,  46, 46,  3,
    49,  49,  13,  14,  46,  47,  51,  3,   47,  45, 48,  45,  45,  45, 3,   10,
    19,  47,  47,  47,  47,  52,  47,  45,  5,   3,  3,   9,   38,  52, 5,   47,
    50,  53,  8,   48,  50,  46,  46,  31,  5,   48, 47,  16,  50,  25, 37,  64,
    120, 37,  51,  53,  -1,  113, 122};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  55, 56, 0,  12, 3,  45, 3,  57, 49, 46, 57, 47, 58, 59, 60, 13, 14,
    63, 3,  57, 11, 17, 18, 64, 65, 70, 51, 50, 3,  3,  3,  20, 21, 22, 23,
    24, 25, 29, 70, 71, 72, 73, 74, 75, 76, 77, 78, 80, 81, 47, 58, 48, 4,
    5,  6,  15, 41, 42, 43, 44, 61, 62, 67, 66, 45, 52, 45, 45, 45, 45, 3,
    4,  5,  8,  39, 45, 82, 83, 84, 85, 86, 89, 90, 91, 92, 3,  78, 79, 80,
    82, 19, 47, 10, 63, 47, 47, 47, 52, 47, 45, 68, 68, 45, 88, 89, 5,  3,
    3,  3,  6,  3,  6,  45, 52, 92, 84, 82, 89, 26, 40, 38, 8,  32, 33, 34,
    35, 36, 37, 38, 40, 87, 9,  27, 28, 30, 73, 89, 70, 5,  57, 69, 47, 50,
    89, 46, 49, 53, 46, 46, 46, 49, 46, 49, 46, 49, 46, 49, 88, 89, 46, 46,
    73, 83, 84, 90, 89, 92, 89, 89, 73, 48, 50, 46, 47, 62, 89, 3,  89, 3,
    89, 3,  89, 3,  89, 46, 53, 26, 26, 31, 48, 61, 57, 47, 46, 46, 46, 46,
    46, 46, 46, 46, 73, 73, 73, 5,  50, 53, 61, 16, 62};

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY (-2)
#define YYEOF 0

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL goto yyerrlab

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                                 \
    do                                                                         \
        if (yychar == YYEMPTY && yylen == 1) {                                 \
            yychar = (Token);                                                  \
            yylval = (Value);                                                  \
            yytoken = YYTRANSLATE(yychar);                                     \
            YYPOPSTACK(1);                                                     \
            goto yybackup;                                                     \
        } else {                                                               \
            yyerror(YY_("syntax error: cannot back up"));                      \
            YYERROR;                                                           \
        }                                                                      \
    while (YYID(0))

#define YYTERROR 1
#define YYERRCODE 256

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                                        \
    do                                                                         \
        if (YYID(N)) {                                                         \
            (Current).first_line = YYRHSLOC(Rhs, 1).first_line;                \
            (Current).first_column = YYRHSLOC(Rhs, 1).first_column;            \
            (Current).last_line = YYRHSLOC(Rhs, N).last_line;                  \
            (Current).last_column = YYRHSLOC(Rhs, N).last_column;              \
        } else {                                                               \
            (Current).first_line = (Current).last_line =                       \
                YYRHSLOC(Rhs, 0).last_line;                                    \
            (Current).first_column = (Current).last_column =                   \
                YYRHSLOC(Rhs, 0).last_column;                                  \
        }                                                                      \
    while (YYID(0))
#endif

/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
#if YYLTYPE_IS_TRIVIAL
#define YY_LOCATION_PRINT(File, Loc)                                           \
    fprintf(File, "%d.%d-%d.%d", (Loc).first_line, (Loc).first_column,         \
            (Loc).last_line, (Loc).last_column)
#else
#define YY_LOCATION_PRINT(File, Loc) ((void)0)
#endif
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
    if (!yyvaluep)
        return;
#ifdef YYPRINT
    if (yytype < YYNTOKENS)
        YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
#else
    YYUSE(yyoutput);
#endif
    switch (yytype) {
    default:
        break;
    }
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T yysyntax_error(char *yyresult, int yystate, int yychar) {
    int yyn = yypact[yystate];

    if (!(YYPACT_NINF < yyn && yyn <= YYLAST))
        return 0;
    else {
        int yytype = YYTRANSLATE(yychar);
        YYSIZE_T yysize0 = yytnamerr(0, yytname[yytype]);
        YYSIZE_T yysize = yysize0;
        YYSIZE_T yysize1;
        int yysize_overflow = 0;
        enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
        char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
        int yyx;

#if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
        char *yyfmt;
        char const *yyf;
        static char const yyunexpected[] = "syntax error, unexpected %s";
        static char const yyexpecting[] = ", expecting %s";
        static char const yyor[] = " or %s";
        char yyformat[sizeof yyunexpected + sizeof yyexpecting - 1 +
                      ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2) * (sizeof yyor - 1))];
        char const *yyprefix = yyexpecting;

        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  */
        int yyxbegin = yyn < 0 ? -yyn : 0;

        /* Stay within bounds of both yycheck and yytname.  */
        int yychecklim = YYLAST - yyn + 1;
        int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        int yycount = 1;

        yyarg[0] = yytname[yytype];
        yyfmt = yystpcpy(yyformat, yyunexpected);

        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR) {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM) {
                    yycount = 1;
                    yysize = yysize0;
                    yyformat[sizeof yyunexpected - 1] = '\0';
                    break;
                }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr(0, yytname[yyx]);
                yysize_overflow |= (yysize1 < yysize);
                yysize = yysize1;
                yyfmt = yystpcpy(yyfmt, yyprefix);
                yyprefix = yyor;
            }

        yyf = YY_(yyformat);
        yysize1 = yysize + yystrlen(yyf);
        yysize_overflow |= (yysize1 < yysize);
        yysize = yysize1;

        if (yysize_overflow)
            return YYSIZE_MAXIMUM;

        if (yyresult) {
            /* Avoid sprintf, as that infringes on the user's name space.
               Don't have undefined behavior even if the translation
               produced a string with the wrong number of "%s"s.  */
            char *yyp = yyresult;
            int yyi = 0;
            while ((*yyp = *yyf) != '\0') {
                if (*yyp == '%' && yyf[1] == 's' && yyi < yycount) {
                    yyp += yytnamerr(yyp, yyarg[yyi++]);
                    yyf += 2;
                } else {
                    yyp++;
                    yyf++;
                }
            }
        }
        return yysize;
    }
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
    case 57: /* "ident_lista" */

    {
        // printf("Dropping ident_lista:  ");
        // vec_debug_verbose(&$$);

        for (size_t i = 0; i < (yyvaluep->idents).len; i++) {
            Symbol *s = vec_get(&(yyvaluep->idents), i);
            vec_drop(&s->refs);
        }

        vec_drop(&(yyvaluep->idents));
    };

        break;
    case 69: /* "parametros_lista" */

    {
        // printf("Dropping parametros_lista:  ");
        // vec_debug_verbose(&$$);

        for (size_t i = 0; i < (yyvaluep->idents).len; i++) {
            Symbol *s = vec_get(&(yyvaluep->idents), i);
            vec_drop(&s->refs);
        }

        vec_drop(&(yyvaluep->idents));
    };

        break;

    default:
        break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
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

/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
    int yytoken;
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

    yytoken = 0;
    yyss = yyssa;
    yyvs = yyvsa;
    yystacksize = YYINITDEPTH;

    YYDPRINTF((stderr, "Starting parse\n"));

    yystate = 0;
    yyerrstatus = 0;
    yynerrs = 0;
    yychar = YYEMPTY; /* Cause a token to be read.  */

    /* Initialize stack pointers.
       Waste one element of value and location stack
       so that they stay on the same level as the state stack.
       The wasted elements are never initialized.  */
    yyssp = yyss;
    yyvsp = yyvs;

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
    if (yyn == YYPACT_NINF)
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
        if (yyn == 0 || yyn == YYTABLE_NINF)
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
    *++yyvsp = yylval;

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
        ;
    } break;

    case 3:

    {
        // printf("Programa: %.*s\n", (int)$3.name.len, $3.name.ptr);
        (yyvsp[(3) - (11)].symbol).type = Function;
        assert_not_sym_exists(&(yyvsp[(3) - (11)].symbol));
        hashset_insert(&tabla, &(yyvsp[(3) - (11)].symbol));
        // printf("Entradas: %zu\n", $5.len);
        for (size_t i = 0; i < (yyvsp[(5) - (11)].idents).len; i++) {
            Symbol *s = vec_get(&(yyvsp[(5) - (11)].idents), i);
            assert_not_sym_exists(s);
            s->type = Variable;
            hashset_insert(&tabla, s);
            printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
        }

        vec_drop(&(yyvsp[(5) - (11)].idents));
        ;
    } break;

    case 4:

    {
        (yyval.idents) = (yyvsp[(3) - (3)].idents);
        // vec_debug_verbose(&$$);
        Symbol *s = vec_push(&(yyval.idents));
        *s = (yyvsp[(1) - (3)].symbol);
        ;
    } break;

    case 5:

    {
        (yyval.idents) = vec_new(sizeof(Symbol));
        // vec_debug_verbose(&$$);
        Symbol *s = vec_push(&(yyval.idents));
        *s = (yyvsp[(1) - (1)].symbol);
        ;
    } break;

    case 9:

    {
        printf("Declarando variables: %zu\n", (yyvsp[(3) - (6)].idents).len);
        for (size_t i = 0; i < (yyvsp[(3) - (6)].idents).len; i++) {
            Symbol *s = vec_get(&(yyvsp[(3) - (6)].idents), i);
            s->type = Variable;
            assert_not_sym_exists(s);
            hashset_insert(&tabla, s);
            printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
        }

        vec_drop(&(yyvsp[(3) - (6)].idents));
        ;
    } break;

    case 10:

    {
        printf("Declarando constante: %lld\n", (yyvsp[(5) - (6)].snum));
        Symbol s = (yyvsp[(3) - (6)].symbol);
        s.type = Constant;
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 11:

    {
        printf("Declarando constante: %f\n", (yyvsp[(5) - (6)].fnum));
        Symbol s = (yyvsp[(3) - (6)].symbol);
        s.type = Constant;
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 12:

    {
        printf("Declarando constante: %.*s\n", (yyvsp[(5) - (6)].slice).len,
               (yyvsp[(5) - (6)].slice).ptr);
        Symbol s = (yyvsp[(3) - (6)].symbol);
        s.type = Constant;
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 15:

    {
        (yyval.type) = Int;
        ;
    } break;

    case 16:

    {
        (yyval.type) = Real;
        ;
    } break;

    case 17:

    {
        (yyval.type) = Str;
        ;
    } break;

    case 18:

    {
        (yyval.type) = Bool;
        ;
    } break;

    case 21:

    {
        scope -= fun_id;
        ;
    } break;

    case 22:

    {
        printf("Declarando funcion %.*s\n",
               (int)(yyvsp[(2) - (2)].symbol).name.len,
               (yyvsp[(2) - (2)].symbol).name.ptr);
        Symbol s = (yyvsp[(2) - (2)].symbol);
        s.type = Function;
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 23:

    {
        Symbol *s = (Symbol *)hashset_get(&tabla, &(yyvsp[(2) - (7)].symbol));
        s->info.fun = (FunctionInfo){.return_type = (yyvsp[(6) - (7)].type)};
        printf("Declarada %.*s\n", (int)(yyvsp[(2) - (7)].symbol).name.len,
               (yyvsp[(2) - (7)].symbol).name.ptr);
        ;
    } break;

    case 24:

    {
        printf("Declarando procedure %.*s\n",
               (int)(yyvsp[(2) - (2)].symbol).name.len,
               (yyvsp[(2) - (2)].symbol).name.ptr);
        Symbol s = (yyvsp[(2) - (2)].symbol);
        s.type = Procedure;
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 25:

    {
        printf("Declarada %.*s\n", (int)(yyvsp[(2) - (5)].symbol).name.len,
               (yyvsp[(2) - (5)].symbol).name.ptr);
        ;
    } break;

    case 26:

    {
        printf("Argumentos: %zu\n", (yyvsp[(2) - (3)].idents).len);
        for (size_t i = 0; i < (yyvsp[(2) - (3)].idents).len; i++) {
            Symbol *s = (Symbol *)vec_get(&(yyvsp[(2) - (3)].idents), i);
            s->type = Variable;
            assert_not_sym_exists(s);
            hashset_insert(&tabla, s);
            printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
        }

        vec_drop(&(yyvsp[(2) - (3)].idents));
        ;
    } break;

    case 28:

    {
        (yyval.idents) = (yyvsp[(1) - (3)].idents);
        ;
    } break;

    case 29:

    {
        (yyval.idents) = (yyvsp[(1) - (5)].idents);
        vec_extend(&(yyval.idents), &(yyvsp[(3) - (5)].idents));
        vec_drop(&(yyvsp[(3) - (5)].idents));
        ;
    } break;

    case 45:

    {
        Symbol s = (yyvsp[(3) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 46:

    {
        Symbol s = (yyvsp[(3) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 47:

    {
        Symbol s = (yyvsp[(5) - (6)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 48:

    {
        Symbol s = (yyvsp[(5) - (6)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 53:

    {
        Symbol s = (yyvsp[(3) - (6)].symbol);
        Symbol s1 = (yyvsp[(5) - (6)].symbol);
        assert_sym_exists(&s);
        assert_sym_exists(&s1);
        ;
    } break;

    case 54:

    {
        Symbol s = (yyvsp[(3) - (6)].symbol);
        Symbol s1 = (yyvsp[(5) - (6)].symbol);
        assert_sym_exists(&s);
        assert_sym_exists(&s1);
        ;
    } break;

    case 55:

    {
        Symbol s = (yyvsp[(3) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 56:

    {
        Symbol s = (yyvsp[(3) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 57:

    {
        Symbol s = (yyvsp[(3) - (6)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 58:

    {
        Symbol s = (yyvsp[(3) - (6)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 64:

    {
        Symbol s = (yyvsp[(1) - (1)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 65:

    {
        Symbol s = (yyvsp[(1) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 66:

    {
        Symbol s = (yyvsp[(1) - (1)].symbol);
        printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
        assert_sym_exists(&s);
        ;
    } break;

    case 67:

    {
        Symbol s = (yyvsp[(1) - (4)].symbol);
        printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
        assert_sym_exists(&s);
        ;
    } break;

    case 91:

    {
        Symbol s = (yyvsp[(1) - (4)].symbol);
        printf("Llamando funcion: %.*s\n", (int)s.name.len, s.name.ptr);
        assert_sym_exists(&s);
        ;
    } break;

    case 92:

    {
        Symbol s = (yyvsp[(1) - (1)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    case 93:

    {
        Symbol s = (yyvsp[(1) - (4)].symbol);
        assert_sym_exists(&s);
        ;
    } break;

    default:
        break;
    }
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
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus) {
        ++yynerrs;
#if !YYERROR_VERBOSE
        yyerror(YY_("syntax error"));
#else
        {
            YYSIZE_T yysize = yysyntax_error(0, yystate, yychar);
            if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM) {
                YYSIZE_T yyalloc = 2 * yysize;
                if (!(yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
                    yyalloc = YYSTACK_ALLOC_MAXIMUM;
                if (yymsg != yymsgbuf)
                    YYSTACK_FREE(yymsg);
                yymsg = (char *)YYSTACK_ALLOC(yyalloc);
                if (yymsg)
                    yymsg_alloc = yyalloc;
                else {
                    yymsg = yymsgbuf;
                    yymsg_alloc = sizeof yymsgbuf;
                }
            }

            if (0 < yysize && yysize <= yymsg_alloc) {
                (void)yysyntax_error(yymsg, yystate, yychar);
                yyerror(yymsg);
            } else {
                yyerror(YY_("syntax error"));
                if (yysize != 0)
                    goto yyexhaustedlab;
            }
        }
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
        if (yyn != YYPACT_NINF) {
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

    *++yyvsp = yylval;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
    yyerror(YY_("memory exhausted"));
    yyresult = 2;
    /* Fall through.  */
#endif

yyreturn:
    if (yychar != YYEMPTY)
        yydestruct("Cleanup: discarding lookahead", yytoken, &yylval);
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
