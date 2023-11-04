%{
 #include <stdlib.h>
 #include <stdio.h>
 #include "parser.h"
%}

%option prefix="yy"
%option noyywrap
%option nounput
%option noinput

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
{LETRA}({LETRA}|{DIGIT})* {
 yylval.slice = (StrSlice) { .start = yytext, .len = yyleng };
 return IDENT;
}
%%

