#include "utest.h"

test_t tests[MAX_TESTS];
static size_t test_count = 0;

void register_test(int_fn_void fn, void_fn_void setup, void_fn_void teardown,
                   char *name) {
  test_t test;

  if (test_count == MAX_TESTS) {
    eprintf("Too many tests (max tests = %d)!\n", MAX_TESTS);
    return;
  }

  test.fn = fn;
  test.setup = setup;
  test.teardown = teardown;
  strncpy(test.name, name, TEST_NAME_SIZE);

  tests[test_count++] = test;
}

int run_tests(void) {
  int status, failed = 0, passed = 0;
  test_t test;
  size_t i;

  printf("\n");

  for (i = 0; i < test_count; i++) {
    test = tests[i];

    printf("Running test: " CYAN "%s" RESET "... ", tests[i].name);

    if (test.setup)
      test.setup();

    if ((status = tests[i].fn())) {
      failed++;
    } else {
      passed++;
    }

    if (test.teardown)
      test.teardown();

    if (status) {
      printf(RED "failed" RESET "\n");
    } else {
      printf(GREEN "ok" RESET "\n");
    }
  }

  printf("\nSummary: %d passed, %d failed.\n", passed, failed);

  return failed == 0;
}
