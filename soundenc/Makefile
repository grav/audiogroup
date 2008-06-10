LIBS = -lsamplerate -lsndfile -lfftw3f -lao -L/opt/local/lib -I/opt/local/include -lm
SRC = wavread.cc conv.cc samples.cc wavplay.cc soundenc.cc normalize.cc threshold.cc wavwrite.cc

all: ${SRC}
	g++ ${LIBS} -g ${SRC} -o soundenc

clean:
	rm -f *~ soundenc