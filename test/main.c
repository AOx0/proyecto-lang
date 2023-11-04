extern int test_v(void);
extern int test_str(void);

int main(void) {
    int res = 0;

    res |= test_str();
    res |= test_v();

    return res;
}
