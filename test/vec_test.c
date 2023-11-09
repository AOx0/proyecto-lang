#include "../src/vector.h"

extern int deep_compare(const char *lhs, const char *rhs, size_t len);
int push_vec_char(void);
int push_vec_u64(void);
int get_idx_u64(void);
int pop_u64(void);
int vec_slice_windows(void);
int rem_u64(void);
int vec_resize(void);
int vec_extend_test(void);

int test_v(void) {
    int res = 0;

    printf("%-30s", "VEC::push_vec_char");
    res |= push_vec_char();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::push_vec_u64");
    res |= push_vec_u64();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::vec_extend_test");
    res |= vec_extend_test();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::get_idx_u64");
    res |= get_idx_u64();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::pop_u64");
    res |= pop_u64();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::remove_u64");
    res |= rem_u64();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::vec_slice_windows");
    res |= vec_slice_windows();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    printf("%-30s", "VEC::vec_resize");
    res |= vec_resize();

    if (!res)
        puts("PASS");
    else
        puts("FAIL");

    return res;
}

int vec_resize(void) {
    int res = 0;

    Vec a = vec_new(sizeof(char));

    char push_v[] = {'a', 'b', 'c', 'd'};

    for (int i = 0; i < 4; i++) {
        char *top = vec_push(&a);
        *top = push_v[i];
    }

    vec_shrink(&a);

    if (a.cap != 4)
        res = 1;
    if (a.len != 4)
        res = 1;

    vec_pop(&a);
    vec_pop(&a);

    if (a.cap != 4)
        res = 1;
    if (a.len != 2)
        res = 1;

    vec_shrink(&a);

    if (a.cap != 2)
        res = 1;
    if (a.len != 2)
        res = 1;

    vec_pop(&a);
    vec_pop(&a);

    if (a.cap != 2)
        res = 1;
    if (a.len != 0)
        res = 1;

    vec_shrink(&a);

    if (a.cap != 0)
        res = 1;
    if (a.len != 0)
        res = 1;
    if (a.ptr != 0)
        res = 1;

    vec_drop(&a);

    return res;
}

int push_vec_char(void) {
    int res = 0;

    Vec a = vec_new(sizeof(char));

    char push_v[] = {'a', 'b', 'c', 'd'};

    for (int i = 0; i < 4; i++) {
        char *top = vec_push(&a);
        *top = push_v[i];
    }

    // vec_debug_verbose(&a);

    res |= deep_compare(&push_v[0], (char *)a.ptr, 4);

    vec_drop(&a);

    return res;
}

int push_vec_u64(void) {
    int res = 0;

    Vec a = vec_new(sizeof(uint64_t));

    uint64_t push_v[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&a);
        *top = push_v[i];
    }

    // vec_debug_verbose(&a);

    res |= deep_compare((char *)&push_v[0], (char *)a.ptr, 4);

    vec_drop(&a);

    return res;
}

int get_idx_u64(void) {
    int res = 0;

    Vec a = vec_new(sizeof(uint64_t));

    uint64_t push_v[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&a);
        *top = push_v[i];
    }

    // vec_debug_verbose(&a);

    res |= deep_compare((char *)&push_v[0], (char *)a.ptr, 4);

    uint64_t *val = vec_get(&a, 2);

    res |= (*val != push_v[2]);
    // printf("%llu == %llu\n", *val, push_v[2]);

    vec_drop(&a);

    return res;
}

int rem_u64(void) {
    int res = 0;

    Vec a = vec_new(sizeof(uint64_t));
    uint64_t expected[] = {10, 30, 40};
    uint64_t push_v[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&a);
        *top = push_v[i];
    }

    // vec_debug_verbose(&a);

    res |= deep_compare((char *)&push_v[0], (char *)a.ptr, 4);

    vec_remove(&a, 1);

    res |= deep_compare((char *)&expected[0], (char *)a.ptr, 3);

    // vec_debug_verbose(&a);

    vec_drop(&a);

    return res;
}

int pop_u64(void) {
    int res = 0;

    Vec a = vec_new(sizeof(uint64_t));

    uint64_t push_v[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&a);
        *top = push_v[i];
    }

    // vec_debug_verbose(&a);

    res |= deep_compare((char *)&push_v[0], (char *)a.ptr, 4);

    size_t i = 3;
    while (a.len > 0) {
        uint64_t *val = vec_pop(&a);

        // printf("%llu == %llu\n", *val, push_v[i]);

        res |= (*val != push_v[i]);

        // vec_debug_verbose(&a);

        i -= 1;
    }

    vec_drop(&a);

    return res;
}

int vec_extend_test(void) {
    int res = 0;

    uint64_t expected[] = {10, 20, 30, 40, 50, 60, 70, 80};

    Vec a = vec_new(sizeof(uint64_t));
    uint64_t push_v[] = {10, 20, 30, 40};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&a);
        *top = push_v[i];
    }
    res |= deep_compare((char *)&push_v[0], (char *)a.ptr, 4);

    Vec b = vec_new(sizeof(uint64_t));
    uint64_t push_b[] = {50, 60, 70, 80};

    for (int i = 0; i < 4; i++) {
        uint64_t *top = vec_push(&b);
        *top = push_b[i];
    }
    res |= deep_compare((char *)&push_b[0], (char *)b.ptr, 4);

    vec_extend(&a, &b);

    res |= deep_compare((char *)&expected[0], (char *)a.ptr, 8);

    vec_drop(&a);
    vec_drop(&b);

    return res;
}

int vec_slice_windows(void) {
    int res = 0;

    uint16_t v[] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16_t exp_slice[] = {4, 5, 6};

    VecSlice sl = vec_slice_from_ptr((uint8_t *)v, sizeof(uint16_t), 8);

    vec_slice_self(&sl, 3, 5);

    res |= deep_compare((char *)exp_slice, (char *)sl.ptr, 3);

    return res;
}
