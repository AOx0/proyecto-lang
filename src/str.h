#ifndef LNG_STRING
#define LNG_STRING

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String {
        char *ptr;
        size_t len;
        size_t cap;
};
typedef struct String String;

struct StrSlice {
        char *ptr;
        size_t len;
};
typedef struct StrSlice StrSlice;

struct StrIter {
        StrSlice *sl;
        char *sep;
        char ended;
};
typedef struct StrIter StrIter;

struct StrSliceDupla {
        StrSlice lhs;
        StrSlice rhs;
        int did_perform;
};
typedef struct StrSliceDupla StrSliceDupla;

int is_stream(FILE *file);

String str_new(void);
void str_init(String *s);
void str_init_with_cap(String *s, size_t cap);
void str_init_from_cstr(String *s, char *c, size_t cap);
void str_init_from_slice(String *s, StrSlice *sl);

char *str_as_ref(String *s);
String str_with_cap(size_t cap);
size_t str_gets(String *s);
size_t str_getline(String *s);
size_t str_fgets(String *s, char termination, FILE *f);
void str_trim_end(String *s, char c);
void str_grow(String *s, size_t cap);
void str_debug(String *s);
void str_debug_verbose(String *s);
void str_clear(String *s);
void str_push(String *s, char *str);
void str_push_sizet(String *s, size_t val);
void str_push_int(String *s, int64_t val);
void str_push_n(String *s, char *str, size_t len);
void str_drop(String *s);
void str_push_slice(String *s, StrSlice *sl);

StrSlice str_slice_from_cstr(char *c, size_t len);
StrSlice str_slice_new(String *s);

void str_slice_debug(StrSlice *sl);
void str_slice_debug_verbose(StrSlice *sl);
void str_slice_self(StrSlice *sl, size_t start, size_t end);

StrSlice str_slice_slice(StrSlice *sl, size_t start, size_t end);
StrSlice str_slice_slice_end(StrSlice *sl, size_t start);
String str_slice_owned(StrSlice *sl);
StrIter str_iter_from_slice(StrSlice *sl, char *sep);
StrSlice str_iter_next(StrIter *it);

size_t str_slice_find(StrSlice *sl, char *find, size_t sep_len);
StrSliceDupla str_slice_split_once(StrSlice *sl, char *with);

#endif
