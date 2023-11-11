#include "hashset.h"
#include "parser.h"
#include "str.h"
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
extern size_t linea;

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
        char *path = argv[0];
        printf("Ejecutado archivo: %s\n", path);
        parse_file(path);
    } else
        printf("The lng compiler, translate PASCAL to C code\n\nUsage: lng "
               "<FILE>\n");

    size_t i = 0;
    puts("Contenidos de la tabla:");
    while (tabla.elements > i) {
        for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
            Vec *arr = (Vec *)vec_get(&tabla.values, j);
            for (size_t h = 0; h < arr->len; h++) {
                Symbol *s = (Symbol *)vec_get(arr, h);
                printf(" - Simbolo (%zu,%zu) %zu(%zu): %.*s\n", j, h, s->line,
                       s->scope, (int)s->name.len, s->name.ptr);
                i += 1;
            }
        }
    }

    // Al final liberamos la tabla de hashes de memoria
    hashset_drop(&tabla);
    str_drop(&wrn_buff);

    if (!err)
        printf("Linea reconocida correctamente\n");
    return err;
}

void yyerror(char *s) {
    err = 1;
    fprintf(stderr, "Line %zu: %s\n", linea, s);
}
