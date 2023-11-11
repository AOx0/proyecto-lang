
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

extern int yylex(void);
extern int main(void);
extern void yyerror(char *s);
extern size_t linea;
extern size_t scope;
size_t fun_id = 0;

#define SYM(s)                                                                 \
    (Symbol) {                                                                 \
        .name = s, .scope = scope, .line = linea,                              \
        .refs = vec_new(sizeof(size_t))                                        \
    }

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

    // printf("HASH: %zu FROM: %zu(%zu): %.*s\n", res.idx, sy->line, sy->scope,
    // (int)sy->name.len, sy->name.ptr);

    return res;
}

void assert_sym_exists(Symbol *s) {
    size_t orig_scope = s->scope;
    int found = 0;
    for (size_t i = orig_scope; i >= 0; i--) {
        s->scope = i;
        if (hashset_contains(&tabla, s)) {
            found = 1;
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
        // printf("Existe %zu(%zu):  %.*s\n", s->line, s->scope,
        // (int)s->name.len, s->name.ptr);
    }
    s->scope = orig_scope;
}

void assert_not_sym_exists(Symbol *s) {
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
#include "str.h"
#include "vector.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Symbol {
        StrSlice name;
        size_t scope;
        size_t line;
        Vec refs;
};
typedef struct Symbol Symbol;

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
    RELOP_BT = 287,
    RELOP_LT = 288,
    RELOP_EBT = 289,
    RELOP_ELT = 290,
    RELOP_AND = 291,
    RELOP_NOT = 292,
    RELOP_OR = 293,
    T_INT = 294,
    T_REAL = 295,
    T_STR = 296,
    T_BOOL = 297
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
#define YYLAST 238

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 39
/* YYNRULES -- Number of rules.  */
#define YYNRULES 96
/* YYNRULES -- Number of states.  */
#define YYNSTATES 209

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 297

#define YYTRANSLATE(YYX)                                                       \
    ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  43, 44, 2,  2,  47, 2,  46, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  48, 45, 2,  49, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  50, 2,  51, 2,
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
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,   0,   3,   4,   16,  20,  22,  24,  26,  27,  34,  41,  48,  55,
    57,  67,  69,  71,  73,  75,  79,  80,  85,  86,  94,  95,  101, 105,
    106, 110, 116, 120, 122, 123, 125, 129, 131, 133, 135, 137, 139, 141,
    143, 148, 155, 162, 167, 172, 179, 186, 191, 196, 203, 210, 217, 224,
    229, 234, 241, 248, 253, 260, 264, 266, 268, 270, 275, 277, 282, 286,
    288, 292, 294, 297, 299, 303, 305, 309, 311, 313, 315, 317, 319, 321,
    323, 327, 329, 333, 335, 339, 344, 346, 351, 353, 355, 357, 360};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] = {
    53, 0,  -1, -1, 54, 12, 3,  43, 55, 44, 45, 56, 61, 68, 46, -1, 3,  47, 55,
    -1, 3,  -1, 57, -1, 58, -1, -1, 56, 14, 55, 48, 59, 45, -1, 56, 13, 3,  49,
    5,  45, -1, 56, 13, 3,  49, 4,  45, -1, 56, 13, 3,  49, 6,  45, -1, 60, -1,
    15, 50, 5,  46, 46, 5,  51, 16, 60, -1, 39, -1, 40, -1, 41, -1, 42, -1, 61,
    62, 45, -1, -1, 63, 56, 61, 68, -1, -1, 17, 3,  64, 66, 48, 60, 45, -1, -1,
    11, 3,  65, 66, 45, -1, 43, 67, 44, -1, -1, 55, 48, 59, -1, 67, 45, 55, 48,
    59, -1, 18, 69, 19, -1, 70, -1, -1, 71, -1, 70, 45, 71, -1, 76, -1, 79, -1,
    68, -1, 75, -1, 72, -1, 73, -1, 74, -1, 24, 80, 26, 71, -1, 25, 77, 27, 87,
    26, 71, -1, 25, 77, 28, 87, 26, 71, -1, 20, 43, 3,  44, -1, 21, 43, 3,  44,
    -1, 22, 43, 6,  47, 3,  44, -1, 23, 43, 6,  47, 3,  44, -1, 22, 43, 6,  44,
    -1, 23, 43, 6,  44, -1, 22, 43, 6,  47, 87, 44, -1, 23, 43, 6,  47, 87, 44,
    -1, 22, 43, 3,  47, 3,  44, -1, 23, 43, 3,  47, 3,  44, -1, 22, 43, 3,  44,
    -1, 23, 43, 3,  44, -1, 22, 43, 3,  47, 87, 44, -1, 23, 43, 3,  47, 87, 44,
    -1, 29, 80, 30, 71, -1, 29, 80, 30, 71, 31, 71, -1, 78, 10, 87, -1, 76, -1,
    78, -1, 3,  -1, 3,  50, 87, 51, -1, 3,  -1, 3,  43, 86, 44, -1, 80, 38, 81,
    -1, 81, -1, 81, 36, 82, -1, 82, -1, 37, 82, -1, 83, -1, 43, 80, 44, -1, 84,
    -1, 87, 85, 87, -1, 36, -1, 38, -1, 32, -1, 33, -1, 34, -1, 35, -1, 87, -1,
    86, 47, 87, -1, 88, -1, 87, 8,  88, -1, 90, -1, 88, 9,  90, -1, 3,  43, 86,
    44, -1, 3,  -1, 3,  50, 87, 51, -1, 89, -1, 5,  -1, 4,  -1, 8,  90, -1, 43,
    87, 44, -1};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   164, 164, 164, 180, 186, 194, 194, 194, 196, 208, 214, 220, 228,
    228, 229, 229, 229, 229, 232, 232, 233, 236, 236, 246, 246, 258, 268,
    269, 272, 280, 281, 281, 282, 282, 283, 283, 283, 284, 284, 284, 284,
    286, 287, 288, 290, 294, 298, 302, 306, 306, 307, 307, 308, 314, 320,
    324, 328, 332, 336, 337, 340, 341, 341, 342, 346, 350, 355, 362, 362,
    363, 363, 364, 364, 365, 365, 366, 367, 367, 367, 367, 367, 367, 370,
    370, 371, 371, 372, 372, 373, 378, 378, 378, 378, 378, 378, 378};
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
    0,   256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267,
    268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280,
    281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293,
    294, 295, 296, 297, 40,  41,  59,  46,  44,  58,  61,  91,  93};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  52, 54, 53, 55, 55, 56, 56, 56, 57, 58, 58, 58, 59, 59, 60, 60,
    60, 60, 61, 61, 62, 64, 63, 65, 63, 66, 66, 67, 67, 68, 69, 69, 70,
    70, 71, 71, 71, 71, 71, 71, 71, 72, 72, 72, 73, 73, 74, 74, 74, 74,
    74, 74, 74, 74, 74, 74, 74, 74, 75, 75, 76, 77, 77, 78, 78, 79, 79,
    80, 80, 81, 81, 82, 82, 83, 83, 84, 85, 85, 85, 85, 85, 85, 86, 86,
    87, 87, 88, 88, 89, 90, 90, 90, 90, 90, 90, 90};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 0, 11, 3, 1, 1, 1, 0, 6, 6, 6, 6, 1, 9, 1, 1, 1, 1, 3, 0, 4, 0, 7, 0,
    5, 3, 0, 3,  5, 3, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 4, 6, 6, 4, 4, 6, 6, 4,
    4, 6, 6, 6,  6, 4, 4, 6, 6, 4, 6, 3, 1, 1, 1, 4, 1, 4, 3, 1, 3, 1, 2, 1, 3,
    1, 3, 1, 1,  1, 1, 1, 1, 1, 3, 1, 3, 1, 3, 4, 1, 4, 1, 1, 1, 2, 3};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    2,  0,  0,  1,  0,  0,  0,  5,  0,  0,  0,  4,  8,  20, 6,  7,  0,  0,  0,
    0,  0,  0,  0,  32, 0,  8,  0,  0,  0,  24, 22, 66, 0,  0,  0,  0,  0,  0,
    0,  37, 0,  31, 33, 39, 40, 41, 38, 35, 0,  36, 19, 20, 3,  0,  0,  0,  0,
    15, 16, 17, 18, 0,  13, 27, 27, 0,  0,  0,  0,  0,  0,  90, 94, 93, 0,  0,
    0,  0,  69, 71, 73, 75, 0,  85, 92, 87, 64, 62, 0,  63, 0,  30, 0,  0,  0,
    11, 10, 12, 0,  9,  0,  0,  0,  0,  0,  83, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  95, 72, 0,  0,  0,  0,  0,  0,  79, 80, 81, 82, 77, 78, 0,  0,  0,  0,
    0,  34, 61, 21, 0,  0,  0,  25, 0,  0,  67, 0,  65, 45, 46, 55, 0,  49, 0,
    56, 0,  50, 0,  0,  0,  74, 96, 42, 68, 70, 86, 76, 88, 0,  0,  59, 0,  0,
    26, 0,  0,  84, 90, 0,  90, 0,  90, 0,  90, 0,  89, 91, 0,  0,  0,  0,  28,
    0,  23, 53, 57, 47, 51, 54, 58, 48, 52, 43, 44, 60, 0,  0,  0,  29, 0,  14};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1, 1,   2,   8,  13, 14, 15, 61,  62,  18, 24, 25, 64,
    63, 101, 139, 39, 40, 41, 42, 43,  44,  45, 46, 47, 88,
    48, 49,  77,  78, 79, 80, 81, 129, 104, 82, 83, 84, 85};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -137
