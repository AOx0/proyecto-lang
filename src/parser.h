/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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

    struct Symbol {
        StrSlice name;
        size_t scope;
        size_t line;
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


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENT = 258,                   /* IDENT  */
    CONST_REAL = 259,              /* CONST_REAL  */
    CONST_ENTERA = 260,            /* CONST_ENTERA  */
    CONST_CADENA = 261,            /* CONST_CADENA  */
    RELOP = 262,                   /* RELOP  */
    ADDOP = 263,                   /* ADDOP  */
    MULOP = 264,                   /* MULOP  */
    OP_ASIGN = 265,                /* OP_ASIGN  */
    KW_PROCEDURE = 266,            /* KW_PROCEDURE  */
    KW_PROG = 267,                 /* KW_PROG  */
    KW_CONST = 268,                /* KW_CONST  */
    KW_VAR = 269,                  /* KW_VAR  */
    KW_ARRAY = 270,                /* KW_ARRAY  */
    KW_OF = 271,                   /* KW_OF  */
    KW_FUNC = 272,                 /* KW_FUNC  */
    KW_BEGIN = 273,                /* KW_BEGIN  */
    KW_END = 274,                  /* KW_END  */
    KW_READ = 275,                 /* KW_READ  */
    KW_READLN = 276,               /* KW_READLN  */
    KW_WRITE = 277,                /* KW_WRITE  */
    KW_WRITELN = 278,              /* KW_WRITELN  */
    KW_WHILE = 279,                /* KW_WHILE  */
    KW_FOR = 280,                  /* KW_FOR  */
    KW_DO = 281,                   /* KW_DO  */
    KW_TO = 282,                   /* KW_TO  */
    KW_DOWNTO = 283,               /* KW_DOWNTO  */
    KW_IF = 284,                   /* KW_IF  */
    KW_THEN = 285,                 /* KW_THEN  */
    KW_ELSE = 286,                 /* KW_ELSE  */
    RELOP_BT = 287,                /* RELOP_BT  */
    RELOP_LT = 288,                /* RELOP_LT  */
    RELOP_EBT = 289,               /* RELOP_EBT  */
    RELOP_ELT = 290,               /* RELOP_ELT  */
    RELOP_AND = 291,               /* RELOP_AND  */
    RELOP_NOT = 292,               /* RELOP_NOT  */
    RELOP_OR = 293,                /* RELOP_OR  */
    T_INT = 294,                   /* T_INT  */
    T_REAL = 295,                  /* T_REAL  */
    T_STR = 296,                   /* T_STR  */
    T_BOOL = 297                   /* T_BOOL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

    int64_t snum;
    uint64_t unum;
    double fnum;
    char * ident;
    StrSlice slice;
    Vec idents;
    RelOp relop;
    AddOp addop;
    MulOp mulop;


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
