#include "utest.h"
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#ifdef NO_STD

int __printstub(const char *format, ...) { return 0; };
int (*utest_printer)(const char *, ...) = __printstub;

test_t tests[UTEST_MAX_TESTS];
static size_t test_count = 0;

#else

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int (*utest_printer)(const char *, ...) = printf;
test_t *head = NULL;

#endif

int utest_capture = 1;

static int tname_width = 0;

void register_test(int_fn_void fn, void_fn_void setup, void_fn_void teardown,
                   char *name) {
    size_t nlen;
    test_t test;

#ifdef NO_STD
    if (test_count == UTEST_MAX_TESTS) {
        utest_printer("Too many tests (max tests = %d)!\n", UTEST_MAX_TESTS);
        return;
    }
#else
    test_t *test_buf, *curr;

    if ((test_buf = malloc(sizeof(test_t))) == NULL) {
        perror("");
        exit(EXIT_FAILURE);
    }
#endif
    test.fn = fn;
    test.setup = setup;
    test.teardown = teardown;

    strncpy(test.name, name, UTEST_NAME_SIZE);

    nlen = strlen(test.name);

#ifdef NO_STD
    tests[test_count++] = test;
#else
    test.next = NULL;
    *test_buf = test;

    if (head == NULL) {
        head = test_buf;
    } else {
        curr = head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = test_buf;
    }
#endif

    if (nlen + 2 > tname_width) {
        tname_width = nlen + 2;
    }
}

int run_test(test_t *test) {
    int status, log, stdout_cpy, stderr_cpy;

    if (utest_capture) {
        if ((log = open(UTEST_LOG, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1)
            return TEST_ERROR;

        if ((stdout_cpy = dup(1)) == -1)
            return TEST_ERROR;

        if ((stderr_cpy = dup(2)) == -1)
            return TEST_ERROR;

        close(1);
        close(2);

        if (dup2(log, 1) == -1)
            return TEST_ERROR;

        if (dup2(log, 2) == -1)
            return TEST_ERROR;
    }

    if (test->setup)
        test->setup();

    status = test->fn();

    if (test->teardown)
        test->teardown();

    if (utest_capture) {
        if (dup2(stdout_cpy, 1) == -1)
            return TEST_ERROR;

        if (dup2(stderr_cpy, 2) == -1)
            return TEST_ERROR;

        if (close(stdout_cpy) == -1)
            return TEST_ERROR;

        if (close(stderr_cpy) == -1)
            return TEST_ERROR;

        if (close(log) == -1)
            return TEST_ERROR;
    }

    return status;
}

void echo_log() {
    int log, nb, w;
    char buf[256];

    if ((log = open(UTEST_LOG, O_RDONLY)) == -1) {
        return;
    }

    while ((nb = read(log, buf, 256)) > 0) {
        w = write(1, buf, nb);
        while (w != -1 && (nb -= w) > 0);
    }

    close(log);
}

int run_tests(void) {
    int status, failed = 0, passed = 0;
    test_t test;
    size_t j, nspace, nlen;

#ifdef NO_STD
    size_t i;

    for (i = 0; i < test_count; i++) {
        test = tests[i];
#else
    test_t *curr = head, *tmp;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    while (curr != NULL) {
        test = *curr;
#endif
        utest_printer(CYAN "%s" RESET, test.name);
        nlen = strlen(test.name);
        nspace = (nlen >= tname_width) ? 1 : tname_width - nlen;
        for (j = 0; j < nspace; j++) {
            utest_printer(" ");
        }
        utest_printer("... ");

        if ((status = run_test(&test)) == 1) {
            failed++;
            utest_printer(RED "failed" RESET "\n");
            echo_log();
        } else if (status == TEST_ERROR) {
            return TEST_ERROR;
        } else {
            passed++;
            utest_printer(GREEN "ok" RESET "\n");
        }

#ifndef NO_STD
        tmp = curr->next;
        free(curr);
        curr = tmp;
#endif
    }

    utest_printer("Summary: %d passed, %d failed.\n", passed, failed);

    return failed != 0;
}
