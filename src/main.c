#include "include/grammar.h"
#include "include/hashset.h"
#include "include/str.h"
#include "include/token.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]);
void run_file(char *path);
int run(char *code);

void run_prompt(void) {
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

void run_file(char *path) {
    FILE *f = fopen(path, "r");

    if (f == NULL) {
        puts("Err: al abrir el archivo!!");
        exit(1);
    }

    String contents = str_new();

    if (str_fgets(&contents, 0, f) == 0) {
        puts("Wrn: No se obtuvo nada del archivo!!");
    } else {
        // printf("Contenido:\n```\n%s\n```\n", str_as_ref(&contents));
        // run(str_as_ref(&contents));
        Grammar g = gr_new();
        gr_load(&g, str_as_ref(&contents), contents.len);
        gr_drop(&g);
    }

    str_drop(&contents);
    fclose(f);
}

HashIdx hashf_vec(void *val) {
    HashIdx res;

    res.idx = 0;
    char * str = (char *)val;

    for (size_t i=0; i < strlen(str); i++) {
        res.idx += (size_t)str[i] * i;
    }

    return res;
}

HashIdx hashf_int(void *val) {
    HashIdx res;

    res.idx = *(size_t *)val;

    return res;
}

int run(char *code) {
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

    HashSet hs = hashset_new(sizeof(char *), &hashf_vec);

    hashset_insert(&hs, "baa");
    hashset_insert(&hs, "aba");
    hashset_insert(&hs, "aab");
    hashset_insert(&hs, "aaa");
    hashset_insert(&hs, "bbb");

    hashset_debug_verbose(&hs);


    hashset_drop(&hs);

    size_t num = 20;
    HashSet hs_nums = hashset_new(sizeof(size_t), &hashf_int);
    hashset_insert(&hs_nums, &num);
    num = 40;
    hashset_insert(&hs_nums, &num);

    hashset_debug_verbose(&hs_nums);

    num = 20;
    hashset_remove(&hs_nums, &num);

    hashset_debug_verbose(&hs_nums);
    
    hashset_drop(&hs_nums);

    switch (argc) {
    case 0: {
        puts("Corriendo interprete");
        run_prompt();
        break;
    }
    case 1: {
        char *path = argv[0];
        printf("Ejecutado archivo: %s\n", path);

        run_file(path);
        break;
    }
    default: {
        puts("Error,usage:\n    lox <FILE>");
        break;
    }
    }

    return 0;
}
