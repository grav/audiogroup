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

#define LOW_HZ   110
#define LOW   (int)((float)g_fs / (float)LOW_HZ) // ms

#define HIGH_HZ  860
#define HIGH  (int)((float)g_fs / (float)HIGH_HZ) // ms

#include <stdio.h>

float pitchdetect(SAMPLE *x, int size)
{
  SAMPLE min = 1000000;
  int minkey = 0;

  //for(int p = LOW; p < HIGH; p++) {
  for(int p = HIGH; p < LOW; p++) {
    double sum = 0;

    for(int i = 0; i < size - p; i++) {
      sum += fabs(x[i] - x[i+p]) / size;
    }
    //    sum /= size;

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
