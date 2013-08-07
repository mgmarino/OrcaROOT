// ORDataProcManager.cc

#include "ORDataProcManager.hh"

#include "ORLogger.hh"
#include "ORSocketReader.hh"
#include "ORVWriter.hh"
#include <vector>

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
  fRunAsDaemon = false;
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
  
  ORLog(kDebug) << "ProcessDataStream(): calling fReader->Open()..." << std::endl;
  if (!fReader->Open()) return kAlarm; 
  while (1) {
    retCode = ProcessRun();
    if (retCode >= kAlarm) return kAlarm;
    if (retCode >= kBreak || !fReader->OKToRead()) break;
  }
  ORLog(kDebug) << "ProcessDataStream(): calling fReader->Close()..." << std::endl;
  fReader->Close();

  return EndProcessing();
}

ORDataProcManager::EReturnCode ORDataProcManager::ProcessRun()
{
  // always allow all processors to try to process a run
  SetDoProcessRun();


  EReturnCode retCode;
  size_t nLongsMax = 1024;
  std::vector<UInt_t> vecbuffer(nLongsMax);

  Bool_t headerIsReadIn = false;

  ORLog(kDebug) << "ProcessRun(): start reading records..." << std::endl;
  while (fReader->ReadRecord(vecbuffer)) {

    UInt_t* buffer = &vecbuffer[0];
    // Check if it is a header

    if(fHeaderProcessor.ProcessDataRecord(buffer) == kSuccess) {
      // It is a header, perform the setup
      // Set the default flag, header is not read in yet
      headerIsReadIn = false;
      fRunContext->SetMustSwap(fReader->MustSwap());
      
      /* Also check to see if we can write to the reader. */
      if (ORVWriter* theMonitor = dynamic_cast<ORVWriter*>(fReader)) {
        fRunContext->SetWritableSocket(theMonitor);
      } else {
        fRunContext->SetWritableSocket(NULL);
      }
      
      // Load the header dictionary
      ORLog(kDebug) << "ProcessRun(): loading dictionary..." << std::endl;
      if (!fRunContext->LoadHeader(fHeaderProcessor.GetHeader(), fRunAsDaemon)) {

        /* We have encountered a problem loading the header file. */
        /* Kill Run, try going to the next run. */
        ORLog(kError) << "ProcessRun(): Error loading header file.  Stopping run." << std::endl;
        break;
      } 
      
      // Set all the IDs, dictionary
      ORLog(kDebug) << "ProcessRun(): setting dataIDs..." << std::endl;

      SetDataId();

      SetDecoderDictionary();

      headerIsReadIn = true;
      
      // Read the next record
      continue; 
    } // End checking if it is a header
    if (!headerIsReadIn) break;

    fRunContext->ResetRecordFlags();
    if (!fRunAsDaemon) {
      fRunDataProcessor->ProcessDataRecord(buffer);
    }

    if (fRunContext->GetState() <= ORRunContext::kStarting) {
      // Starting a run
      retCode = StartRun();
      if (retCode >= kFailure) KillRun(); // but keep processing: skips to next run
      if (retCode >= kAlarm) return kAlarm;
      if (!fRunAsDaemon) {
        fRunDataProcessor->OnStartRunComplete(); 
      }
    }      
    if (fDoProcessRun) {
      // let all processors process the data record
      if (ProcessDataRecord(buffer) >= kAlarm) return kAlarm;
    }
  
    if (fRunContext->GetState() == ORRunContext::kStopping) {
      break;
    }

    if (TestCancel()) break;
  }
  // Set up Run Context
  ORLog(kDebug) << "ProcessRun(): finished reading records..." << std::endl;
  retCode = EndRun();

  if (retCode >= kAlarm) return kAlarm;
  if (!fRunAsDaemon) {
    fRunDataProcessor->OnEndRunComplete();
  }

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
  if (!fRunAsDaemon) {
    fRunDataProcessor->SetDataId();
  }
  ORCompoundDataProcessor::SetDataId();
}
