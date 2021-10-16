# µ-test

A micro unit testing framework for C/C++ to get you up and running with unit-testing ASAP.


# Overview

Simply include the C and header file in your codebase and register/run your tests. Each test
consists of:

* A test funtion (`int (*fn)(void)`) that returns `TEST_PASS` or `TEST_FAIL`
* Optional setup function (`(void)(*fn)(void)`, `NULL` if not used)
* Optional teardown function (`(void)(*fn)(void)`, `NULL` if not used)
* A name

In your entrypoint, register each test and then call `run_tests()`.


# Example

**demo.c**:

```c
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
```

```
$ gcc -o -c utest.o utest.c
$ gcc -D TEST -o -c demo.o demo.c
$ gcc -o demo demo.o utest.o
$ ./demo

should pass       ... ok
should fail       ... failed: demo.c+27: 7 != 5
explicit fail     ... failed: demo.c+32: explicit fail

Summary: 1 passed, 2 failed.
```
