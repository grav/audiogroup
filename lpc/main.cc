#include <sndfile.hh>
#include "lpc.h"

#include <math.h>

// Global samplerate
int g_fs;

//#define WINDOW_SIZE (int)(1.7 * 882) // 882 frames = 20ms
//#define WINDOW_SIZE 882 // 882 frames = 20ms
//#define WINDOW_SIZE 2048 // 882 frames = 20ms
#define WINDOW_SIZE_MS 20
#define WINDOW_SIZE (int)((float)g_fs * ((float)WINDOW_SIZE_MS / 1000.0))

#define COEFS 12

int main(int argc, char *argv[])
{
  if(argc < 3) {
    printf("Usage: %s [input] [output]\n", argv[0]);
    return 1;
  }

  char *ifilename = argv[1];
  char *ofilename = argv[2];

  lpc_data lpc = lpc_create();

  float coefs[COEFS];
  float power;
  float pitch;

  SndfileHandle isfh(ifilename);
  g_fs = isfh.samplerate();

  SAMPLE x[WINDOW_SIZE];
  SAMPLE y[WINDOW_SIZE];

  printf("Running with windowsize: %dms (%d samples)\n", WINDOW_SIZE_MS, WINDOW_SIZE);

  SndfileHandle osfh(ofilename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, g_fs);

  while(isfh.read(x, WINDOW_SIZE) != 0) {
    lpc_analyze(lpc, x, WINDOW_SIZE, coefs, COEFS, &power, &pitch);
    lpc_synthesize(lpc, y, WINDOW_SIZE, coefs, COEFS, power, pitch);
    printf("pitch %f\n", pitch);
    osfh.write(y, WINDOW_SIZE);
  }
  
  lpc_destroy(lpc);

  return 0;
}
