#include "utest.h"

#include <string.h>

#ifndef NO_STD
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int (*utest_printer)(const char *, ...) = printf;
#else
int __printstub(const char *format, ...) {
    return 0;
};
int (*utest_printer)(const char *, ...) = __printstub;
#endif

static int tname_width = 18;

test_t tests[UTEST_MAX_TESTS];
static size_t test_count = 0;

void register_test(int_fn_void fn, void_fn_void setup, void_fn_void teardown,
                   char *name) {
    test_t test;
    size_t nlen;

    if (test_count == UTEST_MAX_TESTS) {
        utest_printer("Too many tests (max tests = %d)!\n", UTEST_MAX_TESTS);
        return;
    }

    test.fn = fn;
    test.setup = setup;
    test.teardown = teardown;
    strncpy(test.name, name, UTEST_NAME_SIZE);

    nlen = strlen(test.name);
    if (nlen + 2 > tname_width) {
        tname_width = nlen + 2;
    }

    tests[test_count++] = test;
}

int run_tests(void) {
    int status, failed = 0, passed = 0;
    test_t test;
    size_t i, j, nspace, nlen;

    for (i = 0; i < test_count; i++) {
        test = tests[i];

        utest_printer(CYAN "%s" RESET, test.name);
        nlen = strlen(test.name);
        nspace = (nlen >= tname_width) ? 1 : tname_width - nlen;
        for (j = 0; j < nspace; j++) {
            utest_printer(" ");
        }
        utest_printer("... ");

        if (test.setup)
            test.setup();

        if ((status = test.fn())) {
            failed++;
        } else {
            passed++;
        }

        if (test.teardown)
            test.teardown();

        if (!status) {
            utest_printer(GREEN "ok" RESET "\n");
        }
    }

    utest_printer("\nSummary: %d passed, %d failed.\n", passed, failed);

    return failed != 0;
}
