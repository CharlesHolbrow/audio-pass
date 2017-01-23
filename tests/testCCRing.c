#include <stdio.h>
#include <stdlib.h>
// tests
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
// local imports
#include "CCRing.h"


void simple_test(void) {
  CU_ASSERT_EQUAL(1, 1.);
}

void should_fail(void) {
  CU_ASSERT_EQUAL(1, 2);
}

//use ccAudioDataType array arguments, unsigned long length
int compare(ccAudioDataType ar1[], ccAudioDataType ar2[], unsigned long argc) {
  int i;
  for (i = 0; i < argc; i++) {
    if (ar1[i] != ar2[i]){
      return 0;
    }
  }
  return 1;
}

void compare_test(void) {
  ccAudioDataType a1[] = {1.0, 2.0}; //ccAudioDataType is currently a float value
  ccAudioDataType a2[] = {1.0, 2.0}; 
  CU_ASSERT_TRUE(compare(a1, a2, 2));
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
  CU_pTest t3 = CU_add_test(ccr_suite, "Compare Test", compare_test);

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
