%{
 #include <stdlib.h>
 #include <stdio.h>
 #include "parser.h"
 #include "str.h"

 size_t comentarios_abiertos = 0;
 // Su scanner tiene un contador tanto de lineas como de columnasº
 // Lo hace referenciando con la palabra extern.

 // Tiene un buffer donde va poniendo la cadena que encuentra, y lo manda al arbol sintactico, asi no necesitamos union

 // Le funciono mejor la tabla de simbolos con un programa de 40k+ lineas,
 // si quita la lista de colisiones la maneja usando un arreglo de apuntadores
 size_t linea = 1;
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
ENTERO {NZDIGIT}{DIGIT}*
CALFANUM {LETRA}|{DIGIT}|"$"|"&"|"/"|"+"|"-"|"*"|"%"|"="|":"|"{"|"}"|">"|"<"|":"|";"|"["|"]"|","|"."|"#"
%%
<COMENTARIO>[^\(\*\n]* { nchar += 1; }
<COMENTARIO>\n { yyleng=2; yytext = "\\n"; linea++; nchar = 1; }
<INITIAL,COMENTARIO>"(*" {
    nchar += yyleng;
    if (++comentarios_abiertos == 1) {
        BEGIN(COMENTARIO); 
        // printf("   Comentario: %s", yytext); 
    } else {
        // ECHO;
    }
}
<COMENTARIO>"*)"  { 
    nchar += yyleng;
    if (--comentarios_abiertos == 0) {
        BEGIN(INITIAL); /*printf("%s\n", yytext); */
    } else {
        // ECHO;
    }
}
{SPACE}+ { nchar += yyleng; }/* Nah */
\n { linea++; nchar = 1; }
["]{CALFANUM}*["] {
 nchar += yyleng;
 yylval.slice = (StrSlice) { .ptr = yytext, .len = yyleng };
 return CONST_CADENA;
}
{SIGNO}{ENTERO} {
 nchar += yyleng;
 yylval.snum = atoll(yytext);
 return CONST_ENTERA;
}
{SIGNO}{ENTERO}"."{ENTERO} {
 nchar += yyleng;
 yylval.fnum = atof(yytext);
 return CONST_REAL;
}
":=" { nchar += yyleng;  return OP_ASIGN; }
"("|")"|";"|","|"="|":"|"["|"]"|"." { nchar += yyleng; return yytext[0]; }
"of" { nchar += yyleng;  return KW_OF; }
"program" { nchar += yyleng; return KW_PROG; }
"const" { nchar += yyleng; return KW_CONST; }
"integer" { nchar += yyleng; return T_INT; }
"real" { nchar += yyleng; return T_REAL; }
"string" { nchar += yyleng; return T_STR; }
"boolean" { nchar += yyleng; return T_BOOL; }
"array" { nchar += yyleng; return KW_ARRAY; }
"var" { nchar += yyleng; return KW_VAR; }
"function" { nchar += yyleng; return KW_FUNC; }
"procedure" { nchar += yyleng; return KW_PROCEDURE; }
"begin" { nchar += yyleng; return KW_BEGIN; }
"end" { nchar += yyleng; return KW_END; }
"read" { nchar += yyleng; return KW_READ; }
"readln" { nchar += yyleng; return KW_READLN; }
"write" { nchar += yyleng; return KW_WRITE; }
"writeln" { nchar += yyleng; return KW_WRITELN; }
"while" { nchar += yyleng; return KW_WHILE; }
"for" { nchar += yyleng; return KW_FOR; }
"do" { nchar += yyleng; return KW_DO; }
"to" { nchar += yyleng; return KW_TO; }
"downto" { nchar += yyleng; return KW_DOWNTO; }
"if" { nchar += yyleng; return KW_IF; }
".." { nchar += yyleng; return KW_DOTS; }
"then" { nchar += yyleng; return KW_THEN; }
"else" { nchar += yyleng; return KW_ELSE; }
"and" { nchar += yyleng; return RELOP_AND; }
"not" { nchar += yyleng; return RELOP_NOT; }
"or" { nchar += yyleng; return RELOP_OR; }
">" { nchar += yyleng; return RELOP_BT; }
"<" { nchar += yyleng; return RELOP_LT; }
">=" { nchar += yyleng; return RELOP_EBT; }
"<=" { nchar += yyleng; return RELOP_ELT; }
"==" { nchar += yyleng; return RELOP_EQ; }
"!=" { nchar += yyleng; return RELOP_NEQ; }
"+" { yylval.addop = Add; return ADDOP; }
"-" { yylval.addop = Sub; return ADDOP; }
"/"|"div" { yylval.mulop = Div; return MULOP; }
"*" { yylval.mulop = Mul; return MULOP; }
"mod" { yylval.mulop = Mod; return MULOP; }
{LETRA}({LETRA}|{DIGIT})* {
 StrSlice name = (StrSlice) { .ptr = yytext, .len = yyleng };
 yylval.symbol = (Symbol) { 
   .name = name, 
   .type = Ukw, 
   .info = (SymbolInfo) {}, 
   .scope = scope, 
   .line = linea, 
   .nchar = nchar, 
   .refs = vec_new(sizeof(size_t)) 
 }; 
 nchar += yyleng;
 return IDENT;
}
%%

