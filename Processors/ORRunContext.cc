// ORRunContext.cc

#include "ORRunContext.hh"
#include "ORRunDecoder.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"
#include "ORXmlPlistString.hh"

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
  fHardwareDict = NULL;
}

ORRunContext::~ORRunContext()
{
  if (fHardwareDict) delete fHardwareDict;
}

void ORRunContext::LoadHeader(ORHeader* header, const char* runCtrlPath)
{
  if (!header) {
    ORLog(kError) << "Header is NULL!" << std::endl;
    return;
  }
  fHeader = header;
  if (fHardwareDict) delete fHardwareDict;
  fHardwareDict = new ORHardwareDictionary();
  if(!fHardwareDict->LoadHardwareDictFromDict(fHeader->GetDictionary())) {
    ORLog(kError) << "Error loading hardware dictionary!" << std::endl;
    delete fHardwareDict;
    fHardwareDict = NULL;
  }
  ORDictValueA* dataChain = (ORDictValueA*) header->LookUp(runCtrlPath);
  ORDictionary* runCtrlDict = 0;
  for (size_t i=0; i< dataChain->GetNValues();i++) {
    runCtrlDict = (ORDictionary*)((ORDictionary*)dataChain->At(i))->LookUp("Run Control");
    if (runCtrlDict) break;
  }
  if(!runCtrlDict) {
    ORLog(kError) << runCtrlPath << " not found!" << endl;
    return;
  }
  if (ORLogger::GetSeverity() <= ORLogger::kDebug) {
    ORXmlPlistString xmlString;
    xmlString.LoadDictionary(fHardwareDict);
    ORLog(kDebug) << std::endl << xmlString << std::endl;
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

