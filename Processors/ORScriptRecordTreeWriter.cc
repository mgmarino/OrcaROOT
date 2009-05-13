
#include "ORScriptDecoderForRecord.hh"
#include "ORScriptRecordTreeWriter.hh"
#include "TString.h"
#include <iostream>
using namespace std;

ORScriptRecordTreeWriter::ORScriptRecordTreeWriter(string treeName) : 
  ORVTreeWriter(new ORScriptDecoderForRecord, treeName)
{
  fRecordDecoder = dynamic_cast<ORScriptDecoderForRecord*>(fDataDecoder); 
  Clear();
  fXMLCharString = new TString;
}

ORScriptRecordTreeWriter::~ORScriptRecordTreeWriter()
{
  delete fXMLCharString;
}

ORDataProcessor::EReturnCode ORScriptRecordTreeWriter::InitializeBranches()
{
  fTree->Branch("xmlRecord", "TString", &fXMLCharString); 
  return kSuccess;
}

ORDataProcessor::EReturnCode ORScriptRecordTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  if (GetScriptID() == 0 || GetScriptID() == fRecordDecoder->ScriptObjectIDOf(record)) {
    return ProcessMyScriptRecord(record);
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORScriptRecordTreeWriter::ProcessMyScriptRecord(UInt_t* record)
{ 
  *fXMLCharString = fRecordDecoder->XMLPlistOf(record).GetRawXML();
  cout << *fXMLCharString << endl;
  return kSuccess;
}
