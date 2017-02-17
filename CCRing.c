#include <stdlib.h>
#include <string.h> // memset
#include <stdio.h>
#include <math.h>
#include "CCRing.h"

#define PI 3.14159265

CCRing* createRing(unsigned long length) {

  CCRing* ring = (CCRing*) malloc(sizeof(CCRing));
  if (ring == NULL) {
    return NULL;
  }

  // size of the data in bytes
  size_t dataSize = length * sizeof(ccAudioDataType);

  // allocate data storage
  ring->data = (ccAudioDataType*) malloc(dataSize);
  if (ring->data == NULL) {
    free(ring);
    return NULL;
  }

  ring->length = length;
  ring->index_ring = 0;

  // initialize all data to zero
  memset(ring->data, 0, dataSize);
  return ring;
}


CCError freeRing(CCRing* pRing) {
  free(pRing->data);
  free(pRing);
  return ccNoError;
}


int ccAppend(CCRing* appendRing, ccAudioDataType array[], unsigned long length) {
  unsigned long index = appendRing->index_ring;

  for (unsigned long i = 0; i < length; ++i) {
      unsigned long n = (index + i) % appendRing->length;
      appendRing->data[n] = array[i];

      if (i==(length - 1) ){
        index = ((index + i + 1) % appendRing->length);
        appendRing->index_ring = index;
      }
   }
  return 1;
}


CCError ccGenerateSin(CCRing* sinusoid, double cycles) {
  unsigned long length = sinusoid->length;
  double distBetweenPoints = (2*PI)/length;

  ccAudioDataType dataPoints[length];
  for (unsigned long i = 0; i < length; ++i) {
    double n = i * distBetweenPoints;
    dataPoints[i] = sin(cycles * n);
  }

  ccAppend(sinusoid, dataPoints, length);

  /************************* PRINT STATEMENTS *************************/
  printf("%f\n\n", distBetweenPoints);
  for (unsigned long i = 0; i < length; i++) {
    printf("%f\n", dataPoints[i]);
  }
  printf("\n");
  for (unsigned long i = 0; i < length; i++) {
    printf("%f\n", *(sinusoid->data + i));
  }
  printf("\n");
  /*******************************************************************/

  return ccNoError;
}
