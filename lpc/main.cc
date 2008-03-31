#include <sndfile.hh>
#include "lpc.h"

#include <math.h>

// For getopt_long and friends
#include <getopt.h>

// Default values
#define WINDOW_SIZE 20
#define THRESHOLD   0.065
#define COEFS       20

// Global samplerate
int g_fs;
float g_threshold = THRESHOLD;
int g_window_size_samples;

static const char usage_str[] =
"Usage: %s [options] inputfile outputfile\n"
"Options:\n"
"  -w  --windowsize ms   Set the window size to ms (default %d).\n"
"  -c  --coefficients c  Set the number of coefficients to use with the lpc to c (default %d).\n"
"  -t  --threshold t     Set the pitched/unpitched threashold to t (default %.3f).\n"
"  -h, --help            Print this message and exit.\n"
;

void print_usage(char *program)
{
	printf(usage_str, program, WINDOW_SIZE, COEFS, THRESHOLD);
}

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
			print_usage(argv[0]);
			return 0;

    default:
      break;
    }
  }

  char *ifilename = NULL;
  char *ofilename = NULL;

	while (optind < argc) {
		if(ifilename == NULL) ifilename = argv[optind++];
		else if(ofilename == NULL) ofilename = argv[optind++];
		else optind++;
	}

  if(ifilename == NULL || ofilename == NULL) {
		print_usage(argv[0]);
		return 1;
  }

  lpc_data lpc = lpc_create();

  float coefs[num_coefs];
  float power;
  float pitch;

  SndfileHandle isfh(ifilename);
  g_fs = isfh.samplerate();
	float filesize = isfh.seek(0, SEEK_END);
	float total = 0.0;
	isfh.seek(0, SEEK_SET);

	g_window_size_samples = (int)((float)g_fs * ((float)window_size_ms / 1000.0));

  SAMPLE x[g_window_size_samples];
  SAMPLE y[g_window_size_samples];

  printf("Running with windowsize: %dms (%d samples)\n", window_size_ms, g_window_size_samples);
  printf("Running with %d coefficients\n", num_coefs);

  SndfileHandle osfh(ofilename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, g_fs);

  while(isfh.read(x, g_window_size_samples) != 0) {
    lpc_analyze(lpc, x, g_window_size_samples, coefs, num_coefs, &power, &pitch);
    lpc_synthesize(lpc, y, g_window_size_samples, coefs, num_coefs, power, pitch);
    printf("Pitch %.2f \tPower: %.8f \tDone: %.2f%\r", pitch, power, total / filesize * 100.0); fflush(stdout);
    osfh.write(y, g_window_size_samples);
		total += g_window_size_samples;
  }
	printf("\nDone\n");
  
  lpc_destroy(lpc);

  return 0;
}
