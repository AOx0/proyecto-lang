/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

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
	#include "symbol.h"

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





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENT = 3,                      /* IDENT  */
  YYSYMBOL_CONST_REAL = 4,                 /* CONST_REAL  */
  YYSYMBOL_CONST_ENTERA = 5,               /* CONST_ENTERA  */
  YYSYMBOL_CONST_CADENA = 6,               /* CONST_CADENA  */
  YYSYMBOL_RELOP = 7,                      /* RELOP  */
  YYSYMBOL_ADDOP = 8,                      /* ADDOP  */
  YYSYMBOL_MULOP = 9,                      /* MULOP  */
  YYSYMBOL_OP_ASIGN = 10,                  /* OP_ASIGN  */
  YYSYMBOL_KW_PROCEDURE = 11,              /* KW_PROCEDURE  */
  YYSYMBOL_KW_PROG = 12,                   /* KW_PROG  */
  YYSYMBOL_KW_CONST = 13,                  /* KW_CONST  */
  YYSYMBOL_KW_VAR = 14,                    /* KW_VAR  */
  YYSYMBOL_KW_ARRAY = 15,                  /* KW_ARRAY  */
  YYSYMBOL_KW_OF = 16,                     /* KW_OF  */
  YYSYMBOL_KW_FUNC = 17,                   /* KW_FUNC  */
  YYSYMBOL_KW_BEGIN = 18,                  /* KW_BEGIN  */
  YYSYMBOL_KW_END = 19,                    /* KW_END  */
  YYSYMBOL_KW_READ = 20,                   /* KW_READ  */
  YYSYMBOL_KW_READLN = 21,                 /* KW_READLN  */
  YYSYMBOL_KW_WRITE = 22,                  /* KW_WRITE  */
  YYSYMBOL_KW_WRITELN = 23,                /* KW_WRITELN  */
  YYSYMBOL_KW_WHILE = 24,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 25,                    /* KW_FOR  */
  YYSYMBOL_KW_DO = 26,                     /* KW_DO  */
  YYSYMBOL_KW_TO = 27,                     /* KW_TO  */
  YYSYMBOL_KW_DOWNTO = 28,                 /* KW_DOWNTO  */
  YYSYMBOL_KW_IF = 29,                     /* KW_IF  */
  YYSYMBOL_KW_THEN = 30,                   /* KW_THEN  */
  YYSYMBOL_KW_ELSE = 31,                   /* KW_ELSE  */
  YYSYMBOL_RELOP_EQ = 32,                  /* RELOP_EQ  */
  YYSYMBOL_RELOP_NEQ = 33,                 /* RELOP_NEQ  */
  YYSYMBOL_RELOP_BT = 34,                  /* RELOP_BT  */
  YYSYMBOL_RELOP_LT = 35,                  /* RELOP_LT  */
  YYSYMBOL_RELOP_EBT = 36,                 /* RELOP_EBT  */
  YYSYMBOL_RELOP_ELT = 37,                 /* RELOP_ELT  */
  YYSYMBOL_RELOP_AND = 38,                 /* RELOP_AND  */
  YYSYMBOL_RELOP_NOT = 39,                 /* RELOP_NOT  */
  YYSYMBOL_RELOP_OR = 40,                  /* RELOP_OR  */
  YYSYMBOL_T_INT = 41,                     /* T_INT  */
  YYSYMBOL_T_REAL = 42,                    /* T_REAL  */
  YYSYMBOL_T_STR = 43,                     /* T_STR  */
  YYSYMBOL_T_BOOL = 44,                    /* T_BOOL  */
  YYSYMBOL_45_ = 45,                       /* '('  */
  YYSYMBOL_46_ = 46,                       /* ')'  */
  YYSYMBOL_47_ = 47,                       /* ';'  */
  YYSYMBOL_48_ = 48,                       /* '.'  */
  YYSYMBOL_49_ = 49,                       /* ','  */
  YYSYMBOL_50_ = 50,                       /* ':'  */
  YYSYMBOL_51_ = 51,                       /* '='  */
  YYSYMBOL_52_ = 52,                       /* '['  */
  YYSYMBOL_53_ = 53,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 54,                  /* $accept  */
  YYSYMBOL_programa = 55,                  /* programa  */
  YYSYMBOL_56_1 = 56,                      /* $@1  */
  YYSYMBOL_ident_lista = 57,               /* ident_lista  */
  YYSYMBOL_decl = 58,                      /* decl  */
  YYSYMBOL_decl_var = 59,                  /* decl_var  */
  YYSYMBOL_decl_const = 60,                /* decl_const  */
  YYSYMBOL_tipo = 61,                      /* tipo  */
  YYSYMBOL_estandard_tipo = 62,            /* estandard_tipo  */
  YYSYMBOL_subprograma_decl = 63,          /* subprograma_decl  */
  YYSYMBOL_subprograma_declaracion = 64,   /* subprograma_declaracion  */
  YYSYMBOL_subprograma_encabezado = 65,    /* subprograma_encabezado  */
  YYSYMBOL_66_2 = 66,                      /* $@2  */
  YYSYMBOL_67_3 = 67,                      /* $@3  */
  YYSYMBOL_argumentos = 68,                /* argumentos  */
  YYSYMBOL_parametros_lista = 69,          /* parametros_lista  */
  YYSYMBOL_instruccion_compuesta = 70,     /* instruccion_compuesta  */
  YYSYMBOL_instrucciones_opcionales = 71,  /* instrucciones_opcionales  */
  YYSYMBOL_instrucciones_lista = 72,       /* instrucciones_lista  */
  YYSYMBOL_instrucciones = 73,             /* instrucciones  */
  YYSYMBOL_repeticion_instruccion = 74,    /* repeticion_instruccion  */
  YYSYMBOL_lectura_instruccion = 75,       /* lectura_instruccion  */
  YYSYMBOL_escritura_instruccion = 76,     /* escritura_instruccion  */
  YYSYMBOL_if_instruccion = 77,            /* if_instruccion  */
  YYSYMBOL_variable_asignacion = 78,       /* variable_asignacion  */
  YYSYMBOL_for_asignacion = 79,            /* for_asignacion  */
  YYSYMBOL_variable = 80,                  /* variable  */
  YYSYMBOL_procedure_instruccion = 81,     /* procedure_instruccion  */
  YYSYMBOL_relop_expresion = 82,           /* relop_expresion  */
  YYSYMBOL_relop_and = 83,                 /* relop_and  */
  YYSYMBOL_relop_not = 84,                 /* relop_not  */
  YYSYMBOL_relop_paren = 85,               /* relop_paren  */
  YYSYMBOL_relop_expresion_simple = 86,    /* relop_expresion_simple  */
  YYSYMBOL_relop = 87,                     /* relop  */
  YYSYMBOL_expresion_lista = 88,           /* expresion_lista  */
  YYSYMBOL_expresion = 89,                 /* expresion  */
  YYSYMBOL_termino = 90,                   /* termino  */
  YYSYMBOL_llamado_funcion = 91,           /* llamado_funcion  */
  YYSYMBOL_factor = 92                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
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
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  211

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   152,   152,   152,   171,   177,   185,   185,   185,   187,
     200,   207,   214,   223,   223,   224,   224,   224,   224,   227,
     227,   228,   231,   231,   242,   242,   255,   266,   267,   270,
     278,   279,   279,   280,   280,   281,   281,   281,   282,   282,
     282,   282,   284,   285,   286,   288,   292,   296,   300,   304,
     304,   305,   305,   306,   312,   318,   322,   326,   330,   334,
     335,   338,   339,   339,   340,   344,   348,   353,   360,   360,
     361,   361,   362,   362,   363,   363,   364,   365,   365,   365,
     365,   365,   365,   365,   365,   368,   368,   369,   369,   370,
     370,   371,   376,   376,   376,   376,   376,   376,   376
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENT", "CONST_REAL",
  "CONST_ENTERA", "CONST_CADENA", "RELOP", "ADDOP", "MULOP", "OP_ASIGN",
  "KW_PROCEDURE", "KW_PROG", "KW_CONST", "KW_VAR", "KW_ARRAY", "KW_OF",
  "KW_FUNC", "KW_BEGIN", "KW_END", "KW_READ", "KW_READLN", "KW_WRITE",
  "KW_WRITELN", "KW_WHILE", "KW_FOR", "KW_DO", "KW_TO", "KW_DOWNTO",
  "KW_IF", "KW_THEN", "KW_ELSE", "RELOP_EQ", "RELOP_NEQ", "RELOP_BT",
  "RELOP_LT", "RELOP_EBT", "RELOP_ELT", "RELOP_AND", "RELOP_NOT",
  "RELOP_OR", "T_INT", "T_REAL", "T_STR", "T_BOOL", "'('", "')'", "';'",
  "'.'", "','", "':'", "'='", "'['", "']'", "$accept", "programa", "$@1",
  "ident_lista", "decl", "decl_var", "decl_const", "tipo",
  "estandard_tipo", "subprograma_decl", "subprograma_declaracion",
  "subprograma_encabezado", "$@2", "$@3", "argumentos", "parametros_lista",
  "instruccion_compuesta", "instrucciones_opcionales",
  "instrucciones_lista", "instrucciones", "repeticion_instruccion",
  "lectura_instruccion", "escritura_instruccion", "if_instruccion",
  "variable_asignacion", "for_asignacion", "variable",
  "procedure_instruccion", "relop_expresion", "relop_and", "relop_not",
  "relop_paren", "relop_expresion_simple", "relop", "expresion_lista",
  "expresion", "termino", "llamado_funcion", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-162)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-65)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
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

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
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

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -162,  -162,  -162,    -9,   196,  -162,  -162,  -161,  -137,   177,
    -162,  -162,  -162,  -162,   159,  -162,   -14,  -162,  -162,   -91,
    -162,  -162,  -162,  -162,   187,  -162,   188,  -162,    37,   106,
     -68,  -162,  -162,  -162,   117,   -63,   107,  -162,   -69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     8,    13,    14,    15,    61,    62,    18,
      24,    25,    64,    63,   101,   141,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    88,    48,    49,    77,    78,
      79,    80,    81,   131,   104,    82,    83,    84,    85
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
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

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
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

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
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


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_ident_lista: /* ident_lista  */
            { 
    printf("Dropping ident_lista:  ");
    vec_debug_verbose(&((*yyvaluep).idents));
    vec_drop(&((*yyvaluep).idents));
}
        break;

    case YYSYMBOL_parametros_lista: /* parametros_lista  */
            { 
    printf("Dropping parametros_lista:  ");
    vec_debug_verbose(&((*yyvaluep).idents));
    vec_drop(&((*yyvaluep).idents));
}
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
          { 
    puts("Init symbol table");
    hashset_init(&tabla, sizeof(Symbol), hash_symbol); 
    puts("Init warning buffer");
    str_init(&wrn_buff);
}
    break;

  case 3: /* programa: $@1 KW_PROG IDENT '(' ident_lista ')' ';' decl subprograma_decl instruccion_compuesta '.'  */
                                                                                         {
    printf("Programa: %.*s\n", (int)(yyvsp[-8].symbol).name.len, (yyvsp[-8].symbol).name.ptr);
    printf("Entradas: %zu\n", (yyvsp[-6].idents).len);
    for (size_t i=0; i < (yyvsp[-6].idents).len; i++) {
        Symbol * s = vec_get(&(yyvsp[-6].idents), i);
        assert_not_sym_exists(s);
        s->type = Variable;
        hashset_insert(&tabla, s);
        printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[-6].idents));
}
    break;

  case 4: /* ident_lista: IDENT ',' ident_lista  */
                                   {
    (yyval.idents) = (yyvsp[0].idents);
    // vec_debug_verbose(&$$);
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[-2].symbol);
}
    break;

  case 5: /* ident_lista: IDENT  */
                   {
    (yyval.idents) = vec_new(sizeof(Symbol));
    // vec_debug_verbose(&$$);
    Symbol *s = vec_push(&(yyval.idents));
    *s = (yyvsp[0].symbol);
}
    break;

  case 9: /* decl_var: decl KW_VAR ident_lista ':' tipo ';'  */
                                               {
    printf("Declarando variables: %zu\n", (yyvsp[-3].idents).len);
    for (size_t i=0; i < (yyvsp[-3].idents).len; i++) {
        Symbol * s = vec_get(&(yyvsp[-3].idents), i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
        printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[-3].idents));
}
    break;

  case 10: /* decl_const: decl KW_CONST IDENT '=' CONST_ENTERA ';'  */
                                                     {
    printf("Declarando constante: %lld\n", (yyvsp[-1].snum));
    Symbol s = (yyvsp[-3].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 11: /* decl_const: decl KW_CONST IDENT '=' CONST_REAL ';'  */
                                                   {
    printf("Declarando constante: %f\n", (yyvsp[-1].fnum));
    Symbol s = (yyvsp[-3].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 12: /* decl_const: decl KW_CONST IDENT '=' CONST_CADENA ';'  */
                                                     {
    printf("Declarando constante: %.*s\n", (yyvsp[-1].slice).len, (yyvsp[-1].slice).ptr);
    Symbol s = (yyvsp[-3].symbol);
    s.type = Constant;
    assert_not_sym_exists(&s);
    hashset_insert(&tabla, &s);
}
    break;

  case 21: /* subprograma_declaracion: subprograma_encabezado decl subprograma_decl instruccion_compuesta  */
                                                                                            {
    scope-=fun_id;
}
    break;

  case 22: /* $@2: %empty  */
                                      {
    printf("Declarando funcion %.*s\n", (int)(yyvsp[0].symbol).name.len, (yyvsp[0].symbol).name.ptr);
	Symbol s = (yyvsp[0].symbol);
    s.type = Function;
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
    fun_id++;
	scope+=fun_id;
}
    break;

  case 23: /* subprograma_encabezado: KW_FUNC IDENT $@2 argumentos ':' estandard_tipo ';'  */
                                    {
    printf("Declarada %.*s\n", (int)(yyvsp[-5].symbol).name.len, (yyvsp[-5].symbol).name.ptr);
}
    break;

  case 24: /* $@3: %empty  */
                                           {
    printf("Declarando procedure %.*s\n", (int)(yyvsp[0].symbol).name.len, (yyvsp[0].symbol).name.ptr);
	Symbol s = (yyvsp[0].symbol);
    s.type = Procedure;
	assert_not_sym_exists(&s);
	hashset_insert(&tabla, &s);
    fun_id++;
	scope+=fun_id;
}
    break;

  case 25: /* subprograma_encabezado: KW_PROCEDURE IDENT $@3 argumentos ';'  */
                 {
    printf("Declarada %.*s\n", (int)(yyvsp[-3].symbol).name.len, (yyvsp[-3].symbol).name.ptr);
}
    break;

  case 26: /* argumentos: '(' parametros_lista ')'  */
                                     {
    printf("Argumentos: %zu\n", (yyvsp[-1].idents).len);
    for (size_t i=0; i < (yyvsp[-1].idents).len; i++) {
        Symbol * s = (Symbol *)vec_get(&(yyvsp[-1].idents), i);
        s->type = Variable;
        assert_not_sym_exists(s);
        hashset_insert(&tabla, s);
		printf("    - %.*s\n", (int)s->name.len, s->name.ptr);
    }
	
	vec_drop(&(yyvsp[-1].idents));
}
    break;

  case 28: /* parametros_lista: ident_lista ':' tipo  */
                                       {
    (yyval.idents) = (yyvsp[-2].idents);
}
    break;

  case 29: /* parametros_lista: parametros_lista ';' ident_lista ':' tipo  */
                                                            {
    (yyval.idents) = (yyvsp[-4].idents);
    vec_extend(&(yyval.idents), &(yyvsp[-2].idents));
    vec_drop(&(yyvsp[-2].idents));  
}
    break;

  case 45: /* lectura_instruccion: KW_READ '(' IDENT ')'  */
                                           {
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 46: /* lectura_instruccion: KW_READLN '(' IDENT ')'  */
                                             { 
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 47: /* escritura_instruccion: KW_WRITE '(' CONST_CADENA ',' IDENT ')'  */
                                                               {
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 48: /* escritura_instruccion: KW_WRITELN '(' CONST_CADENA ',' IDENT ')'  */
                                                                 {
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 53: /* escritura_instruccion: KW_WRITE '(' IDENT ',' IDENT ')'  */
                                                        {
    Symbol s = (yyvsp[-3].symbol);
    Symbol s1 = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
    assert_sym_exists(&s1);
}
    break;

  case 54: /* escritura_instruccion: KW_WRITELN '(' IDENT ',' IDENT ')'  */
                                                          {
    Symbol s = (yyvsp[-3].symbol);
    Symbol s1 = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
    assert_sym_exists(&s1);
}
    break;

  case 55: /* escritura_instruccion: KW_WRITE '(' IDENT ')'  */
                                               {
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 56: /* escritura_instruccion: KW_WRITELN '(' IDENT ')'  */
                                                 {
    Symbol s = (yyvsp[-1].symbol);
    assert_sym_exists(&s);
}
    break;

  case 57: /* escritura_instruccion: KW_WRITE '(' IDENT ',' expresion ')'  */
                                                            {
    Symbol s = (yyvsp[-3].symbol);
    assert_sym_exists(&s);
}
    break;

  case 58: /* escritura_instruccion: KW_WRITELN '(' IDENT ',' expresion ')'  */
                                                              {
    Symbol s = (yyvsp[-3].symbol);
    assert_sym_exists(&s);
}
    break;

  case 64: /* variable: IDENT  */
                {
    Symbol s = (yyvsp[0].symbol);
    assert_sym_exists(&s);
}
    break;

  case 65: /* variable: IDENT '[' expresion ']'  */
                                  {
    Symbol s = (yyvsp[-3].symbol);
    assert_sym_exists(&s);
}
    break;

  case 66: /* procedure_instruccion: IDENT  */
                              {
    Symbol s = (yyvsp[0].symbol);
    printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
    assert_sym_exists(&s);
}
    break;

  case 67: /* procedure_instruccion: IDENT '(' expresion_lista ')'  */
                                                      {
    Symbol s = (yyvsp[-3].symbol);
    printf("Llamando procedimiento: %.*s\n", (int)s.name.len, s.name.ptr);
    assert_sym_exists(&s);
}
    break;

  case 91: /* llamado_funcion: IDENT '(' expresion_lista ')'  */
                                                {
    Symbol s = (yyvsp[-3].symbol);
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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}


