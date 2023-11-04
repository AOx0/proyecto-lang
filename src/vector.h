#ifndef LNG_VECTOR
#define LNG_VECTOR

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vec {
        uint8_t *ptr;
        size_t cap;
        size_t len;
        size_t t_size;
};
typedef struct Vec Vec;

struct VecSlice {
        uint8_t *ptr;
        size_t len;
        size_t t_size;
};
typedef struct VecSlice VecSlice;

Vec vec_new(size_t t_size);
void vec_grow(Vec *v, size_t cap);
Vec vec_with_cap(size_t t_size, size_t cap);
void vec_init(Vec *v, size_t t_size);
void vec_init_with_cap(Vec *v, size_t t_size, size_t cap);
void *vec_get(Vec *v, size_t idx);
void *vec_push(Vec *v);
void vec_remove(Vec *v, size_t idx);
void *vec_pop(Vec *v);
void *vec_pop_into(Vec *v, void *dst);
void vec_debug_verbose(Vec *v);
size_t vec_real_size(Vec *v);
size_t vec_real_len(Vec *v);
void vec_clear(Vec *v);
void vec_drop(Vec *v);

void *vec_last(Vec *v);
void *vec_top(Vec *v);
void *vec_bottom(Vec *v);
void *vec_first(Vec *v);

VecSlice vec_slice_from_ptr(uint8_t *c, size_t t_size, size_t len);
VecSlice vec_slice_from_v(Vec *v);

void vec_slice_self(VecSlice *sl, size_t start, size_t end);

VecSlice vec_slice_slice(VecSlice *sl, size_t start, size_t end);
VecSlice vec_slice_slice_end(VecSlice *sl, size_t start);
void *vec_slice_get(VecSlice *sl, size_t idx);
Vec vec_slice_owned(VecSlice *sl);

#endif
