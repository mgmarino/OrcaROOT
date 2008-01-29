// ORMultiEventCounter.cc

#include "ORMultiEventCounter.hh"

#include "OREventCounter.hh"
#include "ORLogger.hh"

using namespace std;

ORMultiEventCounter::~ORMultiEventCounter()
{
  for(size_t i=0; i < fDataProcessors.size(); i++) {
    delete fDataProcessors[i];
  }
}

void ORMultiEventCounter::AddEventCounterDecoder(ORVEventCounterDecoder* eventCounterDecoder)
{
  ORCompoundDataProcessor::AddProcessor(new OREventCounter(eventCounterDecoder));
}

void ORMultiEventCounter::AddProcessor(ORDataProcessor* processor)
{
  ORLog(kWarning) << "You may not use ORMultiEventCounter::AddProcessor(" 
                  << processor->GetDecoder()->GetDataObjectPath() << ")." 
                  << endl;
}

ORDataProcessor::EReturnCode ORMultiEventCounter::EndRun()
{
  ORCompoundDataProcessor::EndRun();
  if (fRunContext) {
    ORLog(kRoutine) << "Event counts for run " << fRunContext->GetRunNumber() << endl;
  }
  for(size_t i=0; i < fDataProcessors.size(); i++) {
    ORLog(kRoutine) << fDataProcessors[i]->GetDecoder()->GetDataObjectPath()
                    << " had " << ((OREventCounter*) fDataProcessors[i])->GetEventCount() 
                    << " events." << endl;
  }
  return kSuccess;
}

