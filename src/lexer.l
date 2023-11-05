%{
 #include <stdlib.h>
 #include <stdio.h>
 #include "parser.h"
%}

%option prefix="yy"
%option noyywrap
%option nounput
%option noinput
%option caseless

LETRA [a-zA-Z]
DIGIT [0-9]
NZDIGIT [1-9]
SPACE [ \t\f] 
SIGNO [+-]?
ENTERO {NZDIGIT}{DIGIT}*
CALFANUM {LETRA}|{DIGIT}|"$"|"&"|"/"|"+"|"-"|"*"|"%"|"="|":"|"{"|"}"|">"|"<"|":"|";"|"["|"]"|","|"."|"#"
%%
{SPACE}+ /* Nah */
\n /* Nah */
["]{CALFANUM}*["] {
 yylval.slice = (StrSlice) { .start = yytext, .len = yyleng };
 return CONST_CADENA;
}
{SIGNO}{ENTERO} {
 yylval.snum = atoll(yytext);
 return CONST_ENTERA;
}
{SIGNO}{ENTERO}"."{ENTERO} {
 yylval.fnum = atof(yytext);
 return CONST_REAL;
}
":=" { return OP_ASIGN; }
"("|")"|";"|","|"="|":"|"["|"]"|"." { return yytext[0]; }
"of" { return KW_OF; }
"program" { return KW_PROG; }
"const" { return KW_CONST; }
"integer" { return T_INT; }
"real" { return T_REAL; }
"string" { return T_STR; }
"boolean" { return T_BOOL; }
"array" { return KW_ARRAY; }
"var" { return KW_VAR; }
"function" { return KW_FUNC; }
"procedure" { return KW_PROCEDURE; }
"begin" { return KW_BEGIN; }
"end" { return KW_END; }
"read" { return KW_READ; }
"readln" { return KW_READLN; }
"write" { return KW_WRITE; }
"writeln" { return KW_WRITELN; }
"while" { return KW_WHILE; }
"for" { return KW_FOR; }
"do" { return KW_DO; }
"to" { return KW_TO; }
"downto" { return KW_DOWNTO; }
"if" { return KW_IF; }
"then" { return KW_THEN; }
"else" { return KW_ELSE; }
"and" { return RELOP_AND; }
"not" { return RELOP_NOT; }
"or" { return RELOP_OR; }
"+" {
 yylval.addop = Add;
 return ADDOP;
}
"-" {
 yylval.addop = Sub;
 return ADDOP;
}
"/"|"div" {
 yylval.mulop = Div;
 return MULOP;
}
"*" {
 yylval.mulop = Mul;
 return MULOP;
}
"mod" {
 yylval.mulop = Mod;
 return MULOP;
}
{LETRA}({LETRA}|{DIGIT})* {
 yylval.slice = (StrSlice) { .start = yytext, .len = yyleng };
 return IDENT;
}
%%