static const yytype_int16 yypact[] = {
    -137, 12,   10,   -137, 11,   42,   73,   72,   87,   73,   107,  -137,
    -137, 25,   -137, -137, 147,  73,   99,   111,  106,  159,  179,  118,
    138,  -137, 139,  173,  93,   -137, -137, 6,    141,  143,  144,  145,
    15,   186,  15,   -137, 171,  146,  -137, -137, -137, -137, -137, -137,
    182,  -137, -137, 25,   -137, 148,  149,  150,  151,  -137, -137, -137,
    -137, 152,  -137, 155,  155,  21,   21,   193,  196,  81,   158,  103,
    -137, -137, 21,   15,   15,   63,   164,  -137, -137, -137, 92,   194,
    -137, -137, 154,  -137, 153,  182,  -23,  -137, 118,  21,   99,   -137,
    -137, -137, 197,  -137, 73,   160,  161,  21,   121,  198,  2,    163,
    166,  122,  123,  127,  128,  21,   21,   -137, -137, 76,   71,   118,
    15,   15,   21,   -137, -137, -137, -137, -137, -137, 21,   21,   21,
    21,   118,  -137, 198,  -137, 162,  165,  16,   -137, 117,  19,   -137,
    21,   -137, -137, -137, -137, 29,   -137, 40,   -137, 70,   -137, 94,
    129,  3,    -137, -137, -137, 164,  -137, 194,  198,  -137, 9,    39,
    180,  168,  93,   -137, 73,   167,  198,  68,   23,   79,   33,   101,
    38,   105,  49,   -137, -137, 118,  118,  118,  210,  -137, 169,  -137,
    -137, -137, -137, -137, -137, -137, -137, -137, -137, -137, -137, 170,
    93,   200,  -137, 117,  -137};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -137, -137, -137, -9,  195,  -137, -137, -134, -136, 172,  -137, -137, -137,
    -137, 174,  -137, -14, -137, -137, -91,  -137, -137, -137, -137, 181,  -137,
    185,  -137, -17,  104, -66,  -137, -137, -137, 112,  -63,  97,   -137, -68};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int16 yytable[] = {
    11,  134, 105, 106, 26,  173, 115, 133, 20,  116, 122, 122, 3,   118, 5,
    120, -64, 122, 71,  72,  73,  90,  4,   74,  71,  72,  73,  122, 160, 74,
    135, 122, 175, 72,  73,  185, 189, 74,  16,  17,  142, 122, 168, 177, 72,
    73,  122, 122, 74,  65,  105, 157, 75,  145, 184, 162, 66,  122, 76,  117,
    171, 172, 165, 159, 103, 186, 164, 193, 166, 167, 206, 208, 103, 179, 72,
    73,  7,   195, 74,  122, 136, 174, 197, 103, 109, 6,   176, 110, 178, 119,
    180, 138, 182, 199, 200, 201, 202, 181, 72,  73,  122, 120, 74,  123, 124,
    125, 126, 127, 56,  128, 21,  113, 192, 103, 120, 159, 22,  23,  114, 9,
    158, 31,  113, 194, 123, 124, 125, 126, 127, 114, 128, 10,  57,  58,  59,
    60,  23,  103, 32,  33,  34,  35,  36,  37,  113, 196, 113, 38,  113, 198,
    19,  114, 12,  114, 28,  114, 57,  58,  59,  60,  27,  111, 29,  190, 112,
    143, 148, 150, 144, 149, 151, 152, 154, 183, 153, 155, 144, 53,  54,  55,
    131, 132, 30,  50,  67,  52,  68,  69,  70,  86,  91,  92,  93,  95,  96,
    97,  107, 99,  100, 108, 121, 98,  137, 130, 66,  140, 122, 146, 169, 141,
    147, 187, 191, 170, 188, 203, 207, 204, 87,  163, 51,  205, 89,  94,  161,
    156, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   102};

