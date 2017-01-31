#ifndef CCTAP_H
#define CCTAP_H

#include <stdlib.h>


typedef struct CCTap {
  unsigned long tapIndex;
} CCTest;

CCTap* createTap(CCRing* ring);

#endif
