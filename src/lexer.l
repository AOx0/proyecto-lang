%{
    #include "parser.h"
    #include "str.h"
    #include <stdio.h>
    #include <stdlib.h>

    size_t comentarios_abiertos = 0;
    size_t line = 1;
    size_t nchar = 1;
    extern size_t scope;
%}

%option prefix="yy"
%option noyywrap
%option nounput
%option noinput
%option caseless

%x COMENTARIO

LETRA [a-zA-Z]
DIGIT [0-9]
NZDIGIT [1-9]
SPACE [ \t\f] 
SIGNO [+-]?
ENTERO 0|{NZDIGIT}{DIGIT}*
ZENTERO {DIGIT}+
CALFANUM {LETRA}|{DIGIT}|"$"|"&"|"/"|"+"|"-"|"*"|"%"|"="|":"|"{"|"}"|">"|"<"|":"|";"|"["|"]"|","|"."|"#"
%%
<INITIAL,COMENTARIO>"(*" {
    nchar += yyleng;
    if (++comentarios_abiertos == 1) BEGIN(COMENTARIO);
}
<COMENTARIO>"*)" {
    nchar += yyleng;
    if (--comentarios_abiertos == 0) BEGIN(INITIAL);
}
<COMENTARIO>[a-zA-Z0-9 \t]* { nchar += yyleng; }
<COMENTARIO>. { nchar += 1; }
<COMENTARIO>\n { line++; nchar = 1; }
{SIGNO}{ENTERO}"."{ZENTERO}(e{SIGNO}{ENTERO})? {
    nchar += yyleng;
    yylval.fnum = atof(yytext);
    return CONST_REAL;
}
{SPACE}+ {
    nchar += yyleng; }/* Nah */
\n {
    line++;
    nchar = 1; }
["]{CALFANUM}*["] {
    nchar += yyleng;
    yylval.slice = (StrSlice){.ptr = &yytext[1], .len = yyleng - 2};
    return CONST_CADENA;
}
{SIGNO}{ENTERO} {
    nchar += yyleng;
    yylval.snum = atoll(yytext);
    return CONST_ENTERA;
}
":=" {
    nchar += yyleng;
    return OP_ASIGN; }
"("|")"|";"|","|"="|":"|"["|"]"|"." {
    nchar += yyleng;
    return yytext[0]; }
"of" {
    nchar += yyleng;
    return KW_OF; }
"program" {
    nchar += yyleng;
    return KW_PROG; }
"const" {
    nchar += yyleng;
    return KW_CONST; }
"integer" {
    nchar += yyleng;
    return T_INT; }
"real" {
    nchar += yyleng;
    return T_REAL; }
"string" {
    nchar += yyleng;
    return T_STR; }
"boolean" {
    nchar += yyleng;
    return T_BOOL; }
"array" {
    nchar += yyleng;
    return KW_ARRAY; }
"var" {
    nchar += yyleng;
    return KW_VAR; }
"function" {
    nchar += yyleng;
    return KW_FUNC; }
"procedure" {
    nchar += yyleng;
    return KW_PROCEDURE; }
"begin" {
    nchar += yyleng;
    return KW_BEGIN; }
"end" {
    nchar += yyleng;
    return KW_END; }
"read" {
    nchar += yyleng;
    return KW_READ; }
"readln" {
    nchar += yyleng;
    return KW_READLN; }
"write" {
    nchar += yyleng;
    return KW_WRITE; }
"writeln" {
    nchar += yyleng;
    return KW_WRITELN; }
"while" {
    nchar += yyleng;
    return KW_WHILE; }
"for" {
    nchar += yyleng;
    return KW_FOR; }
"do" {
    nchar += yyleng;
    return KW_DO; }
"to" {
    nchar += yyleng;
    return KW_TO; }
"downto" {
    nchar += yyleng;
    return KW_DOWNTO; }
"if" {
    nchar += yyleng;
    return KW_IF; }
".." {
    nchar += yyleng;
    return KW_DOTS; }
"then" {
    nchar += yyleng;
    return KW_THEN; }
"else" {
    nchar += yyleng;
    return KW_ELSE; }
"and" {
    nchar += yyleng;
    return RELOP_AND; }
"not" {
    nchar += yyleng;
    return RELOP_NOT; }
"or" {
    nchar += yyleng;
    return RELOP_OR; }
">" {
    nchar += yyleng;
    return RELOP_BT; }
"<" {
    nchar += yyleng;
    return RELOP_LT; }
">=" {
    nchar += yyleng;
    return RELOP_EBT; }
"<=" {
    nchar += yyleng;
    return RELOP_ELT; }
"==" {
    nchar += yyleng;
    return RELOP_EQ; }
"!=" {
    nchar += yyleng;
    return RELOP_NEQ; }
"+" {
    yylval.op = OpAdd;
    return ADDOP; }
"-" {
    yylval.op = OpSub;
    return ADDOP; }
"/"|"div" {
    yylval.op = OpDiv;
    return MULOP; }
"*" {
    yylval.op = OpMul;
    return MULOP; }
"mod" {
    yylval.op = OpMod;
    return MULOP; }
{LETRA}({LETRA}|{DIGIT})* {
    StrSlice name = (StrSlice){.ptr = yytext, .len = yyleng};
    yylval.symbol = (Symbol){.name = name,
                             .type = Unknown,
                             .info.none = (Incomplete) { .f = 0 },
                             .scope = scope,
                             .line = line,
                             .nchar = nchar,
                             .refs = vec_new(sizeof(size_t)) };
    nchar += yyleng;
    return IDENT;
}
%%
