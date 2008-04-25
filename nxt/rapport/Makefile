SOURCE=preamble.tex
TARGET=nxt
FLAGS=
GFXSRC=tegning
#-c-style-errors
PNGs := $(patsubst %.png,%.eps, $(wildcard *.png))
FIGs := $(patsubst %.fig,%.eps, $(wildcard *.fig))
.PHONY =dvi ps pdf mpost clean cleanall

dvi: $(TARGET).tex $(PNGs) $(FIGs)
	latex $(FLAGS) $(TARGET)
	latex $(FLAGS) $(TARGET)

ps: dvi
	dvips $(TARGET).dvi -o

pdf: $(TARGET).tex $(PNGs) $(FIGs)
#	ps2pdf14 $(TARGET).ps $(TARGET).pdf
	pdflatex $(FLAGS) $(TARGET)
	pdflatex $(FLAGS) $(TARGET)


$(TARGET).tex:
	ln -s $(SOURCE) $(TARGET).tex

mpost: 
	mpost $(GFXSRC).mp && latex $(GFXSRC).tex && dvips $(GFXSRC).dvi -o

clean:
	rm -f \#*
	rm -f *~
	rm -f *.log
	rm -f *.toc
	rm -f *.aux

cleanall: clean
	rm -f $(TARGET).dvi
	rm -f $(TARGET).ps
	rm -f $(TARGET).pdf
