include ../buildTools/config.mk

SHLIB = libORIO.$(SOSUFFIX)
    
SOURCES = $(wildcard OR*.cc)
HEADERS = $(wildcard OR*.hh)
OBJECTS = $(SOURCES:.cc=.o)

ORFLAGS = -I../Util -I../Decoders
ORLIBS = -L../lib -lORUtil -lORDecoders 

all: depend $(SHLIB)

.depend depend:
	@echo Checking dependencies...
	@g++ -M $(CXXFLAGS) ${ROOTFLAGS} $(ORFLAGS) *.cc > .depend

$(SHLIB): $(OBJECTS) 
	$(SOMAKER) $(SOFLAGS) -o $(SHLIB) $(OBJECTS) $(ORLIBS) $(ROOTLIBS)

.cc.o: 
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) $(ORFLAGS) -c $<

clean:
	rm -f $(SHLIB) *.o *~ .depend

-include .depend

