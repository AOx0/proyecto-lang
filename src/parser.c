
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

/* Line 171 of yacc.c  */
#line 1 "src/grammar.y"

#include "parser.h"
#include "hashset.h"
#include "node.h"
#include "str.h"
#include "symbol.h"
#include "tree.h"

extern FILE *OUT_FILE;

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

Node *tree_get_root(Tree *t) { return (Node *)vec_get(&t->values, 0); }

Node *tree_get_node(Tree *t, size_t id) {
    return (Node *)vec_get(&t->values, id);
}

void add_reference_to_sym(Symbol *s, size_t line) {
    size_t *ref = vec_push(&s->refs);
    *ref = line;
}

Symbol *assert_sym_exists(Symbol *s) {
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

void assert_not_sym_exists(Symbol *s) {
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

void assert_variable_assigned_type(Node *var, Node *val) {
    if (var->asoc_type != val->asoc_type) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento asignar una expresion de tipo "
                            "distinto a una variable: ");
        str_push_n(&wrn_buff, var->value.expr.value.symbol.name.ptr,
                   var->value.expr.value.symbol.name.len);
        str_push(&wrn_buff, ", la variable es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(var->asoc_type));
        str_push(&wrn_buff, " y la expresion es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(val->asoc_type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

void assert_expr_type(Node *a, Node *b) {
    if (a->asoc_type != b->asoc_type) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento realizar operaciones entre "
                            "expresiones de tipos distintos: ");
        str_push(&wrn_buff, "El primer operando es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(a->asoc_type));
        str_push(&wrn_buff, " y el segundo es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(b->asoc_type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

void assert_node_is_printable(Node *n) {
    if (n->asoc_type == Void || n->asoc_type == Ukw) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento imprimir una expresion de tipo "
                            "no imprimible, la expresion es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

void assert_ident_is_printable(Symbol *s) {
    if (s->asoc_type.type == Void || s->asoc_type.type == Ukw) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se intento imprimir un identificador de "
                            "tipo no imprimible, la expresion es de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(s->asoc_type.type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

void assert_tree_asoc_type_is(Tree t, DataTypeE type) {
    Node *n = tree_get_root(&t);

    if (n->asoc_type != type) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se esperaba una expresion de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(type));
        str_push(&wrn_buff, " pero se recibio una expresion de tipo ");
        str_push(&wrn_buff, data_type_e_display_return(n->asoc_type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

void assert_tree_node_type_is(Tree t, NodeType type) {
    Node *n = tree_get_root(&t);

    if (n->node_type != type) {
        str_clear(&wrn_buff);
        str_push(&wrn_buff, "Error: Se esperaba ");
        str_push(&wrn_buff, node_type_display(type));
        str_push(&wrn_buff, " pero se recibió ");
        str_push(&wrn_buff, node_type_display(n->node_type));
        yyerror(str_as_ref(&wrn_buff));
    }
}

/* Line 171 of yacc.c  */
#line 238 "grammar.tab.c"

/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 245 "grammar.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
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

/* Line 209 of yacc.c  */
#line 166 "src/grammar.y"

#ifndef LNG_PARSERH
#define LNG_PARSERH

#include "hashset.h"
#include "node.h"
#include "str.h"
#include "symbol.h"
#include "tree.h"
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

#endif

/* Line 209 of yacc.c  */
#line 301 "grammar.tab.c"

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

#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

/* Line 214 of yacc.c  */
#line 196 "src/grammar.y"

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
        FunctionCall function_call;
        Vec instructions;

/* Line 214 of yacc.c  */
#line 382 "grammar.tab.c"
} YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define yystype YYSTYPE /* obsolescent; will be withdrawn */
#define YYSTYPE_IS_DECLARED 1
#endif

/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 394 "grammar.tab.c"

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
#define YYNTOKENS 55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 39
/* YYNRULES -- Number of rules.  */
#define YYNRULES 99
/* YYNRULES -- Number of states.  */
#define YYNSTATES 210

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 300

#define YYTRANSLATE(YYX)                                                       \
    ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  46, 47, 2,  2,  50, 2,  49, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  51, 48, 2,  52, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  53, 2,  54, 2,
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
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] = {
    0,   0,   3,   4,   16,  20,  22,  24,  26,  27,  34,  41,  48,  55,  57,
    66,  68,  70,  72,  74,  78,  79,  84,  85,  93,  94,  100, 104, 105, 109,
    115, 119, 121, 122, 124, 128, 130, 132, 134, 136, 138, 140, 142, 147, 154,
    161, 166, 171, 178, 185, 190, 195, 202, 209, 216, 223, 228, 233, 240, 247,
    252, 259, 263, 265, 267, 269, 274, 276, 281, 285, 287, 291, 293, 296, 298,
    302, 304, 308, 310, 312, 314, 316, 318, 320, 322, 324, 326, 330, 331, 333,
    337, 339, 343, 348, 350, 355, 357, 359, 361, 364};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] = {
    56, 0,  -1, -1, 57, 12, 3,  46, 58, 47, 48, 59, 64, 71, 49, -1, 3,  50, 58,
    -1, 3,  -1, 60, -1, 61, -1, -1, 59, 14, 58, 51, 62, 48, -1, 59, 13, 3,  52,
    5,  48, -1, 59, 13, 3,  52, 4,  48, -1, 59, 13, 3,  52, 6,  48, -1, 63, -1,
    15, 53, 5,  32, 5,  54, 16, 63, -1, 42, -1, 43, -1, 44, -1, 45, -1, 64, 65,
    48, -1, -1, 66, 59, 64, 71, -1, -1, 17, 3,  67, 69, 51, 63, 48, -1, -1, 11,
    3,  68, 69, 48, -1, 46, 70, 47, -1, -1, 58, 51, 62, -1, 70, 48, 58, 51, 62,
    -1, 18, 72, 19, -1, 73, -1, -1, 74, -1, 73, 48, 74, -1, 79, -1, 82, -1, 71,
    -1, 78, -1, 75, -1, 76, -1, 77, -1, 24, 83, 26, 74, -1, 25, 80, 27, 90, 26,
    74, -1, 25, 80, 28, 90, 26, 74, -1, 20, 46, 3,  47, -1, 21, 46, 3,  47, -1,
    22, 46, 6,  50, 3,  47, -1, 23, 46, 6,  50, 3,  47, -1, 22, 46, 6,  47, -1,
    23, 46, 6,  47, -1, 22, 46, 6,  50, 90, 47, -1, 23, 46, 6,  50, 90, 47, -1,
    22, 46, 3,  50, 3,  47, -1, 23, 46, 3,  50, 3,  47, -1, 22, 46, 3,  47, -1,
    23, 46, 3,  47, -1, 22, 46, 3,  50, 90, 47, -1, 23, 46, 3,  50, 90, 47, -1,
    29, 83, 30, 74, -1, 29, 83, 30, 74, 31, 74, -1, 81, 10, 90, -1, 79, -1, 81,
    -1, 3,  -1, 3,  53, 5,  54, -1, 3,  -1, 3,  46, 89, 47, -1, 83, 41, 84, -1,
    84, -1, 84, 39, 85, -1, 85, -1, 40, 85, -1, 86, -1, 46, 83, 47, -1, 87, -1,
    90, 88, 90, -1, 39, -1, 41, -1, 35, -1, 36, -1, 37, -1, 38, -1, 33, -1, 34,
    -1, 90, -1, 89, 50, 90, -1, -1, 91, -1, 90, 8,  91, -1, 93, -1, 91, 9,  93,
    -1, 3,  46, 89, 47, -1, 3,  -1, 3,  53, 5,  54, -1, 92, -1, 5,  -1, 4,  -1,
    8,  93, -1, 46, 90, 47, -1};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,    294,  294,  294,  416,  421,  428,  428,  428,  430,  445,  462,
    479,  499,  500,  504,  505,  506,  507,  510,  513,  523,  549,  549,
    562,  562,  577,  590,  593,  604,  621,  624,  637,  646,  651,  658,
    659,  659,  659,  660,  660,  660,  663,  677,  691,  708,  722,  738,
    772,  807,  827,  847,  870,  894,  934,  975,  1002, 1029, 1057, 1085,
    1100, 1120, 1141, 1141, 1143, 1158, 1189, 1222, 1285, 1304, 1306, 1325,
    1327, 1343, 1345, 1345, 1347, 1365, 1366, 1367, 1368, 1369, 1370, 1371,
    1372, 1375, 1385, 1388, 1391, 1391, 1411, 1411, 1431, 1492, 1507, 1537,
    1557, 1570, 1583, 1600};
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
                                      "KW_DOTS",
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
                                      "subprogramas",
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
    297, 298, 299, 300, 40,  41,  59,  46,  44,  58,  61,  91,  93};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  55, 57, 56, 58, 58, 59, 59, 59, 60, 61, 61, 61, 62, 62, 63, 63,
    63, 63, 64, 64, 65, 67, 66, 68, 66, 69, 69, 70, 70, 71, 72, 72, 73,
    73, 74, 74, 74, 74, 74, 74, 74, 75, 75, 75, 76, 76, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 78, 78, 79, 80, 80, 81, 81, 82, 82,
    83, 83, 84, 84, 85, 85, 86, 86, 87, 88, 88, 88, 88, 88, 88, 88, 88,
    89, 89, 89, 90, 90, 91, 91, 92, 93, 93, 93, 93, 93, 93, 93};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 0, 11, 3, 1, 1, 1, 0, 6, 6, 6, 6, 1, 8, 1, 1, 1, 1, 3, 0, 4, 0, 7, 0,
    5, 3, 0, 3,  5, 3, 1, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 4, 6, 6, 4, 4, 6, 6, 4,
    4, 6, 6, 6,  6, 4, 4, 6, 6, 4, 6, 3, 1, 1, 1, 4, 1, 4, 3, 1, 3, 1, 2, 1, 3,
    1, 3, 1, 1,  1, 1, 1, 1, 1, 1, 1, 3, 0, 1, 3, 1, 3, 4, 1, 4, 1, 1, 1, 2, 3};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    2,  0,  0,  1,  0,  0,  0,  5,  0,  0,  0,  4,  8,  20, 6,  7,  0,  0,
    0,  0,  0,  0,  0,  32, 0,  8,  0,  0,  0,  24, 22, 66, 0,  0,  0,  0,
    0,  0,  0,  37, 0,  31, 33, 39, 40, 41, 38, 35, 0,  36, 19, 20, 3,  0,
    0,  0,  0,  15, 16, 17, 18, 0,  13, 27, 27, 87, 0,  0,  0,  0,  0,  93,
    97, 96, 0,  0,  0,  0,  69, 71, 73, 75, 0,  88, 95, 90, 64, 62, 0,  63,
    0,  30, 0,  0,  0,  11, 10, 12, 0,  9,  0,  0,  0,  0,  0,  85, 0,  0,
    0,  0,  0,  0,  0,  87, 0,  98, 72, 0,  0,  0,  0,  0,  0,  83, 84, 79,
    80, 81, 82, 77, 78, 0,  0,  0,  0,  0,  34, 61, 21, 0,  0,  0,  25, 0,
    0,  67, 0,  65, 45, 46, 55, 0,  49, 0,  56, 0,  50, 0,  0,  0,  74, 99,
    42, 68, 70, 89, 76, 91, 0,  0,  59, 0,  0,  26, 0,  0,  86, 93, 0,  93,
    0,  93, 0,  93, 0,  92, 94, 0,  0,  0,  0,  28, 0,  23, 53, 57, 47, 51,
    54, 58, 48, 52, 43, 44, 60, 0,  0,  0,  29, 14};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] = {
    -1, 1,   2,   8,  13, 14, 15, 61,  62,  18, 24, 25, 64,
    63, 101, 141, 39, 40, 41, 42, 43,  44,  45, 46, 47, 88,
    48, 49,  77,  78, 79, 80, 81, 131, 104, 82, 83, 84, 85};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -153
static const yytype_int16 yypact[] = {
    -153, 51,   23,   -153, 69,   21,   73,   79,   103,  73,   33,   -153,
    -153, 72,   -153, -153, 153,  73,   98,   120,  128,  155,  177,  122,
    133,  -153, 134,  170,  92,   -153, -153, 31,   136,  138,  139,  140,
    7,    184,  7,    -153, 169,  141,  -153, -153, -153, -153, -153, -153,
    180,  -153, -153, 72,   -153, 143,  144,  145,  142,  -153, -153, -153,
    -153, 146,  -153, 151,  151,  13,   191,  195,  196,  124,  125,  -27,
    -153, -153, 13,   7,    7,    -12,  161,  -153, -153, -153, 85,   192,
    -153, -153, 149,  -153, 150,  180,  25,   -153, 122,  13,   98,   -153,
    -153, -153, 198,  -153, 73,   156,  154,  13,   91,   199,  152,  162,
    163,  107,  117,  119,  121,  13,   203,  -153, -153, 48,   67,   122,
    7,    7,    13,   -153, -153, -153, -153, -153, -153, -153, -153, 13,
    13,   13,   13,   122,  -153, 199,  -153, 179,  164,  65,   -153, 118,
    -1,   -153, 13,   -153, -153, -153, -153, 19,   -153, 28,   -153, 34,
    -153, 53,   123,  158,  -153, -153, -153, 161,  -153, 192,  199,  -153,
    37,   52,   182,  209,  92,   -153, 73,   168,  199,  64,   1,    86,
    17,   102,  26,   106,  35,   -153, -153, 122,  122,  122,  165,  -153,
    166,  -153, -153, -153, -153, -153, -153, -153, -153, -153, -153, -153,
    -153, 202,  92,   118,  -153, -153};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {
    -153, -153, -153, -9,  197,  -153, -153, -152, -138, 172,  -153, -153, -153,
    -153, 157,  -153, -15, -153, -153, -91,  -153, -153, -153, -153, 183,  -153,
    187,  -153, 11,   105, -69,  -153, -153, -153, 113,  -63,  108,  -153, -70};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const yytype_int16 yytable[] = {
    11,  136, 105, 26,  115, 175, 116, 122, 20,  122, 71,  72,  73,  118, 119,
    74,  71,  72,  73,  113, 191, 74,  177, 72,  73,  122, 114, 74,  162, 120,
    137, 179, 72,  73,  122, 4,   74,  181, 72,  73,  144, -64, 74,  122, 170,
    122, 161, 75,  195, 90,  105, 3,   164, 76,  208, 135, 183, 72,  73,  103,
    122, 74,  167, 187, 197, 103, 120, 6,   166, 209, 168, 169, 5,   199, 103,
    122, 7,   65,  188, 138, 103, 12,  201, 176, 66,  16,  17,  117, 178, 120,
    180, 140, 182, 122, 184, 160, 202, 203, 204, 103, 123, 124, 125, 126, 127,
    128, 129, 56,  130, 21,  113, 194, 173, 174, 161, 22,  23,  114, 123, 124,
    125, 126, 127, 128, 129, 31,  130, 109, 111, 9,   110, 112, 113, 196, 57,
    58,  59,  60,  145, 114, 23,  146, 32,  33,  34,  35,  36,  37,  113, 198,
    10,  38,  113, 200, 150, 114, 19,  151, 29,  114, 57,  58,  59,  60,  152,
    192, 154, 153, 156, 155, 185, 157, 27,  146, 53,  54,  55,  133, 134, 28,
    30,  50,  67,  52,  68,  69,  70,  86,  91,  92,  93,  95,  96,  97,  99,
    98,  106, 100, 107, 108, 121, 132, 66,  139, 142, 143, 147, 122, 159, 148,
    149, 171, 186, 189, 190, 172, 193, 206, 207, 205, 87,  102, 51,  94,  89,
    163, 158, 0,   0,   0,   165};

static const yytype_int16 yycheck[] = {
    9,   92,  65,  18,  74,  143, 75,  8,   17,  8,  3,   4,   5,   76, 26,  8,
    3,   4,   5,   46,  172, 8,   3,   4,   5,   8,  53,  8,   119, 41, 93,  3,
    4,   5,   8,   12,  8,   3,   4,   5,   103, 10, 8,   8,   135, 8,  47,  40,
    47,  38,  113, 0,   121, 46,  206, 30,  3,   4,  5,   46,  8,   8,  132, 26,
    47,  46,  41,  46,  131, 207, 133, 134, 3,   47, 46,  8,   3,   46, 26,  94,
    46,  48,  47,  146, 53,  13,  14,  76,  151, 41, 153, 100, 155, 8,  157, 47,
    187, 188, 189, 46,  33,  34,  35,  36,  37,  38, 39,  15,  41,  11, 46,  47,
    47,  48,  47,  17,  18,  53,  33,  34,  35,  36, 37,  38,  39,  3,  41,  3,
    3,   50,  6,   6,   46,  47,  42,  43,  44,  45, 47,  53,  18,  50, 20,  21,
    22,  23,  24,  25,  46,  47,  47,  29,  46,  47, 47,  53,  3,   50, 3,   53,
    42,  43,  44,  45,  47,  174, 47,  50,  47,  50, 47,  50,  52,  50, 4,   5,
    6,   27,  28,  51,  3,   48,  46,  49,  46,  46, 46,  3,   19,  48, 10,  48,
    48,  48,  48,  53,  5,   46,  3,   3,   39,  9,  53,  5,   48,  51, 54,  8,
    5,   47,  47,  32,  54,  31,  5,   51,  48,  51, 16,  54,  37,  64, 25,  51,
    37,  120, 113, -1,  -1,  -1,  122};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  56, 57, 0,  12, 3,  46, 3,  58, 50, 47, 58, 48, 59, 60, 61, 13, 14,
    64, 3,  58, 11, 17, 18, 65, 66, 71, 52, 51, 3,  3,  3,  20, 21, 22, 23,
    24, 25, 29, 71, 72, 73, 74, 75, 76, 77, 78, 79, 81, 82, 48, 59, 49, 4,
    5,  6,  15, 42, 43, 44, 45, 62, 63, 68, 67, 46, 53, 46, 46, 46, 46, 3,
    4,  5,  8,  40, 46, 83, 84, 85, 86, 87, 90, 91, 92, 93, 3,  79, 80, 81,
    83, 19, 48, 10, 64, 48, 48, 48, 53, 48, 46, 69, 69, 46, 89, 90, 5,  3,
    3,  3,  6,  3,  6,  46, 53, 93, 85, 83, 90, 26, 41, 39, 8,  33, 34, 35,
    36, 37, 38, 39, 41, 88, 9,  27, 28, 30, 74, 90, 71, 5,  58, 70, 48, 51,
    90, 47, 50, 54, 47, 47, 47, 50, 47, 50, 47, 50, 47, 50, 89, 5,  47, 47,
    74, 84, 85, 91, 90, 93, 90, 90, 74, 32, 51, 47, 48, 63, 90, 3,  90, 3,
    90, 3,  90, 3,  90, 47, 54, 26, 26, 31, 5,  62, 58, 48, 47, 47, 47, 47,
    47, 47, 47, 47, 74, 74, 74, 54, 51, 16, 62, 63};

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
    case 58: /* "ident_lista" */

