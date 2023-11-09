#include "hashset.h"
#include "str.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int err = 0;

extern FILE *yyin;
extern int yyparse(void);
extern HashSet tabla;
extern String wrn_buff;

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

    if (!err)
        printf("Linea reconocida correctamente\n");

    fclose(f);
}

int parse(char *code) {
    String buffer = str_new();
    StrSlice sl = str_slice_from_cstr(code, strlen(code));
    StrIter it = str_iter_from_slice(&sl, "\n");

    while (!it.ended) {
        StrSlice next = str_iter_next(&it);

        str_clear(&buffer);
        str_push_slice(&buffer, &next);

        printf("Line: %s\n", str_as_ref(&buffer));
    }

    str_drop(&buffer);
    return 0;
}

int main(int argc, char *argv[]) {
    ++argv;
    --argc;

    switch (argc) {
    case 1: {
        char *path = argv[0];
        printf("Ejecutado archivo: %s\n", path);
        parse_file(path);
        break;
    }
    default: {
        puts("Error,usage:\n    lng <FILE>");
        break;
    }
    }

    // Al final liberamos la tabla de hashes de memoria
    hashset_drop(&tabla);
    str_drop(&wrn_buff);

    return err;
}

void yyerror(char *s) {
    err = 1;
    fprintf(stderr, "%s\n", s);
}
