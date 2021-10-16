#ifndef UTEST_H
#define U_TEST_H

#include <stdio.h>
#include <string.h>

#define TEST_PASS 0
#define TEST_FAIL 1

#define UTEST_MAX_TESTS 256
#define UTEST_NAME_SIZE 32

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[00m"

#ifdef PRINTF_NO_HEAP

#ifndef UTEST_PRINT_BUF_SIZE
#define UTEST_PRINT_BUF_SIZE 1024
#endif

static char print_buf[UTEST_PRINT_BUF_SIZE];

#define printf(...)                                                            \
    {                                                                          \
        sprintf(print_buf, __VA_ARGS__);                                       \
        fputs(print_buf, stdout);                                              \
    }

#define eprintf(...)                                                           \
    {                                                                          \
        sprintf(print_buf, __VA_ARGS__);                                       \
        fputs(print_buf, stderr);                                              \
    }
#else
#define eprintf(...)                                                           \
    { fprintf(stderr, __VA_ARGS__); }
#endif

#define FAIL()                                                                 \
    {                                                                          \
        printf(RED "failed" RESET ": " __FILE__ "+%ld: explicit fail\n",       \
               __LINE__);                                                      \
        return TEST_FAIL;                                                      \
    }

#define ASSERT_EQ(A, B)                                                        \
    {                                                                          \
        if (A != B) {                                                          \
            printf(RED "failed" RESET ": " __FILE__ "+%ld: %lld != %lld\n",    \
                   __LINE__, A, B);                                            \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

typedef int (*int_fn_void)(void);
typedef void (*void_fn_void)(void);

typedef struct test_st {
    int_fn_void fn;
    void_fn_void setup;
    void_fn_void teardown;
    char name[UTEST_NAME_SIZE];
} test_t;

void register_test(int_fn_void fn, void_fn_void setup, void_fn_void teardown,
                   char *name);

int run_tests(void);

#endif
