#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#include "kiss_fftr.h"
#include "CCRing.h"

#define SAMPLE_RATE (44100)

typedef struct
{
  CCRing* ring;
  unsigned long tap;
}   
paTestData;

float counter = 0;


/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData )
{
  float* in = (float*)inputBuffer;
  float* out = (float*)outputBuffer;

  paTestData* data = (paTestData*) userData;

  CCRing* ring = data->ring;
  ccAppend(ring, in, framesPerBuffer);

  counter ++;

  if (counter == 15) plot(ring);

  for (int i; i< framesPerBuffer; i ++){
    out[i] = in[i];
  }
  return 0;
}



int main(int argc, char** argv) {

  paTestData data;

  const PaVersionInfo* info;
  PaError err;
  PaStream *stream;
  
  CCRing* ring = createRing(SAMPLE_RATE * 0.1);
  if (ring == NULL){
    printf("%s", "Failed to create ring");
    return EXIT_FAILURE;
  }

  data.ring = ring;

  // get and display the Port audio version in use
  info = Pa_GetVersionInfo();
  printf("%s\n", info->versionText);


  err = Pa_Initialize();
  if (err != paNoError) {
    goto error;
  }

  /* Open an audio I/O stream. */
  err = Pa_OpenDefaultStream( &stream,
                              1,          /* no input channels */
                              1,          /* stereo output */
                              paFloat32,  /* 32 bit floating point output */
                              SAMPLE_RATE,
                              256,        /* frames per buffer, i.e. the number
                                          of sample frames that PortAudio will
                                          request from the callback. Many apps
                                          may want to use
                                          paFramesPerBufferUnspecified, which
                                          tells PortAudio to pick the best,
                                          possibly changing, buffer size.*/
                              patestCallback, /* this is your callback function */
                              &data); /* This is a pointer that will be passed to
                                      your callback*/                          
  

  if (err != paNoError) {
    goto error;
  }

  // Start playback
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    goto error;
  }
  // Wait
  Pa_Sleep(10 * 1000); 
  // Stop playback
  err = Pa_StopStream(stream);
  if (err != paNoError) {
    goto error;
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    goto error;
  }
  
  err = Pa_Terminate();
  if (err != paNoError) {
    goto error;
  }

  return EXIT_SUCCESS;

  // Report any errors to our user
  error: {
    printf("PortAudio error: %s\n", Pa_GetErrorText(err));
  }
  return EXIT_FAILURE;
}
