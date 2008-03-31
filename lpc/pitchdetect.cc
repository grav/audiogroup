#include "pitchdetect.h"
#include <math.h>
/*
function [AMDF,min] = pitchdetect (f)
[y,fs]=wavread(f);
N=length(y);
k=0;

% lowest and highest period of human speech
l_s = 0.0025;
h_s = 0.0195;

% same in samples
l_P = round(fs * l_s);
h_P = round(fs * h_s);

AMDF = zeros(1,h_P);

for P=l_P:h_P
   sum = 0;
   for i = P+1:N
      sum = sum + abs(y(i)-y(i-P));
   end
   AMDF(P) = 1/N * sum;
end

min = min(AMDF);

*/

extern int g_fs;
extern float g_threshold;

// Samp = s * Fs
// Samp = Fs/Hz
// Hz = Fs/Samp = Fs/(s*Fs) = 1/s

#define LOW_HZ   51 
#define LOW_SAMP   (int)((float)g_fs / (float)LOW_HZ)

#define HIGH_HZ  400
#define HIGH_SAMP  (int)((float)g_fs / (float)HIGH_HZ)

#include <stdio.h>

float pitchdetect(SAMPLE *x, int size)
{
  SAMPLE min = 1000000;
  int minkey = 0;

  //for(int p = LOW_SAMP; p < HIGH_SAMP; p++) {
  //  for(int p = HIGH_SAMP; p < LOW_SAMP; p++) {
  for(int p = LOW_SAMP; p > HIGH_SAMP; p--) {
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
