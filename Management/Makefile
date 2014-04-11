
SHLIB = libORManagement.$(SOSUFFIX)
    
ORFLAGS = $(patsubst %,-I$(INSTALLDIR)/%,Util Decoders IO Processors)
ORLIBS = -L$(INSTALLDIR)/lib -lORUtil -lORDecoders -lORIO -lORProcessors

include ../buildTools/BasicMakefile
