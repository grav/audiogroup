LIBS = -lsamplerate -lsndfile -lfftw3f

all: main opg10 filtertest

main: klt.cc dct.cc dft.cc wavread.cc wavwrite.cc samples.cc resample.cc main.cc
	g++ ${LIBS} klt.cc dct.cc dft.cc wavread.cc wavwrite.cc samples.cc resample.cc main.cc mls.cc -o dsptoolbox

opg10: klt.cc dft.cc samples.cc opg10.cc
	g++ ${LIBS} klt.cc dft.cc samples.cc opg10.cc -o opg10

filtertest: wavread.cc wavwrite.cc conv.cc samples.cc dft.cc filtertest.cc
	g++ ${LIBS} -g wavread.cc wavwrite.cc conv.cc samples.cc dft.cc filtertest.cc -o filtertest