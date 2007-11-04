// ORBasicRDTreeWriter.cc

#include "ORBasicRDTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORBasicRDTreeWriter::ORBasicRDTreeWriter(ORVBasicRDTreeDecoder* decoder, string treeName, string branchPrefix) :
ORBasicTreeWriter(decoder, treeName, branchPrefix)
{
  fRDDecoder = decoder;
  fRefDate = 0.0;
}

ORDataProcessor::EReturnCode ORBasicRDTreeWriter::InitializeBranches()
{
  fTree->Branch("refDate", &fRefDate, "refDate/D");
  return ORBasicTreeWriter::InitializeBranches();
}

ORDataProcessor::EReturnCode ORBasicRDTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fRefDate = fRDDecoder->ReferenceDateOf(record);
  return ORBasicTreeWriter::ProcessMyDataRecord(record);
}

void ORBasicRDTreeWriter::Clear()
{
  ORBasicTreeWriter::Clear();
  fRefDate = 0.0;
}
