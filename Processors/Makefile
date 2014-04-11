
SHLIB = libORProcessors.$(SOSUFFIX)
    
ORFLAGS = $(patsubst %,-I$(INSTALLDIR)/%,Util Decoders IO)
ORLIBS = -L$(INSTALLDIR)/lib -lORUtil -lORDecoders -lORIO

include ../buildTools/BasicMakefile
