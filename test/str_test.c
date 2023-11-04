#include "include/str.h"
#include <stdio.h>
#include <string.h>

int push(void);
int slice(void);
int iter(void);
int iter_lines(void);
int split_once_test(void);

int test_str(void) {
    int res = 0;

    puts("STRT : str::push");
    res |= push();

    if (!res)
        puts("    PASS : str::push");

    puts("STRT : str::slice");
    res |= slice();

    if (!res)
        puts("    PASS : str::slice");

    puts("STRT : str::iter");
    res |= iter();

    if (!res)
        puts("    PASS : str::iter");

    puts("STRT : str::iter_lines");
    res |= iter_lines();

    if (!res)
        puts("    PASS : str::iter_lines");

    puts("STRT : str::split_once");
    res |= split_once_test();

    if (!res)
        puts("    PASS : str::split_once");

    return res;
}

int deep_compare(const char *lhs, const char *rhs, size_t len) {
    int err = 0;
    for (size_t i = 0; i < len; ++i) {
        char e = lhs[i];
        char g = rhs[i];
        if (e != g) {
            printf("Mismatch %x and %x\n", e, g);
            err = 1;
        } else {
            // printf("Match %x and %x\n", e, g);
        }
    }

    return err;
}

int split_once_test(void) {
    int res = 0;
    char *original = "Hola h  k amigo";

    StrSlice sl = str_slice_from_cstr(original, strlen(original));

    StrSliceDupla dp = str_slice_split_once(&sl, " h  k ");

    String buff = str_new();

    str_push_slice(&buff, &dp.lhs);
    printf("LHS: '%s'\n", str_as_ref(&buff));

    str_clear(&buff);
    str_push_slice(&buff, &dp.rhs);
    printf("RHS: '%s'\n", str_as_ref(&buff));

    return 0;
}

int iter(void) {
    int res = 0;
    char *original = "Hola--soy--Daniel------a--";

    char *expected[7] = {"Hola", "soy", "Daniel", "", "", "a", ""};

    String str = str_with_cap(strlen(original));
    str_push(&str, original);
    StrSlice s = str_slice_new(&str);

    StrIter it = str_iter_from_slice(&s, "--");

    String ls = str_new();

    int i = 0;
    while (!it.ended) {

        StrSlice line = str_iter_next(&it);

        str_clear(&ls);
        str_push_slice(&ls, &line);
        // str_debug_verbose(&ls);
        // printf("Va: '%s'\n", str_as_ref(&ls));

        res |= deep_compare(&expected[i][0], str_as_ref(&ls),
                            strlen(&expected[i][0]));

        i += 1;
    }

    str_drop(&str);
    str_drop(&ls);

    return res;
}

int iter_lines(void) {
    int res = 0;
    char *original = "Hola\nsoy\nDaniel\n\n\na\n";

    char *expected[7] = {"Hola", "soy", "Daniel", "", "", "a", ""};

    StrSlice s = str_slice_from_cstr(original, strlen(original));
    StrIter it = str_iter_from_slice(&s, "\n");

    String ls = str_new();

    int i = 0;
    while (!it.ended) {

        StrSlice line = str_iter_next(&it);

        str_clear(&ls);
        str_push_slice(&ls, &line);
        // str_debug(&ls);
        // printf("Va: '%s'\n", str_as_ref(&ls));

        res |= deep_compare(&expected[i][0], str_as_ref(&ls),
                            strlen(&expected[i][0]));

        i += 1;
    }

    str_drop(&ls);

    return res;
}

int slice(void) {
    int res = 0;

    char *original = "Hola, soy Daniel";

    char *expected[8] = {"Ho", "la", ", ", "so", "y ", "Da", "ni", "el"};

    String str = str_with_cap(strlen(original));
    str_push(&str, original);
    StrSlice s = str_slice_new(&str);

    String ls = str_new();
    int j = 0;
    for (size_t i = 0; res == 0 && i < strlen(original) - 1; i += 2) {
        StrSlice sl = str_slice_slice(&s, i, i + 2);

        str_clear(&ls);
        str_push_slice(&ls, &sl);
        // str_debug(&ls);
        // printf("Va: (%zu, %zu) %s\n", i, i+1, str_as_ref(&ls));

        res |= deep_compare(&expected[j][0], str_as_ref(&ls), 3);

        ++j;
    }

    str_drop(&str);
    str_drop(&ls);

    return res;
}

int push(void) {
    int res = 0;

    String s = str_new();
    str_push(&s, "a");
    str_push(&s, "b");
    str_push(&s, "c");

    // str_debug(&s);

    char expected[4] = {'a', 'b', 'c', 0};

    // printf("%s == %s\n", str_as_ref(&s), expected);

    res |= deep_compare(&expected[0], str_as_ref(&s), 4);

    str_drop(&s);

    return res;
}
