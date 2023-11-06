#include "hashset.h"
#include "str.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int err = 0;

extern FILE *yyin;
extern int yyparse(void);

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

#ifdef WIN
        FILE *stream = tmpfile();

        if (stream == NULL) {
            perror("tmpfile");
            return;
        } else {
            fwrite(str_as_ref(&inp), 1, inp.len, stream);
            rewind(stream);
            yyin = stream;
            yyparse();
        }
#else
        FILE *stream = fmemopen(str_as_ref(&inp), inp.len, "r");

        if (stream == NULL) {
            perror("fmemopen");
            return;
        } else {
            yyin = stream;
            yyparse();
        }
#endif

        fclose(stream);
        str_clear(&inp);

        if (!err)
            printf("Linea reconocida correctamente\n");
        else
            err = 0;
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

    return err;
}

void yyerror(char *s) {
    err = 1;
    fprintf(stderr, "Error: %s\n", s);
}