static const yytype_int16 yycheck[] = {
    9,   92,  65,  66, 18,  141, 74,  30, 17,  75,  8,   8,   0,   76,  3,
    38,  10,  8,   3,  4,   5,   38,  12, 8,   3,   4,   5,   8,   119, 8,
    93,  8,   3,   4,  5,   26,  170, 8,  13,  14,  103, 8,   133, 3,   4,
    5,   8,   8,   8,  43,  113, 114, 37, 51,  51,  121, 50,  8,   43,  76,
    44,  45,  130, 44, 43,  26,  129, 44, 131, 132, 204, 207, 43,  3,   4,
    5,   3,   44,  8,  8,   94,  144, 44, 43,  3,   43,  149, 6,   151, 26,
    153, 100, 155, 44, 185, 186, 187, 3,  4,   5,   8,   38,  8,   32,  33,
    34,  35,  36,  15, 38,  11,  43,  44, 43,  38,  44,  17,  18,  50,  47,
    44,  3,   43,  44, 32,  33,  34,  35, 36,  50,  38,  44,  39,  40,  41,
    42,  18,  43,  20, 21,  22,  23,  24, 25,  43,  44,  43,  29,  43,  44,
    3,   50,  45,  50, 48,  50,  39,  40, 41,  42,  49,  3,   3,   172, 6,
    44,  44,  44,  47, 47,  47,  44,  44, 44,  47,  47,  47,  4,   5,   6,
    27,  28,  3,   45, 43,  46,  43,  43, 43,  3,   19,  45,  10,  45,  45,
    45,  3,   45,  43, 3,   36,  50,  5,  9,   50,  45,  8,   44,  46,  48,
    44,  31,  45,  48, 46,  5,   16,  48, 37,  122, 25,  51,  37,  51,  120,
    113, -1,  -1,  -1, -1,  -1,  -1,  -1, -1,  -1,  -1,  -1,  -1,  64};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  53, 54, 0,  12, 3,  43, 3,  55, 47, 44, 55, 45, 56, 57, 58, 13, 14, 61,
    3,  55, 11, 17, 18, 62, 63, 68, 49, 48, 3,  3,  3,  20, 21, 22, 23, 24, 25,
    29, 68, 69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 45, 56, 46, 4,  5,  6,  15,
    39, 40, 41, 42, 59, 60, 65, 64, 43, 50, 43, 43, 43, 43, 3,  4,  5,  8,  37,
    43, 80, 81, 82, 83, 84, 87, 88, 89, 90, 3,  76, 77, 78, 80, 19, 45, 10, 61,
    45, 45, 45, 50, 45, 43, 66, 66, 43, 86, 87, 87, 3,  3,  3,  6,  3,  6,  43,
    50, 90, 82, 80, 87, 26, 38, 36, 8,  32, 33, 34, 35, 36, 38, 85, 9,  27, 28,
    30, 71, 87, 68, 5,  55, 67, 45, 48, 87, 44, 47, 51, 44, 44, 44, 47, 44, 47,
    44, 47, 44, 47, 86, 87, 44, 44, 71, 81, 82, 88, 87, 90, 87, 87, 71, 46, 48,
    44, 45, 60, 87, 3,  87, 3,  87, 3,  87, 3,  87, 44, 51, 26, 26, 31, 46, 59,
    55, 45, 44, 44, 44, 44, 44, 44, 44, 44, 71, 71, 71, 5,  48, 51, 59, 16, 60};

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
    case 55: /* "ident_lista" */

    {
        printf("Dropping ident_lista:  ");
        vec_debug_verbose(&(yyvaluep->idents));
        vec_drop(&(yyvaluep->idents));
    };

        break;
    case 67: /* "parametros_lista" */

    {
        printf("Dropping parametros_lista:  ");
        vec_debug_verbose(&(yyvaluep->idents));
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
        printf("Programa: %.*s\n", (yyvsp[(3) - (11)].slice).len,
               (yyvsp[(3) - (11)].slice).ptr);
        printf("Entradas: %zu\n", (yyvsp[(5) - (11)].idents).len);
        for (size_t i = 0; i < (yyvsp[(5) - (11)].idents).len; i++) {
            StrSlice *str = (StrSlice *)vec_get(&(yyvsp[(5) - (11)].idents), i);
            printf("    - %.*s\n", (int)str->len, str->ptr);
        }

        vec_drop(&(yyvsp[(5) - (11)].idents));
        ;
    } break;

    case 4:

    {
        (yyval.idents) = (yyvsp[(3) - (3)].idents);
        // vec_debug_verbose(&$$);
        StrSlice *sl = vec_push(&(yyval.idents));
        *sl = (yyvsp[(1) - (3)].slice);
        ;
    } break;

    case 5:

    {
        (yyval.idents) = vec_new(sizeof(StrSlice));
        // vec_debug_verbose(&$$);
        StrSlice *sl = vec_push(&(yyval.idents));
        *sl = (yyvsp[(1) - (1)].slice);
        ;
    } break;

    case 9:

    {
        printf("Declarando variables: %zu\n", (yyvsp[(3) - (6)].idents).len);
        for (size_t i = 0; i < (yyvsp[(3) - (6)].idents).len; i++) {
            Symbol s = SYM(*(StrSlice *)vec_get(&(yyvsp[(3) - (6)].idents), i));
            assert_not_sym_exists(&s);
            hashset_insert(&tabla, &s);
            printf("    - %.*s\n", (int)s.name.len, s.name.ptr);
        }

        vec_drop(&(yyvsp[(3) - (6)].idents));
        ;
    } break;

    case 10:

    {
        printf("Declarando constante: %lld\n", (yyvsp[(5) - (6)].snum));
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 11:

    {
        printf("Declarando constante: %f\n", (yyvsp[(5) - (6)].fnum));
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 12:

    {
        printf("Declarando constante: %.*s\n", (yyvsp[(5) - (6)].slice).len,
               (yyvsp[(5) - (6)].slice).ptr);
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        ;
    } break;

    case 21:

    {
        scope -= fun_id;
        ;
    } break;

    case 22:

    {
        printf("Declarando funcion %.*s\n", (int)(yyvsp[(2) - (2)].slice).len,
               (yyvsp[(2) - (2)].slice).ptr);
        Symbol s = SYM((yyvsp[(2) - (2)].slice));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 23:

    {
        printf("Declarada %.*s\n", (int)(yyvsp[(2) - (7)].slice).len,
               (yyvsp[(2) - (7)].slice).ptr);
        ;
    } break;

    case 24:

    {
        printf("Declarando procedure %.*s\n", (int)(yyvsp[(2) - (2)].slice).len,
               (yyvsp[(2) - (2)].slice).ptr);
        Symbol s = SYM((yyvsp[(2) - (2)].slice));
        assert_not_sym_exists(&s);
        hashset_insert(&tabla, &s);
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 25:

    {
        printf("Declarada %.*s\n", (int)(yyvsp[(2) - (5)].slice).len,
               (yyvsp[(2) - (5)].slice).ptr);
        ;
    } break;

    case 26:

    {
        printf("Argumentos: %zu\n", (yyvsp[(2) - (3)].idents).len);
        for (size_t i = 0; i < (yyvsp[(2) - (3)].idents).len; i++) {
            Symbol s = SYM(*(StrSlice *)vec_get(&(yyvsp[(2) - (3)].idents), i));
            assert_not_sym_exists(&s);
            hashset_insert(&tabla, &s);
            printf("    - %.*s\n", (int)s.name.len, s.name.ptr);
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
        Symbol s = SYM((yyvsp[(3) - (4)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 46:

    {
        Symbol s = SYM((yyvsp[(3) - (4)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 47:

    {
        Symbol s = SYM((yyvsp[(5) - (6)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 48:

    {
        Symbol s = SYM((yyvsp[(5) - (6)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 53:

    {
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        Symbol s1 = SYM((yyvsp[(5) - (6)].slice));
        assert_sym_exists(&s);
        assert_sym_exists(&s1);
        ;
    } break;

    case 54:

    {
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        Symbol s1 = SYM((yyvsp[(5) - (6)].slice));
        assert_sym_exists(&s);
        assert_sym_exists(&s1);
        ;
    } break;

    case 55:

    {
        Symbol s = SYM((yyvsp[(3) - (4)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 56:

    {
        Symbol s = SYM((yyvsp[(3) - (4)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 57:

    {
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 58:

    {
        Symbol s = SYM((yyvsp[(3) - (6)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 64:

    {
        Symbol s = SYM((yyvsp[(1) - (1)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 65:

    {
        Symbol s = SYM((yyvsp[(1) - (4)].slice));
        assert_sym_exists(&s);
        ;
    } break;

    case 66:

    {
        Symbol s = SYM((yyvsp[(1) - (1)].slice));
        printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
        assert_sym_exists(&s);
        ;
    } break;

    case 67:

    {
        Symbol s = SYM((yyvsp[(1) - (4)].slice));
        printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
        assert_sym_exists(&s);
        ;
    } break;

    case 89:

    {
        Symbol s = SYM((yyvsp[(1) - (4)].slice));
        printf("Llamando funcion: %.*s\n", (int)s.name.len, s.name.ptr);
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
