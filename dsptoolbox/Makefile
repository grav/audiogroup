LIBS = -lsamplerate -lsndfile -lfftw3f -lao

all: main opg10 filtertest modulatetest

main: klt.cc dct.cc dft.cc wavread.cc wavwrite.cc samples.cc resample.cc main.cc
	g++ ${LIBS} klt.cc dct.cc dft.cc wavread.cc wavwrite.cc samples.cc resample.cc main.cc -o dsptoolbox

opg10: klt.cc dft.cc samples.cc opg10.cc
	g++ ${LIBS} klt.cc dft.cc samples.cc opg10.cc -o opg10

filtertest: wavread.cc wavwrite.cc conv.cc samples.cc dft.cc filtertest.cc wavplay.cc
	g++ ${LIBS} -g wavread.cc wavwrite.cc conv.cc samples.cc dft.cc filtertest.cc wavplay.cc -o filtertest normalize.cc

modulatetest: modulatetest.cc wavread.cc wavplay.cc modulate.cc samples.cc normalize.cc
	g++ ${LIBS} modulatetest.cc wavread.cc wavplay.cc modulate.cc samples.cc normalize.cc -o modulatetest

clean:
	rm -f dsptoolbox opg10 filtertest