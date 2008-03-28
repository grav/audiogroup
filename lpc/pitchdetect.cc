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

#define LOW_MS   0.0025 // ms
#define HIGH_MS  0.0195 // ms
#define USTEMT 0.025

float pitchdetect(SAMPLE *x, int size)
{
  int fs = 44100;
  SAMPLE min = 1000000;
  int minkey = 0;

  for(int p = (int)(LOW_MS * fs); p < (int)(HIGH_MS * fs); p++) {
    SAMPLE sum = 0;
    for(int i = p; i < size; i++) {
      sum += fabs(x[i] - x[i-p]) / size;
    }
    //    sum /= size;
    if(sum < min) {
      min = sum;
      minkey = p;
    }
  }
  //  return fs / (minkey * 3);

  if(min > USTEMT) return 0;

  while(minkey > 500) minkey /= 2;

  return minkey;
}
