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
extern size_t linea;
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

    size_t i = 0;
    puts("Contenidos de la tabla:");
    while (tabla.elements > i) {
        for (size_t j = 0; j < HASH_BUFF_SIZE; j++) {
            Vec *arr = (Vec *)vec_get(&tabla.values, j);
            for (size_t h = 0; h < arr->len; h++) {
                Symbol *s = (Symbol *)vec_get(arr, h);
                printf("    - ");
                sym_type_display(s->type);
                printf(" (%.2zu,%.2zu), name: %10.*s, location: %2zu:%-2zu, "
                       "scope: %zu, refs: { ",
                       j, h, (int)s->name.len, s->name.ptr, s->line, s->nchar,
                       s->scope);
                for (size_t i = 0; i < s->refs.len; i++) {
                    size_t *ref = (size_t *)vec_get(&s->refs, i);
                    printf("%zu", *ref);
                    if (i + 1 != s->refs.len) {
                        printf(", ");
                    }
                }
                printf(" }, info: ");

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
                    printf("None");
                    break;
                }
                default: {
                    puts("Panic: Invalid SymbolType");
                    exit(1);
                }
                }

                puts("");
                i += 1;
                vec_drop(&s->refs);
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
    fprintf(stderr, "Error %s:%zu:%zu: %s\n", path, linea, nchar, s);
}
