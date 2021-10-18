#ifndef UTEST_H
#define UTEST_H

#define TEST_PASS 0
#define TEST_FAIL 1

#define UTEST_MAX_TESTS 256
#define UTEST_NAME_SIZE 32

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[00m"

typedef unsigned long long int uint_t;
typedef long long int int_t;

extern int (*utest_printer)(const char *, ...);

#define FAIL()                                                                 \
    {                                                                          \
        utest_printer(RED "failed" RESET ": " __FILE__ "+%d: explicit fail\n", \
                      __LINE__);                                               \
        return TEST_FAIL;                                                      \
    }

#define ASSERT_EQ(A, B)                                                        \
    {                                                                          \
        if ((A) != (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected = %lld, Actual = %lld\n",         \
                          __LINE__, (int_t)(A), (int_t)(B));                   \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_UNSIGNED_EQ(A, B)                                               \
    {                                                                          \
        if ((A) != (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected = %llu, Actual = %llu\n",         \
                          __LINE__, (uint_t)(A), (uint_t)(B));                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_PTR_EQ(A, B)                                                    \
    {                                                                          \
        if ((A) != (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected = %p, Actual = %p\n",             \
                          __LINE__, (void *)(A), (void *)(B));                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_NEQ(A, B)                                                       \
    {                                                                          \
        if ((A) == (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: %lld == %lld\n",                           \
                          __LINE__, (int_t)(A), (int_t)(B));                   \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_UNSIGNED_NEQ(A, B)                                              \
    {                                                                          \
        if ((A) == (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: %llu == %llu\n",                           \
                          __LINE__, (uint_t)(A), (uint_t)(B));                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_PTR_NEQ(A, B)                                                   \
    {                                                                          \
        if ((A) == (B)) {                                                      \
            utest_printer(RED "failed" RESET ": " __FILE__ "+%d: %p == %p\n",  \
                          __LINE__, (void *)(A), (void *)(B));                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ABS(A) ((A > 0) ? (A) : -(A))

#define ASSERT_ALMOST_EQ(A, B, D)                                              \
    {                                                                          \
        if (ABS((A) - (B)) > D) {                                              \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected: %lld, Actual: %lld\n",           \
                          __LINE__, (int_t)(A), (int_t)(B));                   \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_UNSIGNED_ALMOST_EQ(A, B, D)                                     \
    {                                                                          \
        if (ABS((A) - (B)) > D) {                                              \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected = %llu, Actual = %llu\n",         \
                          __LINE__, (uint_t)(A), (uint_t)(B));                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_FLOAT_EQ(A, B, D)                                               \
    {                                                                          \
        if (ABS((A) - (B)) > D) {                                              \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Expected = %f, Actual = %f\n",             \
                          __LINE__, (A), (B));                                 \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_NULL(A)                                                         \
    {                                                                          \
        if ((void *)(A) != NULL) {                                             \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: %p not null\n",                            \
                          __LINE__, (void *)(A));                              \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

#define ASSERT_NOT_NULL(A)                                                     \
    {                                                                          \
        if ((void *)(A) == NULL) {                                             \
            utest_printer(RED "failed" RESET ": " __FILE__                     \
                              "+%d: Null-pointer\n",                           \
                          __LINE__);                                           \
            return TEST_FAIL;                                                  \
        }                                                                      \
    }

typedef int (*int_fn_void)(void);
typedef void (*void_fn_void)(void);

#ifdef NO_STD
typedef struct test_st {
    int_fn_void fn;
    void_fn_void setup;
    void_fn_void teardown;
    char name[UTEST_NAME_SIZE];
} test_t;
#else
typedef struct test_st {
    int_fn_void fn;
    void_fn_void setup;
    void_fn_void teardown;
    char name[UTEST_NAME_SIZE];
    struct test_st *next;
} test_t;
#endif

void register_test(int_fn_void, void_fn_void, void_fn_void, char *);

int run_tests(void);

#endif
