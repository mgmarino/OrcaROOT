// ORIpeV4SLTEventTreeWriter.cc

#include "ORIpeV4SLTEventTreeWriter.hh"
#include "ORLogger.hh"

using namespace std;

ORIpeV4SLTEventTreeWriter::ORIpeV4SLTEventTreeWriter(string treeName) :
ORVTreeWriter(new ORIpeV4SLTEventDecoder, treeName)
{
  fEventDecoder = dynamic_cast<ORIpeV4SLTEventDecoder*>(fDataDecoder);
  Clear();
}

ORIpeV4SLTEventTreeWriter::~ORIpeV4SLTEventTreeWriter()
{
  delete fEventDecoder;
}

ORDataProcessor::EReturnCode ORIpeV4SLTEventTreeWriter::InitializeBranches()
{
  fTree->Branch("crate", &fCrate, "crate/s");
  fTree->Branch("card", &fCard, "card/s");
  fTree->Branch("counterType", &fCounterType, "counterType/s");
  fTree->Branch("counterSubType", &fCounterSubType, "counterSubType/s");
  fTree->Branch("eventCounter", &fEventCounter, "eventCounter/i");
  fTree->Branch("timestampHigh", &fTimestampHigh, "timestampHigh/i");
  fTree->Branch("timestampLow", &fTimestampLow, "timestampLow/i");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORIpeV4SLTEventTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  // the event decoder could run into a problem, but this might not
  // ruin the rest of the run.
      // check severity to improve speed:
  fCrate = fEventDecoder->CrateOf(record);
  fCard = fEventDecoder->CardOf(record);
  
  fCounterType = fEventDecoder->CounterTypeOf(record);
  fCounterSubType = fEventDecoder->CounterSubTypeOf(record);

  fEventCounter  = fEventDecoder->EventCounterOf(record);
  fTimestampHigh = fEventDecoder->TimestampHighOf(record);
  fTimestampLow  = fEventDecoder->TimestampLowOf(record);
  
  
  if (ORLogger::GetSeverity() >= ORLogger::kDebug) 
  { 
    ORLog(kDebug) << "ProcessMyDataRecord(): "
      << "sec-subsec-ID-crate-card-counterType-counterSubType-eventCounter-timestampHigh-timestampLow = "
      << fCrate << "-"
      << fCard << "-" 
      << fCounterType << "-" 
      << fCounterSubType << "-" 
      << fEventCounter << "-" 
      << fTimestampHigh << "-" 
	  << fTimestampHigh 
      << endl;
  }
  return kSuccess;
}


