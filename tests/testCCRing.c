#include <stdio.h>
#include <stdlib.h>
// tests
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
// local imports
#include "CCRing.h"


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

void print_array(ccAudioDataType ar1[], unsigned long length) {
  printf("\n");
  for (int i = 0; i < length; i++) {
    printf("%f, ", ar1[i]);
  }
  printf("\n");
}

void compare_test(void) {
  ccAudioDataType a1[] = {1.0, 2.0}; //ccAudioDataType is currently a float value
  ccAudioDataType a2[] = {1.0, 2.0}; 
  CU_ASSERT_TRUE(compare(a1, a2, 2));
}

void append_test(void) {
  CCRing* test_ring = createRing(4);
  ccAudioDataType a1[] = {1.0, 2.0};
  ringAppend(test_ring, a1, 2);
  ccAudioDataType expected[] = {1.0, 2.0, 0.0, 0.0};
  CU_ASSERT_TRUE(compare(test_ring->data, expected, 4));

  ccAudioDataType a2[] = {3.0, 4.0, 5.0};
  ringAppend(test_ring, a2, 3);
  ccAudioDataType expected2[] = {5.0, 2.0, 3.0, 4.0};
  CU_ASSERT_TRUE(compare(test_ring->data, expected2, 4));

}

int main(int argc, char** argv) {

  if (CU_initialize_registry() != CUE_SUCCESS) {
    printf("failed to initialize CUnit\n");
  }

  // create the test suite before adding tests. The last two
  // arguments are optional:
  // CU_InitializeFunc pInit, CU_CleanupFunc pClean
  CU_pSuite ccr_suite = CU_add_suite("CCRing Suite", NULL, NULL);

  CU_pTest t1 = CU_add_test(ccr_suite, "Compare Test", compare_test);
  CU_pTest t2 = CU_add_test(ccr_suite, "Append Test", append_test);

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
