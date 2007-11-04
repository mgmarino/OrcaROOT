// ORRunDataProcessor.cc

#include "ORRunDataProcessor.hh"

#include "TROOT.h"
#include "ORLogger.hh"

using namespace std;

ORRunDataProcessor::ORRunDataProcessor() : ORDataProcessor(new ORRunDecoder)
{
  fRunDecoder = dynamic_cast<ORRunDecoder*>(fDataDecoder);
  fByteCount = 0;
}

ORRunDataProcessor::~ORRunDataProcessor()
{
  if(fgRunContext.GetState() != ORRunContext::kIdle) {
    ORLog(kError) << "Deconstructing ORRunDataProcessor in non-idle state "
                  << fgRunContext.GetState() << ": something has gone wrong" << endl;
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
  if (fRunDecoder->IsHeartBeat(record)) {
    if(fByteCount < 1024) {
      ORLog(kRoutine) << "Heartbeat for run " << fgRunContext.GetRunNumber() 
                      << ": " << fByteCount << " B have been processed since "
		      << "last heartbeat." << endl;
    }
    else if(fByteCount < 1024*1024) {
      ORLog(kRoutine) << "Heartbeat for run " << fgRunContext.GetRunNumber() 
                      << ": " << ::Form("%.1f", float(fByteCount)/1024.0) 
		      << " kB have been processed since last heartbeat." << endl;
    }
    else {
      ORLog(kRoutine) << "Heartbeat for run " << fgRunContext.GetRunNumber() 
                      << ": " << ::Form("%.1f", float(fByteCount)/1024.0/1024.0) 
		      << " MB have been processed since last heartbeat." << endl;
    }
    fByteCount = 0;
    return ProcessRunHeartBeat(record);
  }
  else if (fRunDecoder->IsRunStart(record)) {
    if (fgRunContext.GetRunNumber() != fRunDecoder->RunNumberOf(record)) {
      ORLog(kWarning) << "ProcessMyDataRecord(): "
                      << "run number in run-start record doesn't match "
		      << "that loaded from the header. Switching to new "
		      << "run number" << endl;
      fgRunContext.LoadRunStartRecord(record);
    }
    fgRunContext.SetStarting();
    ORLog(kRoutine) << "Start proceessing run " << fgRunContext.GetRunNumber() << endl;
    return ProcessRunStart(record);
  } else if (fRunDecoder->IsRunStop(record)) {
    fgRunContext.SetStopping();
    ORLog(kRoutine) << "Stop proceessing run " << fgRunContext.GetRunNumber() << endl;
    return ProcessRunStop(record);
  } else {
    ORLog(kWarning) << "ProcessMyDataRecord(): "
                    << "got run data that wasn't a heartbeat, a start, "
		    << "or a stop" << endl;
    return kFailure;
  }
}

