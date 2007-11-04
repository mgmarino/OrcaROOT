CXXFLAGS = -O2  -W -Wall 
LDFLAGS = -bind_at_load
ORBIG_ENDIAN_MACHINE = 

SOMAKER = g++
SOFLAGS =  -dynamiclib -Wl
SOSUFFIX = dylib

ROOTCONF = /sw/bin/root-config 
ROOTFLAGS = $(shell $(ROOTCONF) --cflags)
ROOTLIBS = $(shell $(ROOTCONF) --libs) -lXMLParser

