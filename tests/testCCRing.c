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
  for (int i = 0; i < argc; i++) {
    if (ar1[i] != ar2[i]) {
      return 0;
    }
  }
  return 1;
}


int compare_float_array(ccAudioDataType ar1[], ccAudioDataType ar2[], unsigned long argc) {
  for (int i = 0; i < argc; i++) {
    if (fabsf(ar1[i] - ar2[i]) >= 1.0E-7) {
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
  ccAudioDataType a1[] = {1.0, 2.0}; 
  ccAudioDataType a2[] = {1.0, 2.0}; 
  CU_ASSERT_TRUE(compare(a1, a2, 2));
}


void append_test(void) {
  CCRing* test_ring = createRing(4);
  ccAudioDataType a1[] = {1.0, 2.0};
  ccAppend(test_ring, a1, 2);
  ccAudioDataType expected[] = {1.0, 2.0, 0.0, 0.0};
  CU_ASSERT_TRUE(compare(test_ring->data, expected, 4));

  ccAudioDataType a2[] = {3.0, 4.0, 5.0};
  ccAppend(test_ring, a2, 3);
  ccAudioDataType expected2[] = {5.0, 2.0, 3.0, 4.0};
  CU_ASSERT_TRUE(compare(test_ring->data, expected2, 4));
}


void testcompare(void) {
  ccAudioDataType a2[3] = {1,2,3};
  ccAudioDataType b2[3] = {1,2,3};

  ccAudioDataType a[3] = {1,2,3};
  ccAudioDataType b[3] = {1,4,5};

  CU_ASSERT_TRUE(compare(a2, b2, 3));
  CU_ASSERT_FALSE(compare(a, b, 3));
}


void initializeCCRing(void) {
  CCRing* pRing = createRing(3);
  ccAudioDataType test[3] = {0, 0, 0};
  CU_ASSERT_TRUE(compare(pRing->data, test, 3));

  //change tests to be false
  test[2] = 3;
  CU_ASSERT_FALSE(compare(pRing->data, test, 3));
}


void testAppendRing(void) {
  //test if Ring is changed as expected
  CCRing* appendRing2 = createRing(3);
  ccAudioDataType b[6] = {50, 4, 3, 4, 6, 7};
  ccAppend(appendRing2, b, 6);
  ccAudioDataType expected[3] = {4, 6, 7};
  CU_ASSERT_TRUE(compare(appendRing2->data, expected, 3));

  CCRing* appendRing3 = createRing(4);
  ccAudioDataType c[8] = {50, 4, 3, 4, 6, 7, 2, 3};
  ccAppend(appendRing3, c, 8);
  ccAudioDataType expected2[4] = {4, 6, 7, 5};
  CU_ASSERT_FALSE(compare(appendRing3->data, expected2, 4));

  // test if same Ring being appended keeps pointer index
  ccAudioDataType d[1] = {20};
  ccAudioDataType e[1] = {37};
  ccAppend(appendRing2, d, 1);
  ccAppend(appendRing2, e, 1);
  ccAudioDataType expected3[3] = {20, 37, 7};
  CU_ASSERT_TRUE(compare(appendRing2->data, expected3, 3))

  ccAudioDataType f[1] = {47};
  ccAudioDataType g[1] = {80};
  ccAppend(appendRing2, f, 1);
  ccAppend(appendRing2, g, 1);
  ccAudioDataType expected4[3] = {80, 37, 47};
  CU_ASSERT_TRUE(compare(appendRing2->data, expected4, 3))
}


void testGenerateSin(void) {
  CCRing* ring = createRing(4);
  double cycles = 1.0;
  ccGenerateSin(ring, cycles);
  ccAudioDataType expected[4] = {0, 1, 0, -1};
  CU_ASSERT_TRUE(compare_float_array(ring->data, expected, 4))

  CCRing* ring2 = createRing(4);
  double cycles2 = 3.0;
  ccGenerateSin(ring2, cycles2);
  ccAudioDataType expected2[4] = {0, -1, 0, 1};
  CU_ASSERT_TRUE(compare_float_array(ring2->data, expected2, 4));
}


void testCompareFloatArray(void) {
  ccAudioDataType ar1[3] = {1.00000001, 2.00000001, 3.00000003}; //return True
  ccAudioDataType ar2[3] = {1.00000000, 2.00000000, 3.00000002};
  CU_ASSERT_TRUE(compare_float_array(ar1, ar2, 3))

  ccAudioDataType ar3[3] = {1.000001, 2.000002, 3.000003}; //return False
  ccAudioDataType ar4[3] = {1.000000, 2.000001, 3.000002};
  CU_ASSERT_FALSE(compare_float_array(ar3, ar4, 3))
}


int main(int argc, char** argv) {

  if (CU_initialize_registry() != CUE_SUCCESS) {
    printf("failed to initialize CUnit\n");
  }

  // create the test suite before adding tests. The last two
  // arguments are optional:
  // CU_InitializeFunc pInit, CU_CleanupFunc pClean
  CU_pSuite ccr_suite = CU_add_suite("CCRing Suite", NULL, NULL);

  CU_pTest t1 = CU_add_test(ccr_suite, "Compare Array Test", testcompare);
  CU_pTest t2 = CU_add_test(ccr_suite, "Initialization Test", initializeCCRing);
  CU_pTest t3 = CU_add_test(ccr_suite, "Appended Array Test", testAppendRing);
  CU_pTest t4 = CU_add_test(ccr_suite, "Compare Test", compare_test);
  CU_pTest t5 = CU_add_test(ccr_suite, "Append Test", append_test);
  CU_pTest t6 = CU_add_test(ccr_suite, "Generate Sin Test", testGenerateSin);
  CU_pTest t7 = CU_add_test(ccr_suite, "Float Array Test", testCompareFloatArray);;

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