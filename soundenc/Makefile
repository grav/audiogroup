LIBS = -lsamplerate -lsndfile -lfftw3f -lfftw3f_threads -lao -L/opt/local/lib -I/opt/local/include -lm
SRC = wavread.cc conv.cc samples.cc wavplay.cc soundenc.cc normalize.cc threshold.cc wavwrite.cc quantizer.cc config.cc biquadthreshold.cc dft.cc ath.cc

all: ${SRC}
	g++ ${LIBS} -g ${SRC} -o soundenc

biquadtest:
	g++ ${LIBS} biquadtest.cc dft.cc samples.cc normalize.cc -o bqt

clean:
	rm -f *~ soundenc