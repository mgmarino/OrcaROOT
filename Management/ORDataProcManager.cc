// ORDataProcManager.cc

#include "ORDataProcManager.hh"

#include "ORLogger.hh"
#include "ORSocketReader.hh"
#include "ORVWriter.hh"

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

  if (dynamic_cast<ORSocketReader*>(fReader) != NULL) {
    fRunDataProcessor->IncreaseHeartbeatVerbosity();
  }
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
		  << std::endl;
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


  ORLog(kDebug) << "ProcessRun(): calling fReader->Open()..." << std::endl;
  if(!fReader->Open()) return kBreak;

  EReturnCode retCode;
  size_t nLongsMax = 1024;
  UInt_t* buffer = new UInt_t[nLongsMax];

  bool continueProcessing = true;
  ORLog(kDebug) << "ProcessRun(): reading header..." << std::endl;
  continueProcessing = fReader->ReadRecord(buffer, nLongsMax);
  if(continueProcessing) {
    if(fHeaderProcessor.ProcessDataRecord(buffer) != kSuccess) {
      ORLog(kWarning) << "ProcessRun(): First record was not a header! Can't continue reading run." << std::endl;
      continueProcessing = false;
    }
    fRunContext->SetMustSwap(fReader->MustSwap());

    /* Also check to see if we can write to the reader. */
    if (ORVWriter* theMonitor = dynamic_cast<ORVWriter*>(fReader)) {
      fRunContext->SetWritableSocket(theMonitor);
    } else {
      fRunContext->SetWritableSocket(NULL);
    }
  }
  
  if(continueProcessing) {
    ORLog(kDebug) << "ProcessRun(): loading dictionary..." << std::endl;
    if (!fRunContext->LoadHeader(fHeaderProcessor.GetHeader())) {
      /* We have encountered a problem loading the header file. */
      /* Kill Run, try going to the next run. */
      ORLog(kError) << "ProcessRun(): Error loading header file.  Stopping run." << std::endl;
    } else {
      ORLog(kDebug) << "ProcessRun(): setting dataIDs..." << std::endl;
      SetDataId();
      SetDecoderDictionary();
      
      ORLog(kDebug) << "ProcessRun(): start reading records..." << std::endl;
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
      
        /*
        if (ORProcessStopper::StopNow()) {
          ORLog(kTrace) << "ORProcessStopper stopped data processing..." << std::endl;
          break;
        }*/

        if (TestCancel()) break;
      }
      ORLog(kDebug) << "ProcessRun(): finished reading records..." << std::endl;
      
      retCode = EndRun();
      if (retCode >= kAlarm) return kAlarm;
      fRunDataProcessor->OnEndRunComplete();
    }
  }

  ORLog(kDebug) << "ProcessRun(): calling fReader->Close()..." << std::endl;
  fReader->Close();

  delete [] buffer;
  /*
  if(ORProcessStopper::Stop()) {
    ORLog(kDebug) << "ProcessRun(): ORProcessStopper is stopping the run processing..." << std::endl;
    return kBreak;
  }
  else*/ 
  if (TestCancel()) return kBreak;
  return kSuccess;
}

/*
void ORDataProcManager::Handle(int)
{
  ORLog(kWarning) << "Caught ctrl-c, trying to exit nicely" << std::endl;
  EndRun();
  fRunDataProcessor->OnEndRunComplete();
  EndProcessing();
}*/

void ORDataProcManager::SetRunContext(ORRunContext* aContext)
{
  fRunDataProcessor->SetRunContext(aContext);
  ORCompoundDataProcessor::SetRunContext(aContext); 
}

void ORDataProcManager::SetDataId()
{
  // Not necessary to SetDataId of fHeaderProcessor -- it is always 0x0!
  fRunDataProcessor->SetDataId();
  ORCompoundDataProcessor::SetDataId();
}
