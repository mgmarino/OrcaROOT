
SHLIB = libORIO.$(SOSUFFIX)
    
ORFLAGS = $(patsubst %,-I$(INSTALLDIR)/%,Util Decoders)
ORLIBS = -L$(INSTALLDIR)/lib -lORUtil -lORDecoders 

include ../buildTools/BasicMakefile
