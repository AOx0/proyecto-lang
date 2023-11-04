#include "include/grammar.h"
#include "include/hashset.h"
#include "include/str.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]);
void parse_file(char *path);
int parse(char *code);

void parse_prompt(void) {
    String inp = str_new();
    while (1) {
        printf(">");

        if (str_getline(&inp) == 0)
            break;
        if (strcmp(str_as_ref(&inp), "exit\n") == 0)
            break;

        str_trim_end(&inp, '\n');
        printf("Ingreso: %s\n", str_as_ref(&inp));
        str_debug(&inp);
        str_clear(&inp);
    }

    puts("");

    str_drop(&inp);
}

void parse_file(char *path) {
    FILE *f = fopen(path, "r");

    if (f == NULL) {
        puts("Err: al abrir el archivo!!");
        exit(1);
    }

    String contents = str_new();

    if (str_fgets(&contents, 0, f) == 0) {
        puts("Wrn: No se obtuvo nada del archivo!!");
        return;
    }
     
    str_drop(&contents);
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
    case 0: {
        puts("Corriendo interprete");
        parse_prompt();
        break;
    }
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

    return 0;
}
