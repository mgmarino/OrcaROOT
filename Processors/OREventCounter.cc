// OREventCounter.cc

#include "OREventCounter.hh"

#include "ORLogger.hh"
#include "TROOT.h"
#include "TParameter.h"

using namespace std;

OREventCounter::OREventCounter(ORVEventCounterDecoder* eventCounterDecoder) :
ORDataProcessor(eventCounterDecoder)
{
  fEventCounterDecoder = eventCounterDecoder;
  fEventCount = 0;
}

ORDataProcessor::EReturnCode OREventCounter::StartRun()
{
  fEventCount = 0;
  return kSuccess;
}

ORDataProcessor::EReturnCode OREventCounter::ProcessMyDataRecord(UInt_t* record)
{
  fEventCount += fEventCounterDecoder->GetEventCount(record);
  return kSuccess;
}

ORDataProcessor::EReturnCode OREventCounter::EndRun()
{
  string name = fEventCounterDecoder->GetDataObjectPath();
  for(size_t i = 0; i < name.size(); i++) {
    if(name[i] == ':') name[i] = '_';
  }
  name += "_Count";
  TParameter<int> eventCount(name.c_str(), fEventCount);
  eventCount.Write();
  return kSuccess;
}

