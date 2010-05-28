// OrcaRootHeader.hh //
// Created by Sanshiro Enomoto on 1 March 2010. //
// Updated by Sanshiro Enomoto on 24 March 2010. //
// Updated by Sanshiro Enomoto on 1 April 2010. //


#include <iostream>
#include <TFile.h>
#include <TString.h>
#include <TObjString.h>
#include "Plist.hh"
#include "OrcaRootHeader.hh"

using namespace std;


static const char* HeaderObjectName = "headerXML";


TOrcaRootHeader::TOrcaRootHeader(TFile& OrcaRootFile)
{
    TObjString* Header = GetHeaderObjStringFromFile(OrcaRootFile);
    if (Header) {
	ParseXmlString(Header->GetString().Data());
    }
}

TOrcaRootHeader::TOrcaRootHeader(TObjString& OrcaRootHeaderObjString)
{
    ParseXmlString(OrcaRootHeaderObjString.GetString().Data());
}

TOrcaRootHeader::TOrcaRootHeader(TString& OrcaRootHeaderString)
{
    ParseXmlString(OrcaRootHeaderString.Data());
}

TOrcaRootHeader::~TOrcaRootHeader()
{
}

TObjString* TOrcaRootHeader::GetHeaderObjStringFromFile(TFile& OrcaRootFile)
{
    TObjString* Header = (
	dynamic_cast<TObjString*>(OrcaRootFile.Get(HeaderObjectName))
    );

    // returns 0 on error //

    return Header;
}
