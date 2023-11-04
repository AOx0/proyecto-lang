#include "str.h"
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 2

int is_stream(FILE *file) {
    if (fseek(file, 0, SEEK_END) == 0) {
        return (ftell(file) == -1);
    } else {
        return -1;
    }
}

String str_new(void) {
    String s;
    str_init(&s);
    return s;
}

void str_init(String *s) {
    s->cap = 0;
    s->len = 0;
    s->ptr = 0;
}

void str_init_with_cap(String * s, size_t cap) {
    str_init(s);
    str_grow(s, cap);
}

void str_init_from_cstr(String * s, char * c, size_t len) {
    str_init(s);
    str_grow(s, len);
    str_push_n(s, c, len);
}

void str_init_from_slice(String * s, StrSlice * sl) {
    str_init_from_cstr(s, sl->ptr, sl->len);
}


char *str_as_ref(String *s) { return s->ptr; }

String str_with_cap(size_t cap) {
    String s = str_new();

    str_grow(&s, cap);

    return s;
}

size_t str_gets(String *s) { return str_fgets(s, '\n', stdin); }

size_t str_getline(String *s) { return str_fgets(s, '\n', stdin); }

size_t str_fgets(String *s, char termination, FILE *restrict f) {
    char buff[BUFF_SIZE] = {0};
    size_t total_read = 0;

    if (!is_stream(f)) {
        fseek(f, 0, SEEK_END);  // seek to end of file
        size_t size = ftell(f); // get current file pointer
        fseek(f, 0, SEEK_SET);  // seek back to beginning of file

        if (size > s->cap) {
            printf("Resize to %zu\n", size);
            str_grow(s, size - s->cap + 5);
        }
    }

    while (1) {
        size_t read_len = fread(&buff[0], sizeof(char), BUFF_SIZE - 1, f);
        if (read_len == 0)
            break;

        str_push(s, buff);
        total_read += read_len;

        if (buff[read_len - 1] == termination)
            break;
        memset(buff, 0, BUFF_SIZE);
    }

    return total_read;
}

void str_trim_end(String *s, char c) {
    size_t end = s->len;

    while (end > 0 && s->ptr[end - 1] == c) {
        s->len -= 1;
        s->ptr[end - 1] = 0;
    }
}

void str_grow(String *s, size_t cap) {
    if (cap == 0)
        return;

    if (s->cap == 0) {
        s->cap += cap + 1;
        s->ptr = calloc(s->cap, sizeof(char));
    } else {
        s->cap += cap + 1;
        void *ptr = realloc(s->ptr, sizeof(char) * s->cap);
        if (!ptr) {
            puts("Panic: vec_grow: No ptr realloc");
            exit(1);
        }
        s->ptr = ptr;
        memset(s->ptr + s->cap - cap - 1, 0, cap);
    }
}

void str_debug(String *s) {
    printf("String { ptr: %p, len: %zu, cap: %zu }\n", (void *)s->ptr,
           s->len, s->cap);
}

void str_slice_debug(StrSlice *sl) {
    printf("StrSlice { ptr: %p, len: %zu }\n", (void *)sl->ptr,
           sl->len);
}

void str_debug_verbose(String *s) {
    printf("String { ptr: %p, len: %zu, cap: %zu, val: ", (void *)s->ptr,
           s->len, s->cap);

    for (size_t i = 0; i < s->cap; i++) {
        printf("%.2X ", s->ptr[i]);
    }

    printf("}\n");
}

void str_slice_debug_verbose(StrSlice *sl) {
    printf("StrSlice { ptr: %p, len: %zu, val: ", (void *)sl->ptr,
           sl->len);

    for (size_t i = 0; i < sl->len; i++) {
        printf("%.2X ", sl->ptr[i]);
    }

    printf("}\n");
}

void str_clear(String *s) {
    if (s->len == 0) return;

    memset(s->ptr, 0, s->len);
    s->len = 0;
}

