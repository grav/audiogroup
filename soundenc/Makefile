LIBS = -lsamplerate -lsndfile -lfftw3f -lao
SRC = wavread.cc conv.cc samples.cc wavplay.cc soundenc.cc

all: ${SRC}
	g++ ${LIBS} -g ${SRC} -o soundenc

clean:
	rm -f *~ soundenc