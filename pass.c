#include <stdio.h>
#include <stdlib.h>

<<<<<<< HEAD
#include "kiss_fftr.h"
=======
#include "portaudio.h"
>>>>>>> 7e9fa3c97dda44e205f3e1d6c4581bc64bb6b33e
#include "CCRing.h"

#define SAMPLE_RATE (44100)

typedef struct
{
  CCRing* ring;
  unsigned long tap;
}   
paTestData;


/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
  // Why does this line of code cause problems? If it is placed before modifying
  // the out buffer, the out buffer contains silence. It seems quite strange
  // that merely declaring a pointer would have this effect.
  //CCRing *ring;
  // We cannot declare any other variables without causing this problem. I don't
  // know why there is an exception for casting other variables.
  //
  // However, it seems to work fine if the variables are static.

  const float *in = (const float*)inputBuffer;
  float *out = (float*)outputBuffer;

  paTestData* data = (paTestData*) userData;
  ccAppend(data->ring, in, framesPerBuffer);

  for (int i; i< framesPerBuffer; i++){
    *out++ = *in++;
    *out++ = (*in++);
  }

  return 0;
}


int main(int argc, char** argv) {

  paTestData data;
  PaError err;
  PaStream *stream;
  
  CCRing* ring = createRing(SAMPLE_RATE * 0.1);
  if (ring == NULL){
    printf("%s", "Failed to create ring");
    return EXIT_FAILURE;
  }

  data.ring = ring;

  // get and display the Port audio version in use
  const PaVersionInfo* info = Pa_GetVersionInfo();
  printf("%s\n", info->versionText);

  err = Pa_Initialize();
  if (err != paNoError) {
    goto error;
  }

  // print out list of devices
  const PaDeviceInfo * deviceInfo;
  PaDeviceIndex numDevices = Pa_GetDeviceCount();
  printf("Found %d device(s)\n", numDevices);
  for (int i=0; i<numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo(i);
    printf("D%d: %s\n", i, deviceInfo->name);
  }

  int inDevice, outDevice;
  if (argc == 1) {
    inDevice = Pa_GetDefaultInputDevice();
    outDevice = Pa_GetDefaultOutputDevice();
  } else if (argc == 2) {
    inDevice = atoi(argv[1]);
    outDevice = inDevice;
  } else if (argc >= 3) {
    inDevice = atoi(argv[1]);
    outDevice = atoi(argv[2]);
  }

  PaStreamParameters in, out;
  const PaDeviceInfo *inInfo;
  const PaDeviceInfo *outInfo;

  inInfo = Pa_GetDeviceInfo(inDevice);
  in.device = inDevice;
  in.channelCount = 2;
  in.sampleFormat = paFloat32;
  in.suggestedLatency = inInfo->defaultLowInputLatency;
  in.hostApiSpecificStreamInfo = NULL;

  outInfo = Pa_GetDeviceInfo(outDevice);
  out.device = outDevice;
  out.channelCount = 2;
  out.sampleFormat = paFloat32;
  out.suggestedLatency = outInfo->defaultLowOutputLatency;
  out.hostApiSpecificStreamInfo = NULL;

  printf("Input:  %s\n", inInfo->name);
  printf("Output: %s\n", outInfo->name);

  /* Open an audio I/O stream. */
                     
  err = Pa_OpenStream(&stream,
                      &in,
                      &out,
                      SAMPLE_RATE,
                      256,        /* frames per buffer, i.e. the number
                                  of sample frames that PortAudio will
                                  request from the callback. Many apps
                                  may want to use
                                  paFramesPerBufferUnspecified, which
                                  tells PortAudio to pick the best,
                                  possibly changing, buffer size.*/
                      paNoFlag,
                      patestCallback, /* this is your callback function */
                      &data); /* will be passed to your callback*/

  
  if (err != paNoError) {
    goto error;
  }

  // Start playback
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    goto error;
  }

  // Wait
  Pa_Sleep(5 * 1000);
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
