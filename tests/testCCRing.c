#import <stdio.h>
#import <stdlib.h>
// tests
#import <CUnit/cunit.h>
#include <CUnit/Console.h>
// local imports
#import "CCRing.h"


void simple_test(void) {
  CU_ASSERT_EQUAL(1, 1.);
}

void should_fail(void) {
  CU_ASSERT_EQUAL(1, 2);
}

int main(int argc, char** argv) {

  if (CU_initialize_registry() != CUE_SUCCESS) {
    printf("failed to initialize CUnit\n");
  }

  // create the test suite before adding tests. The last two
  // arguments are optional:
  // CU_InitializeFunc pInit, CU_CleanupFunc pClean
  CU_pSuite ccr_suite = CU_add_suite("CCRing Suite", NULL, NULL);

  CU_pTest t1 = CU_add_test(ccr_suite, "Simple Test", simple_test);
  CU_pTest t2 = CU_add_test(ccr_suite, "Should Fail", should_fail);

  if (ccr_suite == NULL) {
    // check the framework error code
    return 0;
  }

  // run and cleanup
  // CU_console_run_tests(); // interactive console
  CU_basic_run_tests(); // results to stdout
  CU_cleanup_registry();

  return 1;
}
