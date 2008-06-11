LIBS = -lsamplerate -lsndfile -lfftw3f -lao -L/opt/local/lib -I/opt/local/include -lm
SRC = wavread.cc conv.cc samples.cc wavplay.cc soundenc.cc normalize.cc threshold.cc wavwrite.cc quantizer.cc config.cc

all: ${SRC}
	g++ ${LIBS} -g ${SRC} -o soundenc

biquadtest:
	g++ biquadtest.cc -o bqt

clean:
	rm -f *~ soundenc