#include <stdlib.h>

static void *handle;

void lib_init(void) {
    if ((handle = malloc(512)) == NULL)
        exit(EXIT_FAILURE);
}

void lib_shutdown(void) {
    if (handle)
        free(handle);
}

int lib_fn(int a, int b) { return a + b; }

#ifdef TEST

#include "utest.h"

int test_should_pass(void) {
    ASSERT_EQ(5, lib_fn(2, 3));
    return TEST_PASS;
}

int test_should_fail(void) {
    ASSERT_EQ(7, lib_fn(2, 3));
    return TEST_PASS;
}

int test_explicit_fail(void) { FAIL(); }

int main(void) {
    register_test(test_should_pass, lib_init, lib_shutdown, "should pass");
    register_test(test_should_fail, lib_init, lib_shutdown, "should fail");
    register_test(test_explicit_fail, lib_init, lib_shutdown, "explicit fail");

    return run_tests();
}

#endif