void str_push(String *s, char *str) {
    size_t len = strlen(str);
    str_push_n(s, str, len);
}

void str_push_n(String *s, char *str, size_t len) {
    while (len) {
        if (s->cap > len + s->len) {
            strncpy(&s->ptr[s->len], str, len);
            s->len += len;
            return;
        } else {
            str_grow(s, len);
            continue;
        }
    }
}

void str_drop(String *s) {
    // printf("Dropping str\n");
    s->cap = 0;
    s->len = 0;
    free(s->ptr);
}

StrSlice str_slice_from_cstr(char *c, size_t len) {
    StrSlice res;

    res.ptr = c;
    res.len = len;

    return res;
}

StrSlice str_slice_new(String *s) {
    StrSlice res;

    res.ptr = s->ptr;
    res.len = s->len;

    return res;
}

void str_slice_self(StrSlice *sl, size_t start, size_t end) {
    if (start > end) {
        printf("Panic: Index is %zu but end is %zu\n", start, end);
        exit(1);
    } else if (end > sl->len) {
        printf("Panic: Index is %zu but len is %zu\n", end, sl->len);
        exit(1);
    }

    sl->ptr = &sl->ptr[start];
    sl->len = end - start;
}

StrSlice str_slice_slice(StrSlice *sl, size_t start,
                                 size_t end) {
    if (start > end) {
        printf("Panic: Index is %zu but end is %zu\n", start, end);
        exit(1);
    } else if (end > sl->len) {
        printf("Panic: Index is %zu but len is %zu\n", end, sl->len);
        exit(1);
    }

    StrSlice res;

    res.ptr = &sl->ptr[start];
    res.len = end - start;

    return res;
}

StrSlice str_slice_slice_end(StrSlice *sl, size_t start) {
    if (start > sl->len) {
        printf("Panic: Index is %zu but len is %zu", start, sl->len);
        exit(1);
    }

    StrSlice res = str_slice_slice(sl, start, sl->len);
    return res;
}

String str_slice_owned(StrSlice *sl) {
    String res = str_with_cap(sl->len);
    str_push_n(&res, sl->ptr, sl->len);

    return res;
}

void str_push_slice(String *s, StrSlice *sl) {
    str_push_n(s, sl->ptr, sl->len);
}

StrIter str_iter_from_slice(StrSlice *sl, char *sep) {
    StrIter res;
    res.sl = sl;
    res.ended = 0;
    res.sep = sep;

    return res;
}

StrSlice str_iter_next(StrIter *it) {
    StrSlice res;
    size_t sep_len = strlen(it->sep);

    size_t idx = 0;

    while (idx < it->sl->len && it->sl->len > sep_len && !it->ended) {

        if (strncmp(&it->sl->ptr[idx], it->sep, sep_len) == 0) {
            res = str_slice_slice(it->sl, 0, idx);
            str_slice_self(it->sl, idx + sep_len, it->sl->len);
            return res;
        }

        idx += 1;
    }

    if (!it->ended) {
        res = str_slice_slice_end(it->sl, 0);
        it->ended = 1;
        return res;
    }

    puts("Panic: Empty iterator");
    exit(1);
}


StrSliceDupla str_slice_split_once(StrSlice * sl, char * with) {
    size_t sep_len = strlen(with);
    
    StrSliceDupla res;
    
    size_t first = 0;
    int set_first = 0;
    
    for (size_t i = 0; i + sep_len <= sl->len ; i++) {
        if (strncmp(&sl->ptr[i], with, sep_len) == 0) {
            first = i;
            set_first = 1;
        };
    }
    
    if (set_first) {
        res.lhs = str_slice_slice(sl, 0, first);
        res.rhs = str_slice_slice(sl, first + sep_len, sl->len);
        res.did_perform = 1;
    } else {
        memcpy(&res.lhs, sl, sizeof(StrSlice));
        memcpy(&res.rhs, sl, sizeof(StrSlice));
        res.did_perform = 0;
    }
    
    
    
    return res;
}

