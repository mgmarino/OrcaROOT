// ORBasicRunDataProcessor.cc

#include "ORBasicRunDataProcessor.hh"
#include "ORRunDecoder.hh"
#include "ORLogger.hh"

using namespace std;

ORBasicRunDataProcessor::ORBasicRunDataProcessor() : ORDataProcessor(new ORRunDecoder)
{
  fRunDecoder = dynamic_cast<ORRunDecoder*>(fDataDecoder);
}

ORBasicRunDataProcessor::~ORBasicRunDataProcessor()
{
  delete fRunDecoder;
}

ORDataProcessor::EReturnCode ORBasicRunDataProcessor::ProcessMyDataRecord(UInt_t* record)
{
  if (fRunDecoder->IsHeartBeat(record)) {
    return ProcessRunHeartBeat(record);
  } else if (fRunDecoder->IsRunStart(record)) {
    return ProcessRunStart(record);
  } else if (fRunDecoder->IsPrepareForSubRun(record)) {
    return ProcessPrepareForSubRun(record);
  } else if (fRunDecoder->IsSubRunStart(record)) {
    return ProcessSubRunStart(record);
  } else if (fRunDecoder->IsRunStop(record)) {
    return ProcessRunStop(record);
  } else {
    ORLog(kWarning) << "ProcessMyDataRecord(): "
                    << "got run data that wasn't a heartbeat, a (sub) run start, "
		    << "or a (sub) run stop" << endl;
    return kFailure;
  }
}

