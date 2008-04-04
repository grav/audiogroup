#include "pitchdetect.h"
#include <math.h>

extern int g_fs;
extern float g_threshold;
extern int g_window_size_samples;

// Samp = s * Fs
// Samp = Fs/Hz
// Hz = Fs/Samp = Fs/(s*Fs) = 1/s

#define LOW_HZ   55

#define HIGH_SAMP (int)((float)(g_window_size_samples * 0.5))  // base on windo size
//#define HIGH_SAMP (int)((float)g_fs / (float)LOW_HZ)

#define HIGH_HZ  400

#define LOW_SAMP  (int)((float)g_fs / (float)HIGH_HZ)

#include <stdio.h>

float pitchdetect(SAMPLE *x, int size)
{
  SAMPLE min = 1000000;
  int minkey = 0;

  //  printf("HIGH_SAMP: %d\n",HIGH_SAMP);

  for(int p = HIGH_SAMP; p > LOW_SAMP; p--) {
    double sum = 0;

    for(int i = 0; i < size; i++) {
      sum += fabs(x[i] - x[(i+p)%size]);// / size - p;
    }
    sum /= size;

    //    printf("sum: %f, min: %f, blah: %f\n", sum, min, (1.0/(float)p)*10);

    if(sum < min) {// - (10.0/(float)p)) {
      min = sum;
      minkey = p;
    }
  }
  //  return fs / (minkey * 3);

  if(min > g_threshold) return 0;

  return minkey;
}


// Hz : svingninger/second
// Fs : samples/second

// Fs / Hz : samples/second * seconds/svingning => samples/svingning
