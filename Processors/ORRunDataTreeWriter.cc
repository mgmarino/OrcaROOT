// ORRunDataTreeWriter.cc

#include "ORRunDataTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORRunDataTreeWriter::ORRunDataTreeWriter(string treeName) : ORBasicTreeWriter(new ORRunDecoder, treeName)
{
  fRunDecoder = dynamic_cast<ORRunDecoder*>(fDataDecoder);
  fByteCount = 0;
}

ORRunDataTreeWriter::~ORRunDataTreeWriter()
{
  delete fRunDecoder;
}

ORDataProcessor::EReturnCode ORRunDataTreeWriter::InitializeBranches()
{
  EReturnCode retCode = ORBasicTreeWriter::InitializeBranches();
  if(retCode >= kFailure) return retCode;
  fTree->Branch("bytesProcessed", &fByteCount, "bytes/i");
  return kSuccess;
}


ORDataProcessor::EReturnCode ORRunDataTreeWriter::ProcessDataRecord(UInt_t* record)
{
  fByteCount += fRunDecoder->LengthOf(record)*sizeof(UInt_t);
  return ORBasicTreeWriter::ProcessDataRecord(record);
}

ORDataProcessor::EReturnCode ORRunDataTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  ORBasicTreeWriter::ProcessMyDataRecord(record); // fTree is filled here
  if (fRunDecoder->IsHeartBeat(record)) fByteCount = 0;
  return kSuccess;
}

