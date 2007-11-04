#include "ORLogger.hh"
#include "TFile.h"
#include "TObjString.h"
#include "ORHeader.hh"

using namespace std;

/*
  This exmaple program demonstrates how to reload an ORHeader with the
  raw xml stored as a TObjString in an output file created by OrcaRoot.
*/

int main(int argc, char** argv)
{
  if(argc != 2) {
    ORLog(kError) << "Usage: " << argv[0] << " [root file]" << endl;
    return 1;
  }

  TFile* orFile = TFile::Open(argv[1]);
  if(orFile == NULL) return 1;

  TObjString* headerOS = (TObjString*) orFile->Get("headerXML");
  ORHeader header(headerOS->GetString().Data(), headerOS->GetString().Length());

  cout << header.GetDictionary()->GetName() << endl;
  ORDictValueA* ncdGeometry = (ORDictValueA*) header.LookUp("NcdModel:NcdDetector:Geometry");
  cout << ncdGeometry->GetName() << ":" << endl;
  for(size_t i=0; i<ncdGeometry->GetNValues(); i++) {
    cout << ((ORDictValueS*) ncdGeometry->At(i))->GetS() << endl;
  }

  return 0;
}

