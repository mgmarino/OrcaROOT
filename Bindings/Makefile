
SHLIB = libPyOrcaROOT.$(SOSUFFIX)

ORFLAGS = -I../Util -I../Decoders -I../IO -I../Processors -I../Management -I$(PYROOT_API_DIR) -I$(PYTHON_API_DIR)
ORLIBS = -L../lib -lORUtil -lORDecoders -lORIO -lORProcessors -lORManagement -lPyROOT -L$(PYTHONLIBDIR) -l$(PYTHONLIB) 

include ../buildTools/BasicMakefile
