// ORDataProcManager.cc

#include "ORDataProcManager.hh"

#include "ORLogger.hh"
#include "ORProcessStopper.hh"

using namespace std;

ORDataProcManager::ORDataProcManager(ORVReader* reader, ORRunDataProcessor* runDataProc)
{ 
  // the optional runDataProc argument allows the user to pass in an
  // overloaded ORRunDataProcessor
  if (runDataProc != NULL) {
    fRunDataProcessor = runDataProc;
    fIOwnRunDataProcessor = false;
  }
  else {
    fRunDataProcessor = new ORRunDataProcessor;
    fIOwnRunDataProcessor = true;
  }
 
  SetReader(reader);
  SetRunContext(new ORRunContext);
  /* Sets fRunContext.  This class owns this object. */
}

ORDataProcManager::~ORDataProcManager()
{
  if(fIOwnRunDataProcessor) delete fRunDataProcessor;
  /* This class owns fRunContext. */
  delete fRunContext;
}

ORDataProcManager::EReturnCode ORDataProcManager::ProcessDataStream()
{
  if (fReader == NULL) {
    ORLog(kError) << "ProcessDataStream(): fReader == NULL: "
                  << "you must set a ORVReader before attempting to process data"
		  << endl;
    return kAlarm;
  }

  EReturnCode retCode = StartProcessing();
  if (retCode >= kFailure) return kAlarm;
  
  while (1) {
    retCode = ProcessRun();
    if (retCode >= kAlarm) return kAlarm;
    if (retCode >= kBreak) break;
  }

  return EndProcessing();
}

ORDataProcManager::EReturnCode ORDataProcManager::ProcessRun()
{
  // always allow all processors to try to process a run
  SetDoProcessRun();


  ORLog(kDebug) << "ProcessRun(): calling fReader->Open()..." << endl;
  if(!fReader->Open()) return kBreak;

  EReturnCode retCode;
  size_t nLongsMax = 1024;
  UInt_t* buffer = new UInt_t[nLongsMax];

  bool continueProcessing = true;
  ORLog(kDebug) << "ProcessRun(): reading header..." << endl;
  continueProcessing = fReader->ReadRecord(buffer, nLongsMax);
  if(continueProcessing) {
    if(fHeaderProcessor.ProcessDataRecord(buffer) != kSuccess) {
      ORLog(kWarning) << "ProcessRun(): First record was not a header! Can't continue reading run." << endl;
      continueProcessing = false;
    }
    fRunContext->SetMustSwap(fReader->MustSwap());
  }
  
  if(continueProcessing) {
    ORLog(kDebug) << "ProcessRun(): loading dictionary..." << endl;
    if (!fRunContext->LoadHeader(fHeaderProcessor.GetHeader())) {
      /* We have encountered a problem loading the header file. */
      /* Kill Run, try going to the next run. */
      ORLog(kError) << "ProcessRun(): Error loading header file.  Stopping run." << endl;
    } else {
      ORLog(kDebug) << "ProcessRun(): setting dataIDs..." << endl;
      SetDataId();
      SetDecoderDictionary();
      
      ORLog(kDebug) << "ProcessRun(): start reading records..." << endl;
      while (fReader->ReadRecord(buffer, nLongsMax) && fDoProcess) {
        fRunContext->ResetRecordFlags();
        fRunDataProcessor->ProcessDataRecord(buffer);
      
        if (fRunContext->GetState() <= ORRunContext::kStarting) {
          retCode = StartRun();
          if (retCode >= kFailure) KillRun(); // but keep processing: skips to next run
          if (retCode >= kAlarm) return kAlarm;
          fRunDataProcessor->OnStartRunComplete(); 
        }
      
        // let all processors process the data record
        if (fDoProcessRun) {
          if (ProcessDataRecord(buffer) >= kAlarm) return kAlarm;
        }
      
        if (fRunContext->GetState() == ORRunContext::kStopping) {
          break;
        }
      
        if (ORProcessStopper::StopNow()) {
          ORLog(kTrace) << "ORProcessStopper stopped data processing..." << endl;
          break;
        }
      }
      ORLog(kDebug) << "ProcessRun(): finished reading records..." << endl;
      
      retCode = EndRun();
      if (retCode >= kAlarm) return kAlarm;
      fRunDataProcessor->OnEndRunComplete();
    }
  }

  ORLog(kDebug) << "ProcessRun(): calling fReader->Close()..." << endl;
  fReader->Close();

  delete [] buffer;

  if(ORProcessStopper::Stop()) {
    ORLog(kDebug) << "ProcessRun(): ORProcessStopper is stopping the run processing..." << endl;
    return kBreak;
  }
  else return kSuccess;
}

void ORDataProcManager::Handle(int)
{
  ORLog(kWarning) << "Caught ctrl-c, trying to exit nicely" << endl;
  EndRun();
  fRunDataProcessor->OnEndRunComplete();
  EndProcessing();
}

void ORDataProcManager::SetDataId()
{
  // Not necessary to SetDataId of fHeaderProcessor -- it is always 0x0!
  fRunDataProcessor->SetDataId();
  ORCompoundDataProcessor::SetDataId();
}
