#ifndef CCRING_H
#define CCRING_H

#include <stdlib.h>

/* I'm making the assumption here that this is the same data
type that will be passed into our audio processing callback.
at the time of writing, this is paFloat32 as defined by
portaudio */
typedef float ccAudioDataType;

typedef struct CCRing {
  ccAudioDataType* data;
  unsigned long length;
  unsigned long index;
} CCRing;

typedef enum CCError {
  ccNoError = 0,
  ccError = -1000,
  ccUndersizedRing
} CCError;

/* Create and initialize a CCRing struct.
On Success, return a CCRing pointer
On Failure, return NULL */
CCRing* createRing(unsigned long length);

/* Free a memory allocated by createRing
On Success, return ccNoError

Note: Calling free on NULL has no effect. The behavior of
calling free on an unfreeable pointer is undefined. I do not
think that this should ever fail - its behavior will either be
undefined, or it will succeed. */
CCError freeRing(CCRing* pRing);

/* ccAppend takes an array and adds it to the Ring. It also
records a pointer where the last change was made to the Ring
and appends at the position of the pointer with each new
append. */
CCError ringAppend(CCRing* pRing, ccAudioDataType arr[], unsigned long length);
/*int ccAppend(CCRing* appendRing, ccAudioDataType array[], unsigned long length);*/

/* ccGenerateSin takes in a pointer to a CCRing, and the # of cycles
for the resulting standard sinusoid (A = 1, phi = 0). It returns evenly 
spaced sin data points for each index in the Ring. */
CCError ccGenerateSin(CCRing* ring, double cycles);

#endif
