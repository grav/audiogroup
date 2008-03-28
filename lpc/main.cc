#include <sndfile.hh>
#include "lpc.h"

#include <math.h>

#define WINDOW_SIZE (int)(1.7 * 882) // 882 frames = 20ms
#define COEFS 32

int main(int argc, char *argv[])
{
  if(argc < 3) {
    printf("Usage: %s [intput] [output]\n", argv[0]);
    return 1;
  }

  char *ifilename = argv[1];
  char *ofilename = argv[2];

  lpc_data lpc = lpc_create();

  SAMPLE x[WINDOW_SIZE];
  SAMPLE y[WINDOW_SIZE];
  float coefs[COEFS];
  float power;
  float pitch;

  SndfileHandle isfh(ifilename);
  SndfileHandle osfh(ofilename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, 44100);

  int cnt = 0;
  while(isfh.read(x, WINDOW_SIZE) != 0) {
    lpc_analyze(lpc, x, WINDOW_SIZE, coefs, COEFS, &power, &pitch);
    lpc_synthesize(lpc, y, WINDOW_SIZE, coefs, COEFS, power * 2, pitch);
    printf("pitch %f\n", pitch);
    osfh.write(y, WINDOW_SIZE);
    cnt++;
  }
  
  lpc_destroy(lpc);

  return 0;
}
