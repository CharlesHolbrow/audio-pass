#import <stdio.h>
#import "CCRing.h"

int main(int argc, char** argv) {
  int size = 16;

  CCRing* ring = createRing(size);
  float* pData = ring->data;

  for (int i = 0; i < size; i++) {
    *(pData++) = i * 2;
  }

  pData = ring->data;
  for (int j = 0; j < size; j++) {
    printf("value %2d: %4.1f\n", j, *(pData++));
  }

  return 1;
}
