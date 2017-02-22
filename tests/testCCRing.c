#include <stdio.h>
#include <stdlib.h>
// tests
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/Basic.h>
// local imports
#include "CCRing.h"

int compareArray(ccAudioDataType a[], ccAudioDataType b[], unsigned long length){

  for (unsigned long i = 0; i < length; ++i) {
    if (a[i] != b[i])
      return 0;
  }

  return 1;
}

void testCompareArray(void) {

  ccAudioDataType a2[3] = {1,2,3};
  ccAudioDataType b2[3] = {1,2,3};

  ccAudioDataType a[3] = {1,2,3};
  ccAudioDataType b[3] = {1,4,5};

  CU_ASSERT_TRUE(compareArray(a2, b2, 3));
  CU_ASSERT_FALSE(compareArray(a, b, 3));
}

void initializeCCRing(void) {
  CCRing* pRing = createRing(3);
  ccAudioDataType test[3] = {0, 0, 0};
  CU_ASSERT_TRUE(compareArray(pRing->data, test, 3));

  //change tests to be false
  test[2] = 3;
  CU_ASSERT_FALSE(compareArray(pRing->data, test, 3));
}

void testAppendRing(void) {
  //sees if ccApend function was called
  CCRing* appendRing = createRing(3);
  ccAudioDataType a[6] = {50, 4, 3, 4, 6, 6};
  CU_ASSERT_TRUE(ccAppend(appendRing, a, 6));

  //test if Ring is changed as expected
  CCRing* appendRing2 = createRing(3);
  ccAudioDataType b[6] = {50, 4, 3, 4, 6, 7};
  ccAppend(appendRing2, b, 6);
  ccAudioDataType expected[3] = {4, 6, 7};
  CU_ASSERT_TRUE(compareArray(appendRing2->data, expected, 3));

  CCRing* appendRing3 = createRing(4);
  ccAudioDataType c[8] = {50, 4, 3, 4, 6, 7, 2, 3};
  ccAppend(appendRing3, c, 8);
  ccAudioDataType expected2[4] = {4, 6, 7, 5};
  CU_ASSERT_FALSE(compareArray(appendRing3->data, expected2, 4));

  // test if same Ring being appended keeps pointer index
  ccAudioDataType d[1] = {20};
  ccAudioDataType e[1] = {37};
  ccAppend(appendRing2, d, 1);
  ccAppend(appendRing2, e, 1);
  ccAudioDataType expected3[3] = {20, 37, 7};
  CU_ASSERT_TRUE(compareArray(appendRing2->data, expected3, 3))

  ccAudioDataType f[1] = {47};
  ccAudioDataType g[1] = {80};
  ccAppend(appendRing2, f, 1);
  ccAppend(appendRing2, g, 1);
  ccAudioDataType expected4[3] = {80, 37, 47};
  CU_ASSERT_TRUE(compareArray(appendRing2->data, expected4, 3))

}

int main(int argc, char** argv) {

  if (CU_initialize_registry() != CUE_SUCCESS) {
    printf("failed to initialize CUnit\n");
  }

  // create the test suite before adding tests. The last two
  // arguments are optional:
  // CU_InitializeFunc pInit, CU_CleanupFunc pClean
  CU_pSuite ccr_suite = CU_add_suite("CCRing Suite", NULL, NULL);

  CU_pTest t1 = CU_add_test(ccr_suite, "Compare Arrays", testCompareArray);
  CU_pTest t2 = CU_add_test(ccr_suite, "Initialization Test", initializeCCRing);
  CU_pTest t3 = CU_add_test(ccr_suite, "Appended Array Test", testAppendRing);

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