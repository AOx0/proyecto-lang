extern int test_v(void);
extern int test_str(void);
extern int test_hashset(void);
extern int test_tree(void);

int main(void) {
    int res = 0;

    res |= test_str();
    res |= test_v();
    res |= test_hashset();
    res |= test_tree();

    return res;
}
