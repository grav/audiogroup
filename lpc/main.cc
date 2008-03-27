#include <sndfile.hh>
#include "lpc.h"

#define WINDOW_SIZE 100
#define COEFS 32

int main()
{
	char ifilename[] = "input.wav";
	char ofilename[] = "output.wav";

	lpc_data lpc = lpc_create();

	SAMPLE x[WINDOW_SIZE];
	SAMPLE y[WINDOW_SIZE];
	float coefs[COEFS];
	float power;
	float pitch;


	SndfileHandle isfh(ifilename);
	SndfileHandle osfh(ofilename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, 1, 44100);
	
	while(isfh.read(x, WINDOW_SIZE) != 0) {
		lpc_analyze(lpc, x, WINDOW_SIZE, coefs, COEFS, &power, &pitch);
		lpc_synthesize(lpc, y, WINDOW_SIZE, coefs, COEFS, power, pitch);
		osfh.write(y, WINDOW_SIZE);
	}

	lpc_destroy(lpc);

	return 0;
}
