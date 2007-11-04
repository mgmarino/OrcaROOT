// ORTrigger32GTIDTreeWriter.cc

#include "ORTrigger32GTIDTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTrigger32GTIDTreeWriter::ORTrigger32GTIDTreeWriter(ORVTrigger32GTIDDecoder* decoder, string treeName) :
ORVTreeWriter(decoder, treeName)
{
  fTrigger32GTIDDecoder = decoder;
  fTriggerBits = 0;
  fGTID = 0;
}

ORDataProcessor::EReturnCode ORTrigger32GTIDTreeWriter::InitializeBranches()
{
  fTree->Branch("gTrigBits", &fTriggerBits, "gTrigBits/i");
  fTree->Branch("GTID", &fGTID, "GTID/i");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORTrigger32GTIDTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  fGTID = fTrigger32GTIDDecoder->GTIDOf(record);
  fTriggerBits = fTrigger32GTIDDecoder->EventTriggerBitsOf(record);

  // check severity to improve speed
  if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): bits = " << fTriggerBits << ", gtid = " << fGTID << endl;
  }

  return kSuccess;
}

