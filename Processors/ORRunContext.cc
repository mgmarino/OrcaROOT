// ORRunContext.cc

#include "ORRunContext.hh"
#include "ORRunDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

using namespace std;

ORRunContext::ORRunContext(ORHeader* header, const char* runCtrlPath)
{
  fHeader = NULL;
  fClassName = "";
  fRunNumber = 0;
  fIsQuickStartRun = false;
  fIsRecordSwapped = false;
  fRunType = 0;
  fStartTime = 0;
  fState = kIdle;
  if (header != NULL) LoadHeader(header, runCtrlPath);
}

void ORRunContext::LoadHeader(ORHeader* header, const char* runCtrlPath)
{
  fHeader = header;
  ORDictionary* runCtrlDict = (ORDictionary*) header->LookUp(runCtrlPath);
  if(!runCtrlDict) {
    ORLog(kError) << runCtrlPath << " not found!" << endl;
    return;
  }
  fClassName = ((ORDictValueS*) runCtrlDict->LookUp("Class Name"))->GetS();
  fRunNumber = ((ORDictValueI*) runCtrlDict->LookUp("RunNumber"))->GetI();
  fIsQuickStartRun = ((ORDictValueB*) runCtrlDict->LookUp("quickStart"))->GetB();
  fRunType = ((ORDictValueI*) runCtrlDict->LookUp("runType"))->GetI();
  fStartTime = ((ORDictValueI*) runCtrlDict->LookUp("startTime"))->GetI();
}

void ORRunContext::LoadRunStartRecord(UInt_t* record)
{
  ORRunDecoder runDecoder;
  fRunNumber = runDecoder.RunNumberOf(record);
  fIsQuickStartRun = runDecoder.IsQuickStartRun(record);
  //fRunType = ?? FIXME
  fStartTime = runDecoder.UtimeOf(record);
}

void ORRunContext::SetIdle()
{ 
  if(fState == kRunning) {
    ORLog(kWarning) << "Never got a run-stop packet..." << endl;
  } else if(fState != kStopping) {
    ORLog(kError) << "Switching to idle from non-stopping state " 
                  << fState << endl;
  } 
  fState = kIdle; 
} 

void ORRunContext::SetStarting()
{ 
  if(fState != kIdle) {
    ORLog(kError) << "Starting from non-idle state " << fState << endl;
  } 
  fState = kStarting; 
} 

void ORRunContext::SetRunning() 
{ 
  if(fState == kIdle) {
    ORLog(kWarning) << "Never got a run-start packet..." << endl;
  } else if(fState != kStarting) {
    ORLog(kError) << "Switching to running state from non-starting state " 
                  << fState << endl;
  }
  fState = kRunning; 
}

void ORRunContext::SetStopping()
{ 
  if(fState != kRunning) {
    ORLog(kError) << "Stopping from non-idle state " << fState << endl;
  } 
  fState = kStopping; 
} 

