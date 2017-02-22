#include <stdlib.h>
#include <string.h> // memset
#include "CCRing.h"
#include <stdio.h>


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
  ring->index_ring = length;

  // initialize all data to zero
  memset(ring->data, 0, dataSize);
  return ring;
}


CCError freeRing(CCRing* pRing) {
  free(pRing->data);
  free(pRing);
  return ccNoError;
}


int ccAppend(CCRing* ring, ccAudioDataType array[], unsigned long length) {

   unsigned long index = ring->index_ring - ring->length;

   for (unsigned long i = 0; i < length; ++i) {
      unsigned long n = (index + i) % ring->length;
      ring->data[n] = array[i];

      if (i==(length - 1) ){
        index = ((index + i + 1) % ring->length);
        ring->index_ring = index + ring->length;
      }
   }
  return 1;
}

unsigned long ccValidLen(CCRing* ring, unsigned long tap) {
  // Last position appended
  unsigned long append_index = ring->index_ring;
  

}