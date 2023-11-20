#include "hashset.h"
#include "parser.h"
#include "str.h"
#include "symbol.h"
#include "vector.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int err = 0;

extern FILE *yyin;
extern int yyparse(void);
extern HashSet tabla;
extern String wrn_buff;
extern size_t line;
extern size_t nchar;
char *path;

int main(int argc, char *argv[]);
void parse_file(char *path);
int parse(char *code);

void parse_file(char *path) {
    FILE *f = fopen(path, "r");

    if (f == NULL) {
        puts("Err: al abrir el archivo!!");
        exit(1);
    }

    yyin = f;
    yyparse();

    fclose(f);
}

int main(int argc, char *argv[]) {
    ++argv;
    --argc;

    if (argc == 1) {
        path = argv[0];
        printf("Traduciendo archivo: %s\n", path);
        parse_file(path);
    } else {
        printf("The lng compiler, translate PASCAL to C code\n\nUsage: lng "
               "<FILE>\n");
        exit(0);
    }

    if (!err)
        printf("Linea reconocida correctamente\n");
    return err;
}

void yyerror(char *s) {
    err = 1;
    fprintf(stderr, "Error %s:%zu:%zu: %s\n", path, line, nchar, s);
}
