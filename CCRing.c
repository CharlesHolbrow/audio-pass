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

/*comments to help me think
given length inputed 
length modulo length of ring <- which index of the ring you want to modify to be */

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