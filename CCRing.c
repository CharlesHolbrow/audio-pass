#include <stdlib.h>
#include <string.h> // memset
#include "CCRing.h"


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

  // initialize all data to zero
  memset(ring->data, 0, dataSize);
  return ring;
}


CCError freeRing(CCRing* pRing) {
  free(pRing->data);
  free(pRing);
  return ccNoError;
}
