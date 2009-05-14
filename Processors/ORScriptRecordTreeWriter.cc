
#include "ORScriptDecoderForRecord.hh"
#include "ORScriptRecordTreeWriter.hh"
using namespace std;

ORScriptRecordTreeWriter::ORScriptRecordTreeWriter(string treeName) : 
  ORVTreeWriter(new ORScriptDecoderForRecord, treeName)
{
  fRecordDecoder = dynamic_cast<ORScriptDecoderForRecord*>(fDataDecoder); 
  fXMLCharString = new TObjString;
  Clear();
}

ORScriptRecordTreeWriter::~ORScriptRecordTreeWriter()
{
  delete fXMLCharString;
}

ORDataProcessor::EReturnCode ORScriptRecordTreeWriter::InitializeBranches()
{
  fTree->Branch("xmlRecord", "TObjString", &fXMLCharString); 
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
  fXMLCharString->SetString(fRecordDecoder->XMLPlistOf(record).GetRawXML());
  return kSuccess;
}
