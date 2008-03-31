#include <sndfile.hh>
#include "lpc.h"

#include <math.h>

// For getopt_long and friends
#include <getopt.h>

// Global samplerate
int g_fs;
float g_threshold = 0.065;

//#define WINDOW_SIZE (int)(1.7 * 882) // 882 frames = 20ms
//#define WINDOW_SIZE 882 // 882 frames = 20ms
//#define WINDOW_SIZE 2048 // 882 frames = 20ms
//#define WINDOW_SIZE_MS 20

#define WINDOW_SIZE (int)((float)g_fs * ((float)window_size_ms / 1000.0))

static const char usage_str[] =
"Usage: %s [options] inputfile outputfile\n"
"Options:\n"
"  -w  --windowsize ms   Set the window size to ms (default 20).\n"
"  -c  --coefficients c  Set the number of coefficients to use with the lpc to c (default 12).\n"
"  -t  --threshold t     Set the pitched/unpitched threashold to t (default 0.065).\n"
"  -h, --help            Print this message and exit.\n"
;



int main(int argc, char *argv[])
{
  int c;
  int option_index = 0;
  int window_size_ms = 20;
  int num_coefs = 12;

  while(1) {
    int this_option_optind = optind ? optind : 1;
    static struct option long_options[] = {
      {"coefficients", required_argument, 0, 'c'},
      {"windowsize", required_argument, 0, 'w'},
      {"threshold", required_argument, 0, 't'},
      {"help", no_argument, 0, 'h'},
      {"version", no_argument, 0, 'v'},
      {0, 0, 0, 0}
    };
    
    c = getopt_long (argc, argv, "hw:c:t:", long_options, &option_index);
    
    if (c == -1)
      break;

    switch(c) {
    case 'w':
      window_size_ms = atoi(optarg);
      break;

    case 'c':
      num_coefs = atoi(optarg);
      break;

    case 't':
      g_threshold = atof(optarg);
      break;

    case '?':
    case 'h':
      printf(usage_str, argv[0]);
      return 0;

    default:
      break;
    }
  }

  char *ifilename = NULL;
  char *ofilename = NULL;

  if (optind < argc) {
    while (optind < argc) {
      if(ifilename == NULL) ifilename = argv[optind++];
      if(ofilename == NULL) ofilename = argv[optind++];
    }
  }

  if(ifilename == NULL || ofilename == NULL) {
      printf(usage_str, argv[0]);
      return 1;
  }

  lpc_data lpc = lpc_create();

  float coefs[num_coefs];
  float power;
  float pitch;

  SndfileHandle isfh(ifilename);
  g_fs = isfh.samplerate();

  SAMPLE x[WINDOW_SIZE];
  SAMPLE y[WINDOW_SIZE];

  printf("Running with windowsize: %dms (%d samples)\n", window_size_ms, WINDOW_SIZE);
  printf("Running with %d coefficients\n", num_coefs);

  SndfileHandle osfh(ofilename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, g_fs);

  while(isfh.read(x, WINDOW_SIZE) != 0) {
    lpc_analyze(lpc, x, WINDOW_SIZE, coefs, num_coefs, &power, &pitch);
    lpc_synthesize(lpc, y, WINDOW_SIZE, coefs, num_coefs, power, pitch);
    printf("pitch %f\n", pitch);
    osfh.write(y, WINDOW_SIZE);
  }
  
  lpc_destroy(lpc);

  return 0;
}
