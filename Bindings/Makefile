
SHLIB = libPyOrcaROOT.$(SOSUFFIX)

ORFLAGS = $(patsubst %,-I$(INSTALLDIR)/%,Util Decoders IO Processors Management) -I$(PYROOT_API_DIR) -I$(PYTHON_API_DIR)
ORLIBS = -L$(INSTALLDIR)/lib -lORUtil -lORDecoders -lORIO -lORProcessors -lORManagement -lPyROOT -L$(PYTHONLIBDIR) -l$(PYTHONLIB) 

include ../buildTools/BasicMakefile