/* Line 1000 of yacc.c  */
#line 267 "src/grammar.y"
    {
        // printf("Dropping ident_lista:  ");
        // vec_debug_verbose(&$$);

        for (size_t i = 0; i < (yyvaluep->idents).len; i++) {
            Symbol *s = vec_get(&(yyvaluep->idents), i);
            vec_drop(&s->refs);
        }

        vec_drop(&(yyvaluep->idents));
    };

/* Line 1000 of yacc.c  */
#line 1495 "grammar.tab.c"
        break;
    case 70: /* "parametros_lista" */

/* Line 1000 of yacc.c  */
#line 279 "src/grammar.y"
    {
        // printf("Dropping parametros_lista:  ");
        // vec_debug_verbose(&$$);

        for (size_t i = 0; i < (yyvaluep->idents).len; i++) {
            Symbol *s = vec_get(&(yyvaluep->idents), i);
            vec_drop(&s->refs);
        }

        vec_drop(&(yyvaluep->idents));
    };

/* Line 1000 of yacc.c  */
#line 1514 "grammar.tab.c"
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

/* Line 1455 of yacc.c  */
#line 294 "src/grammar.y"
    {
        hashset_init(&tabla, sizeof(Symbol), hash_symbol);
        str_init(&wrn_buff);
        tree_init(&ast, sizeof(Node));
        ;
    } break;

    case 3:

