#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vec vec_new(size_t t_size) {
    Vec res;
    vec_init(&res, t_size);
    return res;
}

void vec_init(Vec *v, size_t t_size) {
    v->ptr = 0;
    v->cap = 0;
    v->len = 0;
    v->t_size = t_size;
}

size_t vec_real_size(Vec *v) { return v->t_size * v->cap; }

size_t vec_real_len(Vec *v) { return v->t_size * v->len; }

void vec_shrink(Vec *v) {
    v->cap = v->len;
    if (v->cap == 0) {
        free(v->ptr);
        v->ptr = 0;
    } else {
        void *ptr = realloc(v->ptr, v->t_size * v->cap);
        if (!ptr) {
            puts("Panic: vec_grow: No ptr realloc");
            exit(1);
        }
        v->ptr = ptr;
    }
}

void vec_grow(Vec *v, size_t cap) {
    if (cap == 0)
        return;

    // printf("Grow %zu\n", cap);

    if (v->cap == 0) {
        v->cap += cap;
        v->ptr = calloc(v->cap, v->t_size);
    } else {
        v->cap += cap;
        void *ptr = realloc(v->ptr, v->t_size * v->cap);
        if (!ptr) {
            puts("Panic: vec_grow: No ptr realloc");
            exit(1);
        }
        v->ptr = ptr;
        memset(&v->ptr[v->t_size * (v->cap - cap)], 0, cap * v->t_size);
    }
}

void vec_init_with_cap(Vec *v, size_t t_size, size_t cap) {
    vec_init(v, t_size);
    vec_grow(v, cap);
}

Vec vec_with_cap(size_t t_size, size_t cap) {
    Vec res = vec_new(t_size);
    vec_grow(&res, cap);

    return res;
}

void *vec_pop(Vec *v) {
    if (v->len == 0)
        return NULL;

    v->len -= 1;
    return &v->ptr[v->t_size * v->len];
}

void vec_remove(Vec *v, size_t idx) {
    if (v->len == 0 && v->len < idx)
        return;
    if (v->len - 1 == idx) {
        vec_pop(v);
    } else {
        for (size_t i = idx; i + 1 < v->len; i++) {
            memcpy(vec_get(v, i), vec_get(v, i + 1), v->t_size);
        }
        v->len -= 1;
    }
}

void *vec_get(Vec *v, size_t idx) {
    if (idx < v->len)
        return &v->ptr[v->t_size * idx];
    else {
        printf("Panic: Index is %zu but len is %zu\n", idx, v->len);
        exit(1);
    }
}

void *vec_push(Vec *v) {
    if (v->cap == v->len)
        vec_grow(v, 1);
    v->len += 1;
    return &v->ptr[v->t_size * (v->len - 1)];
}

void vec_debug(Vec *v) {
    printf("Vec { ptr: %p, size: %zu, len: %zu, cap: %zu }\n", (void *)v->ptr,
           v->t_size, v->len, v->cap);
}

void vec_debug_verbose(Vec *v) {
    printf("Vec { ptr: %p, size: %zu, len: %zu, cap: %zu, val: ",
           (void *)v->ptr, v->t_size, v->len, v->cap);

    for (size_t i = 0; i < vec_real_size(v); i++) {
        printf("%.2X ", v->ptr[i]);
    }

    printf("}\n");
}

void vec_clear(Vec *v) {
    memset(v->ptr, 0, vec_real_len(v));
    v->len = 0;
}

void vec_extend(Vec *v, Vec *o) {
    if (v->cap - v->len < o->len) {
        vec_grow(v, o->len - (v->cap - v->len));
    }

    for (size_t i = 0; i < o->len; i++) {
        void *top = vec_push(v);
        memcpy(top, vec_get(o, i), v->t_size);
    }
}

void vec_drop(Vec *v) {
    v->cap = 0;
    v->len = 0;
    v->t_size = 0;
    free(v->ptr);
}

VecSlice vec_slice_from_ptr(uint8_t *c, size_t t_size, size_t len) {
    VecSlice res;

    res.t_size = t_size;
    res.len = len;
    res.ptr = c;

    return res;
}

VecSlice vec_slice_from_v(Vec *v) {
    VecSlice res;

    res.t_size = v->t_size;
    res.len = v->len;
    res.ptr = v->ptr;

    return res;
}

void vec_slice_self(VecSlice *sl, size_t start, size_t end) {
    if (start > end) {
        printf("Panic: Index is %zu but end is %zu\n", start, end);
        exit(1);
    } else if (end > sl->len) {
        printf("Panic: Index is %zu but len is %zu\n", end, sl->len);
        exit(1);
    }

    sl->ptr = &sl->ptr[start * sl->t_size];
    sl->len = end - start;
}

VecSlice vec_slice_slice(VecSlice *sl, size_t start, size_t end) {
    if (start > end) {
        printf("Panic: Index is %zu but end is %zu\n", start, end);
        exit(1);
    } else if (end > sl->len) {
        printf("Panic: Index is %zu but len is %zu\n", end, sl->len);
        exit(1);
    }

    VecSlice res;

    res.ptr = &sl->ptr[start * sl->t_size];
    res.len = end - start;

    return res;
}

VecSlice vec_slice_slice_end(VecSlice *sl, size_t start) {
    if (start > sl->len) {
        printf("Panic: Index is %zu but len is %zu", start, sl->len);
        exit(1);
    }

    VecSlice res = vec_slice_slice(sl, start, sl->len);
    return res;
}

Vec vec_slice_owned(VecSlice *sl) {
    Vec res = vec_with_cap(sl->t_size, sl->len);

    if (res.ptr == NULL) {
        puts("Panic: NULL ptr on non-null context");
        exit(1);
    }

    memcpy(res.ptr, sl->ptr, sl->len * sl->t_size);
    res.len = sl->len;

    return res;
}

void *vec_slice_get(VecSlice *sl, size_t idx) {
    if (idx < sl->len)
        return &sl->ptr[sl->t_size * idx];
    else
        return NULL;
}

void *vec_last(Vec *v) { return vec_get(v, v->len - 1); }

void *vec_top(Vec *v) { return vec_last(v); }

void *vec_first(Vec *v) { return vec_get(v, 0); }

void *vec_bottom(Vec *v) { return vec_first(v); }
void *vec_pop_into(Vec *v, void *dst) {
    if (v->len == 0)
        return NULL;

    v->len -= 1;
    memcpy(dst, &v->ptr[v->t_size * v->len], v->t_size);
    return dst;
}
