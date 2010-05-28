
BINS=orcaroot-header-sample orcaroot-header-error-sample orcaroot-header-dump orcaroot-header-xml-dump
OBJS=PlistNode.o PlistParser.o Plist.o OrcaRootHeader.o

CXX=$(shell root-config --cxx) -g
CFLAGS=$(shell root-config --cflags)
LDFLAGS=$(shell root-config --ldflags)
LIBS=$(shell root-config --libs) -lXMLParser


all: $(BINS)

orcaroot-header-sample: orcaroot-header-sample.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@  $@.o $(OBJS) $(LIBS)

orcaroot-header-error-sample: orcaroot-header-error-sample.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@  $@.o $(OBJS) $(LIBS)

orcaroot-header-dump: orcaroot-header-dump.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@  $@.o $(OBJS) $(LIBS)

orcaroot-header-xml-dump: orcaroot-header-xml-dump.o $(OBJS)
	$(CXX) $(LDFLAGS) -o $@  $@.o $(OBJS) $(LIBS)


.cc.o:
	$(CXX) $(CFLAGS) -c $< 


clean:
	rm -f $(BINS)
	rm -f *.o
	rm -f core.*
