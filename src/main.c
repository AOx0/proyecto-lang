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
FILE *OUT_FILE;

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
        OUT_FILE = stdout;
        path = argv[0];
        parse_file(path);
    } else if (argc == 2) {
        path = argv[0];
        FILE *f = fopen(argv[1], "w+");
        if (f == NULL) {
            puts("Err: al abrir el archivo!!");
            exit(1);
        }
        OUT_FILE = f;
        parse_file(path);
    } else {
        printf("The lng compiler, translate PASCAL to C code\n\nUsage: lng "
               "<FILE>\n");
        exit(0);
    }

    if (!err)
        printf("Successfull compilation\n");
    else
        printf("There was one or more errors\n");
    return err;
}

void yyerror(char *s) {
    err = 1;
    fprintf(stderr, "Error %s:%zu:%zu: %s\n", path, line, nchar, s);
}
