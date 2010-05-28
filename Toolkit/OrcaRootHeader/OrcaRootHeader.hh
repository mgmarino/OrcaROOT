// OrcaRootHeader.hh //
// Created by Sanshiro Enomoto on 1 March 2010. //
// Updated by Sanshiro Enomoto on 24 March 2010. //


#ifndef OrcaRootHeader_hh__
#define OrcaRootHeader_hh__ 1


#include <TFile.h>
#include <TObjString.h>
#include <TString.h>
#include "Plist.hh"


class TOrcaRootHeader: public TPlist {
  public:
    TOrcaRootHeader(TFile& OrcaRootFile);
    TOrcaRootHeader(TObjString& OrcaRootHeaderObjString);
    TOrcaRootHeader(TString& OrcaRootHeaderString);
    virtual ~TOrcaRootHeader();
  public:
    static TObjString* GetHeaderObjStringFromFile(TFile& OrcaRootFile);
};


#endif
