// ORRunDataProcessor.cc

#include "ORRunDataProcessor.hh"

#include <string>
#include "TROOT.h"

using namespace std;

ORRunDataProcessor::ORRunDataProcessor() : ORDataProcessor(new ORRunDecoder)
{
  fRunDecoder = dynamic_cast<ORRunDecoder*>(fDataDecoder);
  fByteCount = 0;
  fIncreaseHeartbeatVerbosity = false;
}

ORRunDataProcessor::~ORRunDataProcessor()
{
  if (fRunContext) {
    if(fRunContext->GetState() != ORRunContext::kIdle) {
      ORLog(kError) << "Deconstructing ORRunDataProcessor in non-idle state "
                    << fRunContext->GetState() << ": something has gone wrong" << endl;
    }
  }
  delete fRunDecoder;
}

ORDataProcessor::EReturnCode ORRunDataProcessor::ProcessDataRecord(UInt_t* record)
{
  fByteCount += fRunDecoder->LengthOf(record)*sizeof(UInt_t);
  return ORDataProcessor::ProcessDataRecord(record);
}

ORDataProcessor::EReturnCode ORRunDataProcessor::ProcessMyDataRecord(UInt_t* record)
{
  if (!fRunContext) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return kFailure;
  }
  if (fRunDecoder->IsHeartBeat(record)) {
    string message;
    if(fByteCount < 1024) message = ::Form("%d B", fByteCount);
    else if(fByteCount < 1024*1024) message = ::Form("%.1f kB", float(fByteCount)/1024.0);
    else message = ::Form("%.1f MB", float(fByteCount)/1024.0/1024.0);
    message = ::Form("Heartbeat for run %d: %s have been processed since last heartbeat.", 
                     fRunContext->GetRunNumber(), message.c_str());
    if(fIncreaseHeartbeatVerbosity) ORLog(kRoutine) << message << endl;
    else ORLog(kTrace) << message << endl;
    fByteCount = 0;
    return ProcessRunHeartBeat(record);
  }
  else if (fRunDecoder->IsRunStart(record)) {
    if (fRunContext->GetRunNumber() != fRunDecoder->RunNumberOf(record)) {
      ORLog(kWarning) << "ProcessMyDataRecord(): "
                      << "run number in run-start record doesn't match "
		      << "that loaded from the header. Switching to new "
		      << "run number" << endl;
      fRunContext->LoadRunStartRecord(record);
    }
    fRunContext->SetStarting();
    ORLog(kRoutine) << "Start processing run " << fRunContext->GetRunNumber() << endl;
    return ProcessRunStart(record);
  } else if (fRunDecoder->IsRunStop(record)) {
    fRunContext->SetStopping();
    ORLog(kRoutine) << "Stop processing run " << fRunContext->GetRunNumber() << endl;
    return ProcessRunStop(record);
  } else {
    ORLog(kWarning) << "ProcessMyDataRecord(): "
                    << "got run data that wasn't a heartbeat, a start, "
		    << "or a stop" << endl;
    return kFailure;
  }
}