/* Line 1455 of yacc.c  */
#line 300 "src/grammar.y"
    {

        for (size_t i = 0; i < (yyvsp[(5) - (11)].idents).len; i++) {
            Symbol *s = vec_get(&(yyvsp[(5) - (11)].idents), i);
            assert_not_sym_exists(s);
            s->type = Variable;
            s->asoc_type = (DataType){.type = Str, .size = 1};
            s->info.var = (VariableInfo){.addr = addr};
            hashset_insert(&tabla, s);
        }

        (yyvsp[(3) - (11)].symbol).type = Function;
        (yyvsp[(3) - (11)].symbol).asoc_type =
            (DataType){.type = Void, .size = 0};
        (yyvsp[(3) - (11)].symbol).info.fun = (FunctionInfo){
            .args = (yyvsp[(5) - (11)].idents),
        };
        assert_not_sym_exists(&(yyvsp[(3) - (11)].symbol));
        hashset_insert(&tabla, &(yyvsp[(3) - (11)].symbol));

        size_t idx;
        Node *node = (Node *)tree_new_node(&ast, &idx);
        *node = (Node){
            .node_type = NRoot,
            .id = idx,
            .asoc_type = Void,
        };

        size_t child_idx;
        node = (Node *)tree_new_node(&ast, &child_idx);
        *node = (Node){.node_type = NProgram,
                       .id = child_idx,
                       .value.fun =
                           (FunctionNode){.symbol = (yyvsp[(3) - (11)].symbol),
                                          .args = (yyvsp[(5) - (11)].idents)},
                       .asoc_type = Void};
        tree_new_relation(&ast, idx, child_idx);

        for (size_t i = 0; i < (yyvsp[(8) - (11)].idents).len; i++) {
            Symbol *s = vec_get(&(yyvsp[(8) - (11)].idents), i);
            node = (Node *)tree_new_node(&ast, &child_idx);

            if (s->type == Variable) {
                *node = (Node){
                    .node_type = NVar,
                    .id = child_idx,
                    .value.var = (VarNode){.symbol = *s},
                };
            } else {
                *node = (Node){
                    .node_type = NConst,
                    .id = child_idx,
                    .value.cons = (ConstNode){.symbol = *s, .value.bool = 1},
                };
            }
            tree_new_relation(&ast, idx, child_idx);
        }

        tree_extend(&ast, &(yyvsp[(9) - (11)].subtree), 0);

#ifdef PRINT_TABLE
        size_t i = 0;
        while (tabla.elements > i) {
            for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
                Vec *arr = (Vec *)vec_get(&tabla.values, j);
                for (size_t h = 0; h < arr->len; h++) {
                    Symbol *s = (Symbol *)vec_get(arr, h);
                    sym_type_display(s->type);
                    printf(
                        " (%.2zu,%.2zu), name: %10.*s, location: %2zu:%-2zu, "
                        "scope: %zu, ",
                        j, h, (int)s->name.len, s->name.ptr, s->line, s->nchar,
                        s->scope);

                    printf("type: ");
                    data_type_debug(&s->asoc_type);
                    printf(", ");

                    printf("info: ");

                    switch (s->type) {
                    case Function: {
                        fun_info_debug(&s->info.fun);
                        break;
                    }
                    case Variable: {
                        var_info_debug(&s->info.var);
                        break;
                    }
                    case Constant: {
                        const_info_debug(&s->info.cons);
                        break;
                    }
                    case Procedure: {
                        fun_info_debug(&s->info.fun);
                        break;
                    }
                    default: {
                        panic("Invalid SymbolType");
                    }
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

        if (err == 0) {
            // node_display_id(idx, OUT_FILE, &ast, &tabla);
            tree_debug(&ast);
        }

        // Al final liberamos la tabla de hashes de memoria
        vec_drop(&(yyvsp[(5) - (11)].idents));
        hashset_drop(&tabla);
        str_drop(&wrn_buff);
        tree_drop(&ast);
        ;
    } break;

    case 4:

/* Line 1455 of yacc.c  */
#line 416 "src/grammar.y"
    {
        (yyval.idents) = (yyvsp[(3) - (3)].idents);
        Symbol *s = vec_push(&(yyval.idents));
        *s = (yyvsp[(1) - (3)].symbol);
        ;
    } break;

    case 5:

/* Line 1455 of yacc.c  */
#line 421 "src/grammar.y"
    {
        (yyval.idents) = vec_new(sizeof(Symbol));
        Symbol *s = vec_push(&(yyval.idents));
        *s = (yyvsp[(1) - (1)].symbol);
        ;
    } break;

    case 8:

/* Line 1455 of yacc.c  */
#line 428 "src/grammar.y"
    {
        (yyval.idents) = vec_new(sizeof(Symbol));
        ;
    } break;

    case 9:

/* Line 1455 of yacc.c  */
#line 430 "src/grammar.y"
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
        ;
    } break;

    case 10:

/* Line 1455 of yacc.c  */
#line 445 "src/grammar.y"
    {
        (yyvsp[(3) - (6)].symbol).type = Constant;
        (yyvsp[(3) - (6)].symbol).asoc_type =
            (DataType){.type = Int, .size = 1};
        (yyvsp[(3) - (6)].symbol).info.cons =
            (ConstantInfo){.addr = addr, .value.snum = (yyvsp[(5) - (6)].snum)};
        addr += 4;
        assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
        hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));
        (yyval.idents) = (yyvsp[(1) - (6)].idents);
        Symbol *s = (Symbol *)vec_push(&(yyval.idents));
        *s = (yyvsp[(3) - (6)].symbol);
        ;
    } break;

    case 11:

/* Line 1455 of yacc.c  */
#line 462 "src/grammar.y"
    {
        (yyvsp[(3) - (6)].symbol).type = Constant;
        (yyvsp[(3) - (6)].symbol).asoc_type =
            (DataType){.type = Real, .size = 1};
        (yyvsp[(3) - (6)].symbol).info.cons =
            (ConstantInfo){.addr = addr, .value.real = (yyvsp[(5) - (6)].fnum)};
        addr += 4;
        assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
        hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));
        (yyval.idents) = (yyvsp[(1) - (6)].idents);
        Symbol *s = (Symbol *)vec_push(&(yyval.idents));
        *s = (yyvsp[(3) - (6)].symbol);
        ;
    } break;

    case 12:

/* Line 1455 of yacc.c  */
#line 479 "src/grammar.y"
    {
        (yyvsp[(3) - (6)].symbol).type = Constant;
        (yyvsp[(3) - (6)].symbol).asoc_type =
            (DataType){.type = Str, .size = (yyvsp[(5) - (6)].slice).len};
        (yyvsp[(3) - (6)].symbol).info.cons =
            (ConstantInfo){.addr = addr, .value.str = (yyvsp[(5) - (6)].slice)};
        addr += 1 * (yyvsp[(5) - (6)].slice).len;
        assert_not_sym_exists(&(yyvsp[(3) - (6)].symbol));
        hashset_insert(&tabla, &(yyvsp[(3) - (6)].symbol));

        (yyval.idents) = (yyvsp[(1) - (6)].idents);
        Symbol *s = (Symbol *)vec_push(&(yyval.idents));
        *s = (yyvsp[(3) - (6)].symbol);
        ;
    } break;

    case 13:

/* Line 1455 of yacc.c  */
#line 499 "src/grammar.y"
    {
        (yyval.type) = (yyvsp[(1) - (1)].type);
        ;
    } break;

    case 14:

/* Line 1455 of yacc.c  */
#line 500 "src/grammar.y"
    {
        (yyval.type) = (yyvsp[(8) - (8)].type);
        (yyval.type).size = (yyval.type).size *
                            ((yyvsp[(5) - (8)].snum) - (yyvsp[(3) - (8)].snum));
        ;
    } break;

    case 15:

/* Line 1455 of yacc.c  */
#line 504 "src/grammar.y"
    {
        (yyval.type) = (DataType){.type = Int, .size = 1};
        ;
    } break;

    case 16:

/* Line 1455 of yacc.c  */
#line 505 "src/grammar.y"
    {
        (yyval.type) = (DataType){.type = Real, .size = 1};
        ;
    } break;

    case 17:

/* Line 1455 of yacc.c  */
#line 506 "src/grammar.y"
    {
        (yyval.type) = (DataType){.type = Str, .size = 1};
        ;
    } break;

    case 18:

/* Line 1455 of yacc.c  */
#line 507 "src/grammar.y"
    {
        (yyval.type) = (DataType){.type = Bool, .size = 1};
        ;
    } break;

    case 19:

/* Line 1455 of yacc.c  */
#line 510 "src/grammar.y"
    {
        (yyval.subtree) = (yyvsp[(1) - (3)].subtree);
        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (3)].subtree), 0);
        ;
    } break;

    case 20:

/* Line 1455 of yacc.c  */
#line 513 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){
            .node_type = NVoid,
            .asoc_type = Void,
        };
        ;
    } break;

    case 21:

/* Line 1455 of yacc.c  */
#line 523 "src/grammar.y"
    {
        scope -= fun_id;

        tree_init(&(yyval.subtree), sizeof(Node));

        Node *root = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *root = (Node){
            .node_type = NVoid,
            .asoc_type = Void,
        };

        // Add to tree subprogram declarations
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (4)].subtree), 0);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NFunction,
                    .asoc_type = Void,
                    .value.fun = (FunctionNode){
                        .symbol = (yyvsp[(1) - (4)].symbol),
                        .args = (yyvsp[(2) - (4)].idents),
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0);
        ;
    } break;

    case 22:

/* Line 1455 of yacc.c  */
#line 549 "src/grammar.y"
    {
        (yyvsp[(2) - (2)].symbol).type = Function;
        assert_not_sym_exists(&(yyvsp[(2) - (2)].symbol));
        hashset_insert(&tabla, &(yyvsp[(2) - (2)].symbol));
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 23:

/* Line 1455 of yacc.c  */
#line 556 "src/grammar.y"
    {
        Symbol *s = (Symbol *)hashset_get(&tabla, &(yyvsp[(2) - (7)].symbol));
        s->asoc_type = (yyvsp[(6) - (7)].type);
        s->info.fun = (FunctionInfo){.args = (yyvsp[(4) - (7)].idents)};
        (yyval.symbol) = *s;
        ;
    } break;

    case 24:

/* Line 1455 of yacc.c  */
#line 562 "src/grammar.y"
    {
        (yyvsp[(2) - (2)].symbol).type = Procedure;
        assert_not_sym_exists(&(yyvsp[(2) - (2)].symbol));
        hashset_insert(&tabla, &(yyvsp[(2) - (2)].symbol));
        fun_id++;
        scope += fun_id;
        ;
    } break;

    case 25:

/* Line 1455 of yacc.c  */
#line 569 "src/grammar.y"
    {
        Symbol *s = (Symbol *)hashset_get(&tabla, &(yyvsp[(2) - (5)].symbol));
        s->asoc_type = (DataType){.type = Void, .size = 0};
        s->info.fun = (FunctionInfo){.args = (yyvsp[(4) - (5)].idents)};
        (yyval.symbol) = *s;
        ;
    } break;

    case 26:

/* Line 1455 of yacc.c  */
#line 577 "src/grammar.y"
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
        ;
    } break;

    case 27:

/* Line 1455 of yacc.c  */
#line 590 "src/grammar.y"
    {
        (yyval.idents) = vec_new(sizeof(Symbol));
        ;
    } break;

    case 28:

/* Line 1455 of yacc.c  */
#line 593 "src/grammar.y"
    {
        for (size_t i = 0; i < (yyvsp[(1) - (3)].idents).len; i++) {
            Symbol *s = (Symbol *)vec_get(&(yyvsp[(1) - (3)].idents), i);
            s->type = Variable;
            s->asoc_type = (yyvsp[(3) - (3)].type);
            s->info.var = (VariableInfo){.addr = addr};
            addr += data_type_size(&(yyvsp[(3) - (3)].type));
            // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
        }
        (yyval.idents) = (yyvsp[(1) - (3)].idents);
        ;
    } break;

    case 29:

/* Line 1455 of yacc.c  */
#line 604 "src/grammar.y"
    {
        (yyval.idents) = (yyvsp[(1) - (5)].idents);

        for (size_t i = 0; i < (yyvsp[(3) - (5)].idents).len; i++) {
            Symbol *s = (Symbol *)vec_get(&(yyvsp[(3) - (5)].idents), i);
            s->type = Variable;
            s->asoc_type = (yyvsp[(5) - (5)].type);
            s->info.var = (VariableInfo){.addr = addr};
            addr += data_type_size(&(yyvsp[(5) - (5)].type));
            // printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
        }

        vec_extend(&(yyval.idents), &(yyvsp[(3) - (5)].idents));
        // vec_drop(&$3);
        ;
    } break;

    case 30:

/* Line 1455 of yacc.c  */
#line 621 "src/grammar.y"
    {
        (yyval.subtree) = (yyvsp[(2) - (3)].subtree);
        ;
    } break;

    case 31:

/* Line 1455 of yacc.c  */
#line 624 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){
            .node_type = NVoid,
            .asoc_type = Void,
        };

        for (size_t i = 0; i < (yyvsp[(1) - (1)].instructions).len; i--) {
            Tree *t2 = (Tree *)vec_get(&(yyvsp[(1) - (1)].instructions), i);
            tree_extend(&(yyval.subtree), t2, 0);
        };
    } break;

    case 32:

/* Line 1455 of yacc.c  */
#line 637 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){
            .node_type = NVoid,
            .asoc_type = Void,
        };
        ;
    } break;

    case 33:

/* Line 1455 of yacc.c  */
#line 646 "src/grammar.y"
    {
        vec_init(&(yyval.instructions), sizeof(Tree));

        Tree *sub = vec_push(&(yyval.instructions));
        *sub = (yyvsp[(1) - (1)].subtree);
        ;
    } break;

    case 34:

/* Line 1455 of yacc.c  */
#line 651 "src/grammar.y"
    {
        (yyval.instructions) = (yyvsp[(1) - (3)].instructions);
        vec_push(&(yyval.instructions));

        Tree *sub = vec_push(&(yyval.instructions));
        *sub = (yyvsp[(3) - (3)].subtree);
        ;
    } break;

    case 42:

/* Line 1455 of yacc.c  */
#line 663 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);

        *n = (Node){
            .node_type = NWhile,
            .asoc_type = Void,
        };

        assert_tree_asoc_type_is((yyvsp[(2) - (4)].subtree), Bool);

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (4)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0);
        ;
    } break;

    case 43:

/* Line 1455 of yacc.c  */
#line 677 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NFor,
                    .asoc_type = Void,
                    .value.forn = (ForNode){
                        .down = 0,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0);
        ;
    } break;

    case 44:

/* Line 1455 of yacc.c  */
#line 691 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NFor,
                    .asoc_type = Void,
                    .value.forn = (ForNode){
                        .down = 1,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0);
        ;
    } break;

    case 45:

/* Line 1455 of yacc.c  */
#line 708 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (4)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NRead,
                    .asoc_type = Void,
                    .value.read = (ReadNode){
                        .newline = 0,
                        .target_symbol = (yyvsp[(3) - (4)].symbol),
                    }};
        ;
    } break;

    case 46:

/* Line 1455 of yacc.c  */
#line 722 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (4)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NRead,
                    .asoc_type = Void,
                    .value.read = (ReadNode){
                        .newline = 1,
                        .target_symbol = (yyvsp[(3) - (4)].symbol),
                    }};
        ;
    } break;

    case 47:

/* Line 1455 of yacc.c  */
#line 738 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(5) - (6)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (6)].slice),
                                      .value.str = (yyvsp[(3) - (6)].slice)}};

        Node *var = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var = (Node){.node_type = NExpr,
                      .asoc_type = (yyvsp[(5) - (6)].symbol).asoc_type.type,
                      .value.expr = (ExprNode){
                          .type = ESymbol,
                          .value.symbol = (yyvsp[(5) - (6)].symbol),
                      }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_new_relation(&(yyval.subtree), 0, 2);
        ;
    } break;

    case 48:

/* Line 1455 of yacc.c  */
#line 772 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(5) - (6)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (6)].slice),
                                      .value.str = (yyvsp[(3) - (6)].slice)}};

        Node *var = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var = (Node){.node_type = NExpr,
                      .asoc_type = (yyvsp[(5) - (6)].symbol).asoc_type.type,
                      .value.expr = (ExprNode){
                          .type = ESymbol,
                          .value.symbol = (yyvsp[(5) - (6)].symbol),
                      }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_new_relation(&(yyval.subtree), 0, 2);
        ;
    } break;

    case 49:

/* Line 1455 of yacc.c  */
#line 807 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (4)].slice),
                                      .value.str = (yyvsp[(3) - (4)].slice)}};

        tree_new_relation(&(yyval.subtree), 0, 1);
        ;
    } break;

    case 50:

/* Line 1455 of yacc.c  */
#line 827 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (4)].slice),
                                      .value.str = (yyvsp[(3) - (4)].slice)}};

        tree_new_relation(&(yyval.subtree), 0, 1);
        ;
    } break;

    case 51:

/* Line 1455 of yacc.c  */
#line 847 "src/grammar.y"
    {
        assert_node_is_printable(tree_get_root(&(yyvsp[(5) - (6)].subtree)));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (6)].slice),
                                      .value.str = (yyvsp[(3) - (6)].slice)}};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_extend(&(yyval.subtree), &(yyvsp[(5) - (6)].subtree), 0);
        ;
    } break;

    case 52:

/* Line 1455 of yacc.c  */
#line 870 "src/grammar.y"
    {
        assert_node_is_printable(tree_get_root(&(yyvsp[(5) - (6)].subtree)));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *str = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *str = (Node){.node_type = NConst,
                      .asoc_type = Str,
                      .value.cons =
                          (ConstNode){.symbol = (yyvsp[(3) - (6)].slice),
                                      .value.str = (yyvsp[(3) - (6)].slice)}};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_extend(&(yyval.subtree), &(yyvsp[(5) - (6)].subtree), 0);
        ;
    } break;

    case 53:

/* Line 1455 of yacc.c  */
#line 894 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (6)].symbol));
        assert_sym_exists(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(3) - (6)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *var = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var = (Node){.node_type = NExpr,
                      .asoc_type = (yyvsp[(3) - (6)].symbol).asoc_type.type,
                      .value.expr = (ExprNode){
                          .type = ESymbol,
                          .value.symbol = (yyvsp[(3) - (6)].symbol),
                      }};

        Node *var2 = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var2 = (Node){.node_type = NExpr,
                       .asoc_type = (yyvsp[(5) - (6)].symbol).asoc_type.type,
                       .value.expr = (ExprNode){
                           .type = ESymbol,
                           .value.symbol = (yyvsp[(5) - (6)].symbol),
                       }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_new_relation(&(yyval.subtree), 0, 2);
        ;
    } break;

    case 54:

/* Line 1455 of yacc.c  */
#line 934 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (6)].symbol));
        assert_sym_exists(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(5) - (6)].symbol));
        assert_ident_is_printable(&(yyvsp[(3) - (6)].symbol));

        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *var = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var = (Node){.node_type = NExpr,
                      .asoc_type = (yyvsp[(3) - (6)].symbol).asoc_type.type,
                      .value.expr = (ExprNode){
                          .type = ESymbol,
                          .value.symbol = (yyvsp[(3) - (6)].symbol),
                      }};

        Node *var2 = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var2 = (Node){.node_type = NExpr,
                       .asoc_type = (yyvsp[(5) - (6)].symbol).asoc_type.type,
                       .value.expr = (ExprNode){
                           .type = ESymbol,
                           .value.symbol = (yyvsp[(5) - (6)].symbol),
                       }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_new_relation(&(yyval.subtree), 0, 2);
        ;
    } break;

    case 55:

/* Line 1455 of yacc.c  */
#line 975 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (4)].symbol));
        assert_ident_is_printable(&(yyvsp[(3) - (4)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *var = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *var = (Node){.node_type = NExpr,
                      .asoc_type = (yyvsp[(3) - (4)].symbol).asoc_type.type,
                      .value.expr = (ExprNode){
                          .type = ESymbol,
                          .value.symbol = (yyvsp[(3) - (4)].symbol),
                      }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        ;
    } break;

    case 56:

/* Line 1455 of yacc.c  */
#line 1002 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (4)].symbol));
        assert_ident_is_printable(&(yyvsp[(3) - (4)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *ident = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *ident = (Node){.node_type = NExpr,
                        .asoc_type = (yyvsp[(3) - (4)].symbol).asoc_type.type,
                        .value.expr = (ExprNode){
                            .type = ESymbol,
                            .value.symbol = (yyvsp[(3) - (4)].symbol),
                        }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        ;
    } break;

    case 57:

/* Line 1455 of yacc.c  */
#line 1029 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (6)].symbol));
        assert_node_is_printable(tree_get_root(&(yyvsp[(5) - (6)].subtree)));
        assert_ident_is_printable(&(yyvsp[(3) - (6)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 0,
                    }};

        Node *ident = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *ident = (Node){.node_type = NExpr,
                        .asoc_type = (yyvsp[(3) - (6)].symbol).asoc_type.type,
                        .value.expr = (ExprNode){
                            .type = ESymbol,
                            .value.symbol = (yyvsp[(3) - (6)].symbol),
                        }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_extend(&(yyval.subtree), &(yyvsp[(5) - (6)].subtree), 0);
        ;
    } break;

    case 58:

/* Line 1455 of yacc.c  */
#line 1057 "src/grammar.y"
    {
        assert_sym_exists(&(yyvsp[(3) - (6)].symbol));
        assert_node_is_printable(tree_get_root(&(yyvsp[(5) - (6)].subtree)));
        assert_ident_is_printable(&(yyvsp[(3) - (6)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NWrite,
                    .asoc_type = Void,
                    .value.write = (WriteNode){
                        .newline = 1,
                    }};

        Node *ident = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *ident = (Node){.node_type = NExpr,
                        .asoc_type = (yyvsp[(3) - (6)].symbol).asoc_type.type,
                        .value.expr = (ExprNode){
                            .type = ESymbol,
                            .value.symbol = (yyvsp[(3) - (6)].symbol),
                        }};

        tree_new_relation(&(yyval.subtree), 0, 1);
        tree_extend(&(yyval.subtree), &(yyvsp[(5) - (6)].subtree), 0);
        ;
    } break;

    case 59:

/* Line 1455 of yacc.c  */
#line 1085 "src/grammar.y"
    {
        assert_expr_type(tree_get_root(&(yyvsp[(2) - (4)].subtree)),
                         tree_get_root(&(yyvsp[(4) - (4)].subtree)));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NIf,
                    .asoc_type = Void,
                    .value.ifn = (IfNode){.blocks = 1}};

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (4)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (4)].subtree), 0);
        ;
    } break;

    case 60:

/* Line 1455 of yacc.c  */
#line 1100 "src/grammar.y"
    {

        assert_expr_type(tree_get_root(&(yyvsp[(2) - (6)].subtree)),
                         tree_get_root(&(yyvsp[(4) - (6)].subtree)));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NIf,
                    .asoc_type = Void,
                    .value.ifn = (IfNode){.blocks = 2}};

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (6)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(4) - (6)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(6) - (6)].subtree), 0);
        ;
    } break;

    case 61:

/* Line 1455 of yacc.c  */
#line 1120 "src/grammar.y"
    {
        Node *var = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *expr = tree_get_root(&(yyvsp[(3) - (3)].subtree));

        assert_variable_assigned_type(var, expr);
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = var->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = OpAssign,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 64:

/* Line 1455 of yacc.c  */
#line 1143 "src/grammar.y"
    {
        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (1)].symbol));
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){
                            .type = ESymbol,
                            .value.symbol = *s,
                        },
                    .asoc_type = s->asoc_type.type};
        ;
    } break;

    case 65:

/* Line 1455 of yacc.c  */
#line 1158 "src/grammar.y"
    {
        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));

        size_t arr_size = s->asoc_type.size;
        if ((int64_t)arr_size < (yyvsp[(3) - (4)].snum) ||
            (yyvsp[(3) - (4)].snum) < 0) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Indice fuera de rango: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                       (yyvsp[(1) - (4)].symbol).name.len);
            str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
            str_push_sizet(&wrn_buff, arr_size);
            str_push(&wrn_buff, " y se intento acceder a la posicion ");
            str_push_sizet(&wrn_buff, (yyvsp[(3) - (4)].snum));
            yyerror(str_as_ref(&wrn_buff));
        }

        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){.type = ESymbolIdx,
                                   .value.symbol_idx =
                                       (IndexedSymbol){
                                           .symbol = *s,
                                           .index = (yyvsp[(3) - (4)].snum)}},
                    .asoc_type = s->asoc_type.type};
        ;
    } break;

    case 66:

/* Line 1455 of yacc.c  */
#line 1189 "src/grammar.y"
    {
        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (1)].symbol));
        if (s->type != Procedure) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una variable como "
                                "si fuera una funcion: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (1)].symbol).name.ptr,
                       (yyvsp[(1) - (1)].symbol).name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        if (s->info.fun.args.len != 0) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una funcion con "
                                "argumentos sin pasarle argumentos: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (1)].symbol).name.ptr,
                       (yyvsp[(1) - (1)].symbol).name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        tree_init(&(yyval.subtree), sizeof(Node));

        Tree args;
        tree_init(&args, sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NCall,
                    .asoc_type = s->asoc_type.type,
                    .value.call = (FunctionCall){
                        .args = args,
                        .symbol = (yyvsp[(1) - (1)].symbol),
                        .return_type = s->asoc_type.type,
                    }};
        ;
    } break;

    case 67:

/* Line 1455 of yacc.c  */
#line 1222 "src/grammar.y"
    {
        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));

        Vec children = tree_get_childs(&(yyvsp[(3) - (4)].subtree), 0);

        // Checamos que los tipos de datos de los argumentos hacen match con los
        // del simbolo
        if (s->info.fun.args.len != children.len) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff,
                     "Error: Se intento llamar a una funcion con una cantidad "
                     "de argumentos distinta a la declarada: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                       (yyvsp[(1) - (4)].symbol).name.len);
            str_push(&wrn_buff, ", se esperaban ");
            str_push_sizet(&wrn_buff, s->info.fun.args.len);
            str_push(&wrn_buff, " argumentos y se pasaron ");
            str_push_sizet(&wrn_buff, children.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        if (children.len > 0) {
            Node *void_root =
                (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values, 0);

            if (void_root->node_type != NVoid) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion "
                                    "con argumentos sin pasarle argumentos: ");
                str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                           (yyvsp[(1) - (4)].symbol).name.len);
                yyerror(str_as_ref(&wrn_buff));
            }

            for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
                Node *arg =
                    (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values,
                                    *(size_t *)vec_get(&children, n_arg));
                Symbol *arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

                if (arg->asoc_type != arg_sym->asoc_type.type) {
                    str_clear(&wrn_buff);
                    str_push(
                        &wrn_buff,
                        "Error: Se intento llamar a una funcion con argumentos "
                        "de tipos distintos a los declarados: ");
                    str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                               (yyvsp[(1) - (4)].symbol).name.len);
                    str_push(&wrn_buff, ", el argumento ");
                    str_push_sizet(&wrn_buff, n_arg + 1);
                    str_push(&wrn_buff, " se esperaba de tipo ");
                    str_push(&wrn_buff, data_type_e_display_return(
                                            arg_sym->asoc_type.type));
                    str_push(&wrn_buff, " y se paso de tipo ");
                    str_push(&wrn_buff,
                             data_type_e_display_return(arg->asoc_type));
                    yyerror(str_as_ref(&wrn_buff));
                }
            }

            vec_drop(&children);
        }

        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NCall,
                    .asoc_type = s->asoc_type.type,
                    .value.call = (FunctionCall){
                        .symbol = (yyvsp[(1) - (4)].symbol),
                        .args = (yyvsp[(3) - (4)].subtree),
                    }};
        ;
    } break;

    case 68:

/* Line 1455 of yacc.c  */
#line 1285 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *rhs = tree_get_root(&(yyvsp[(3) - (3)].subtree));
        assert_expr_type(lhs, rhs);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = lhs->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = OpOr,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 70:

/* Line 1455 of yacc.c  */
#line 1306 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *rhs = tree_get_root(&(yyvsp[(3) - (3)].subtree));
        assert_expr_type(lhs, rhs);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = lhs->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = OpAnd,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 72:

/* Line 1455 of yacc.c  */
#line 1327 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(2) - (2)].subtree));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = lhs->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = OpNot,
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(2) - (2)].subtree), 0);
        ;
    } break;

    case 74:

/* Line 1455 of yacc.c  */
#line 1345 "src/grammar.y"
    {
        (yyval.subtree) = (yyvsp[(2) - (3)].subtree);
        ;
    } break;

    case 76:

/* Line 1455 of yacc.c  */
#line 1347 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *rhs = tree_get_root(&(yyvsp[(3) - (3)].subtree));
        assert_expr_type(lhs, rhs);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){
            .node_type = NExpr,
            .asoc_type = Bool,
            .value.expr = (yyvsp[(2) - (3)].expr),
        };

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 77:

/* Line 1455 of yacc.c  */
#line 1365 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpAnd};
        ;
    } break;

    case 78:

/* Line 1455 of yacc.c  */
#line 1366 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpOr};
        ;
    } break;

    case 79:

/* Line 1455 of yacc.c  */
#line 1367 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpBt};
        ;
    } break;

    case 80:

/* Line 1455 of yacc.c  */
#line 1368 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpLt};
        ;
    } break;

    case 81:

/* Line 1455 of yacc.c  */
#line 1369 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpEbt};
        ;
    } break;

    case 82:

/* Line 1455 of yacc.c  */
#line 1370 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpElt};
        ;
    } break;

    case 83:

/* Line 1455 of yacc.c  */
#line 1371 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpEq};
        ;
    } break;

    case 84:

/* Line 1455 of yacc.c  */
#line 1372 "src/grammar.y"
    {
        (yyval.expr) = (ExprNode){.type = EOp, .value.op = OpNeq};
        ;
    } break;

    case 85:

/* Line 1455 of yacc.c  */
#line 1375 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *void_root = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *void_root = (Node){
            .node_type = NVoid,
            .id = 0,
        };

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (1)].subtree), 0);
        ;
    } break;

    case 86:

/* Line 1455 of yacc.c  */
#line 1385 "src/grammar.y"
    {
        (yyval.subtree) = (yyvsp[(1) - (3)].subtree);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 87:

/* Line 1455 of yacc.c  */
#line 1388 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));
        ;
    } break;

    case 89:

/* Line 1455 of yacc.c  */
#line 1391 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *rhs = tree_get_root(&(yyvsp[(3) - (3)].subtree));
        assert_expr_type(lhs, rhs);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = lhs->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = (yyvsp[(2) - (3)].op),
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 91:

/* Line 1455 of yacc.c  */
#line 1411 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *lhs = tree_get_root(&(yyvsp[(1) - (3)].subtree));
        Node *rhs = tree_get_root(&(yyvsp[(3) - (3)].subtree));
        assert_expr_type(lhs, rhs);

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = lhs->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EOp,
                        .value.op = (yyvsp[(2) - (3)].op),
                    }};

        tree_extend(&(yyval.subtree), &(yyvsp[(1) - (3)].subtree), 0);
        tree_extend(&(yyval.subtree), &(yyvsp[(3) - (3)].subtree), 0);
        ;
    } break;

    case 92:

/* Line 1455 of yacc.c  */
#line 1431 "src/grammar.y"
    {
        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));
        if (s->type != Function) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento llamar a una variable como "
                                "si fuera una funcion: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                       (yyvsp[(1) - (4)].symbol).name.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        Vec children = tree_get_childs(&(yyvsp[(3) - (4)].subtree), 0);

        // Checamos que los tipos de datos de los argumentos hacen match con los
        // del simbolo
        if (s->info.fun.args.len != children.len) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff,
                     "Error: Se intento llamar a una funcion con una cantidad "
                     "de argumentos distinta a la declarada: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                       (yyvsp[(1) - (4)].symbol).name.len);
            str_push(&wrn_buff, ", se esperaban ");
            str_push_sizet(&wrn_buff, s->info.fun.args.len);
            str_push(&wrn_buff, " argumentos y se pasaron ");
            str_push_sizet(&wrn_buff, children.len);
            yyerror(str_as_ref(&wrn_buff));
        }

        if (children.len > 0) {
            Node *void_root =
                (Node *)vec_get(&(yyvsp[(3) - (4)].subtree).values, 0);

            if (void_root->node_type != NVoid) {
                str_clear(&wrn_buff);
                str_push(&wrn_buff, "Error: Se intento llamar a una funcion "
                                    "con argumentos sin pasarle argumentos: ");
                str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                           (yyvsp[(1) - (4)].symbol).name.len);
                yyerror(str_as_ref(&wrn_buff));
            }

            for (size_t n_arg = 0; n_arg < children.len; n_arg++) {
                Node *arg = tree_get_node(&(yyvsp[(3) - (4)].subtree),
                                          *(size_t *)vec_get(&children, n_arg));
                Symbol *arg_sym = (Symbol *)vec_get(&s->info.fun.args, n_arg);

                if (arg->asoc_type != arg_sym->asoc_type.type) {
                    str_clear(&wrn_buff);
                    str_push(
                        &wrn_buff,
                        "Error: Se intento llamar a una funcion con argumentos "
                        "de tipos distintos a los declarados: ");
                    str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                               (yyvsp[(1) - (4)].symbol).name.len);
                    str_push(&wrn_buff, ", el argumento ");
                    str_push_sizet(&wrn_buff, n_arg + 1);
                    str_push(&wrn_buff, " se esperaba de tipo ");
                    str_push(&wrn_buff, data_type_e_display_return(
                                            arg_sym->asoc_type.type));
                    str_push(&wrn_buff, " y se paso de tipo ");
                    str_push(&wrn_buff,
                             data_type_e_display_return(arg->asoc_type));
                    yyerror(str_as_ref(&wrn_buff));
                }
            }

            vec_drop(&children);
        }

        (yyval.function_call) =
            (FunctionCall){.symbol = (yyvsp[(1) - (4)].symbol),
                           .args = (yyvsp[(3) - (4)].subtree),
                           .return_type = s->asoc_type.type};
        ;
    } break;

    case 93:

/* Line 1455 of yacc.c  */
#line 1492 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        assert_sym_exists(&(yyvsp[(1) - (1)].symbol));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){
                            .type = ESymbol,
                            .value.symbol = (yyvsp[(1) - (1)].symbol),
                        },
                    .asoc_type = (yyvsp[(1) - (1)].symbol).asoc_type.type};
        ;
    } break;

    case 94:

/* Line 1455 of yacc.c  */
#line 1507 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Symbol *s = assert_sym_exists(&(yyvsp[(1) - (4)].symbol));

        size_t arr_size = s->asoc_type.size;
        if ((int64_t)arr_size < (yyvsp[(3) - (4)].snum) ||
            (yyvsp[(3) - (4)].snum) < 0) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Indice fuera de rango: ");
            str_push_n(&wrn_buff, (yyvsp[(1) - (4)].symbol).name.ptr,
                       (yyvsp[(1) - (4)].symbol).name.len);
            str_push(&wrn_buff, ", el arreglo tiene un tamaño de ");
            str_push_sizet(&wrn_buff, arr_size);
            str_push(&wrn_buff, " y se intento acceder a la posicion ");
            str_push_sizet(&wrn_buff, (yyvsp[(3) - (4)].snum));
            yyerror(str_as_ref(&wrn_buff));
        }

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){.type = ESymbolIdx,
                                   .value.symbol_idx =
                                       (IndexedSymbol){
                                           .symbol = (yyvsp[(1) - (4)].symbol),
                                           .index = (yyvsp[(3) - (4)].snum)}},
                    .asoc_type = s->asoc_type.type};
        ;
    } break;

    case 95:

/* Line 1455 of yacc.c  */
#line 1537 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){
            .node_type = NExpr,
            .value.expr =
                (ExprNode){
                    .type = EFunctionCall,
                    .value.function_call = (yyvsp[(1) - (1)].function_call),
                },
            .asoc_type = (yyvsp[(1) - (1)].function_call).return_type};

        if ((yyvsp[(1) - (1)].function_call).return_type == Void) {
            str_clear(&wrn_buff);
            str_push(&wrn_buff, "Error: Se intento usar una funcion que "
                                "devuelve () como expresion: ");
            str_push_n(&wrn_buff,
                       (yyvsp[(1) - (1)].function_call).symbol.name.ptr,
                       (yyvsp[(1) - (1)].function_call).symbol.name.len);
            yyerror(str_as_ref(&wrn_buff));
        };
    } break;

    case 96:

/* Line 1455 of yacc.c  */
#line 1557 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){
                            .type = EIntValue,
                            .value.int_value = (yyvsp[(1) - (1)].snum),
                        },
                    .asoc_type = Int};
        ;
    } break;

    case 97:

/* Line 1455 of yacc.c  */
#line 1570 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .value.expr =
                        (ExprNode){
                            .type = ERealValue,
                            .value.real_value = (yyvsp[(1) - (1)].fnum),
                        },
                    .asoc_type = Real};
        ;
    } break;

    case 98:

/* Line 1455 of yacc.c  */
#line 1583 "src/grammar.y"
    {
        tree_init(&(yyval.subtree), sizeof(Node));

        Node *past_root = tree_get_root(&(yyvsp[(2) - (2)].subtree));

        Node *n = (Node *)tree_new_node(&(yyval.subtree), NULL);
        *n = (Node){.node_type = NExpr,
                    .asoc_type = past_root->asoc_type,
                    .value.expr = (ExprNode){
                        .type = EUnaryOp,
                        .value.op = (yyvsp[(1) - (2)].op),
                    }};

        tree_root_extend(&(yyval.subtree), &(yyvsp[(2) - (2)].subtree));
        ;
    } break;

    case 99:

/* Line 1455 of yacc.c  */
#line 1600 "src/grammar.y"
    {
        (yyval.subtree) = (yyvsp[(2) - (3)].subtree);
        ;
    } break;

/* Line 1455 of yacc.c  */
#line 3577 "grammar.tab.c"
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

/* Line 1675 of yacc.c  */
#line 1603 "src/grammar.y"
