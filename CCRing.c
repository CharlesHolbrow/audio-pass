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
  ring->index_append = 0;
  ring->index_read = 0;
  ring->buffer = 0;

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

  unsigned long index_append = ring->index_append;

  for (unsigned long i = 0; i < length; ++i) {
    unsigned long n = (index_append + i) % ring->length;
    ring->data[n] = array[i];

    if (i==(length - 1) ){
    index_append = ((index_append + i + 1) % ring->length);
    ring->index_append = index_append;
    }
  }

  return 1;
}

ccAudioDataType ccRead(CCRing* ring, unsigned long length){

  unsigned long index_read = ring->index_read;
  unsigned long index_append = ring->index_append;
  ++ring->index_read;

  if (index_append > index_read) {
    ring->buffer = index_append - index_read;
    printf("%s %lu\n", "index read is:", index_read);
    printf("%s %lu\n", "index append is:", index_append);
    printf("<(\")\n");
    printf("%s %lu\n", "buffer is!!!:", ring->buffer);
  }
  else{
    ring->buffer = length - index_read + index_append;
    printf("%s %lu\n", "index read is:", index_read);
    printf("%s %lu\n", "index append is:", index_append);
    printf("%s %lu\n", "length is:", length);
    printf("%s %lu\n", "buffer is!!!:", ring->buffer);
    printf("\nread is:");
  }

  return ring->data[index_read];
}