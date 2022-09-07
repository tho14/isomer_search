#
#  Establish the compilers 
#
CXX=g++

THREADFIT=/projects/e16032/threadfit
DDAS_INC=/usr/opt/ddas/2.2/include
DDAS_LIB=/usr/opt/ddas/2.2/lib

#CXXFLAGS=-Wall -Og -fPIC -MD -I. -I$(DDAS_INC) -I$(THREADFIT)
CXXFLAGS=-Wall -g -O2 -fPIC -MD -I. -I$(DDAS_INC) -I$(THREADFIT)
CCFLAGS=$(CXXFLAGS)

#LDFLAGS=-L/usr/lib -lpthread -lrt
LDFLAGS=-L/usr/lib	

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTLDFLAGS  := $(shell root-config --ldflags)

LIBS=-lm $(ROOTGLIBS) -L. -I. -Wl,-rpath,$(PWD) -L$(PWD) -lTParameters -Wl,-rpath,$(DDAS_LIB) -L$(DDAS_LIB) -lddaschannel -L$(THREADFIT) -Wl,-rpath,$(THREADFIT) -lDDASFitClasses

CXXFLAGS+=$(ROOTCFLAGS)

O_FILES=TVariables.o TModule.o TLinearSolver.o TUnpacker.o TCorrelator.o TAnalyzer.o

all: betasort

betasort: $(O_FILES) libTParameters.so
	$(CXX) $(CXXFLAGS) betasort.cpp -o betasort $(O_FILES) $(LDFLAGS) $(LIBS) 

TParametersDictionary.cxx: TParameters.h LinkDef.h
	rm -f TParametersDictionary.cc TParametersDictionary.h
	rootcint -f $@ -c -p TParameters.h LinkDef.h

libTParameters.so: TParametersDictionary.cxx TParameters.cpp 
	$(CXX) -fPIC -shared -o$@ $(ROOTLDFLAGS) $(CXXFLAGS) $^

depend:
	makedepend $(CXXFLAGS) *.cpp *.c

clean:
	rm -f *.o *.d betasort libTParameters.so TParametersDictionary*

-include *.d
# DO NOT DELETE
