// ORTrigger32ClockTreeWriter.cc

#include "ORTrigger32ClockTreeWriter.hh"

#include "ORLogger.hh"

using namespace std;

ORTrigger32ClockTreeWriter::ORTrigger32ClockTreeWriter(string treeName) :
ORVTreeWriter(new ORTrigger32ClockDecoder, treeName)
{
  fTrigger32ClockDecoder = dynamic_cast<ORTrigger32ClockDecoder*>(fDataDecoder); // just renaming
  fTriggerBitsFilter = kBothTriggers;
  fTriggerBits = 0;
  fT_s = 0.0;
}

ORTrigger32ClockTreeWriter::~ORTrigger32ClockTreeWriter()
{
  delete fTrigger32ClockDecoder;
}

ORDataProcessor::EReturnCode ORTrigger32ClockTreeWriter::InitializeBranches()
{
  fTree->Branch("trigBits", &fTriggerBits, "trigBits/i");
  fTree->Branch("t_s", &fT_s, "t/D");
  return kSuccess;
}

ORDataProcessor::EReturnCode ORTrigger32ClockTreeWriter::ProcessMyDataRecord(UInt_t* record)
{
  if(!(fTrigger32ClockDecoder->EventTriggerBitsOf(record) & fTriggerBitsFilter)) {
    // don't write to tree if we are filtering out the trigger records of
    // one or the other trigger
    return kFailure;
  }
  fTriggerBits = fTrigger32ClockDecoder->EventTriggerBitsOf(record);
  fT_s = fTrigger32ClockDecoder->GetMacTime_s(record);

  // check severity to improve speed
  if(ORLogger::GetSeverity() >= ORLogger::kDebug) { 
    ORLog(kDebug) << "ProcessMyDataRecord(): bits = " << fTriggerBits << ", t = " << fT_s << endl;
  }

  return kSuccess;
}

