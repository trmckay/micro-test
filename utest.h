#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <string.h>

#define MAX_TESTS 256
#define TEST_NAME_SIZE 64

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[00m"

#ifdef PRINTF_NO_HEAP

#ifndef PRINT_BUF_SIZE
#define PRINT_BUF_SIZE 1024
#endif

static char print_buf[PRINT_BUF_SIZE];

#define printf(...)                                                            \
  {                                                                            \
    sprintf(print_buf, __VA_ARGS__);                                           \
    fputs(print_buf, stdout);                                                  \
  }

#define eprintf(...)                                                           \
  {                                                                            \
    sprintf(print_buf, __VA_ARGS__);                                           \
    fputs(print_buf, stderr);                                                  \
  }
#else
#define eprintf(...)                                                           \
  { fprintf(stderr, __VA_ARGS__); }
#endif

#define FAIL() return 1

#define ASSERT_EQ(A, B)                                                        \
  {                                                                            \
    if ((A) != (B))                                                            \
      FAIL();                                                                  \
  }

typedef int (*int_fn_void)(void);
typedef void (*void_fn_void)(void);

typedef struct test_st {
  int_fn_void fn;
  void_fn_void setup;
  void_fn_void teardown;
  char name[TEST_NAME_SIZE];
} test_t;

void register_test(int_fn_void fn, void_fn_void setup, void_fn_void teardown,
                   char *name);

int run_tests(void);

#endif
