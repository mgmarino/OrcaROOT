// ORRunContext.cc

#include "ORRunContext.hh"
#include "ORVWriter.hh"
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
  fSubRunNumber = 0;
  fIsQuickStartRun = false;
  fIsRecordSwapped = false;
  fRunType = 0;
  fStartTime = 0;
  fState = kIdle;
  if (header != NULL) LoadHeader(header, runCtrlPath);
  fHardwareDict = NULL;
  fWritableSocket = NULL;
}

ORRunContext::~ORRunContext()
{
  if (fHardwareDict) delete fHardwareDict;
}

bool ORRunContext::LoadHeader(ORHeader* header, bool ignoreRunControl, const char* runCtrlPath)
{

  fClassName = "";
  fRunNumber = -1;
  fRunType = -1;
  fStartTime = -1;
  fIsQuickStartRun = false;
  if (!header) {
    ORLog(kError) << "Header is NULL!" << std::endl;
    return false;
  }
  fHeader = header;
  if (ignoreRunControl) return true;
  if (fHardwareDict) delete fHardwareDict;
  fHardwareDict = new ORHardwareDictionary();
  if(!fHardwareDict->LoadHardwareDictFromDict(fHeader->GetDictionary())) {
    ORLog(kWarning) << "Error loading hardware dictionary!" << std::endl;
    delete fHardwareDict;
    fHardwareDict = NULL;
  }
  ORDictValueA* dataChain = (ORDictValueA*) header->LookUp(runCtrlPath);
  ORDictionary* runCtrlDict = 0;
  if (dataChain) { 
    for (size_t i=0; i< dataChain->GetNValues();i++) {
      runCtrlDict = (ORDictionary*)((ORDictionary*)dataChain->At(i))->LookUp("Run Control");
      if (runCtrlDict) break;
    }
  } else {
    ORLog(kWarning) << "You are using older version Orca file.  Please update Orca as future support will not be guaranteed." << endl; 
    runCtrlDict = (ORDictionary*) header->LookUp("Run Control");
  }
  if(!runCtrlDict) {
    ORLog(kError) << runCtrlPath << " not found!" << endl;
    return false;
  }
  if (ORLogger::GetSeverity() <= ORLogger::kDebug && fHardwareDict) {
    ORXmlPlistString xmlString;
    xmlString.LoadDictionary(fHardwareDict);
    ORLog(kDebug) << std::endl << xmlString << std::endl;
  }
  fClassName = ((ORDictValueS*) runCtrlDict->LookUp("Class Name"))->GetS();
  fRunNumber = ((ORDictValueI*) runCtrlDict->LookUp("RunNumber"))->GetI();
  fIsQuickStartRun = ((ORDictValueB*) runCtrlDict->LookUp("quickStart"))->GetB();
  fRunType = ((ORDictValueI*) runCtrlDict->LookUp("runType"))->GetI();
  fStartTime = ((ORDictValueI*) runCtrlDict->LookUp("startTime"))->GetI();
  return true;
}

std::string ORRunContext::GetStateName(EState aState) const
{
  if (aState >= GetNumberOfStates()) return "";
  switch(aState) {
    case kIdle: return "Idle";
    case kStarting: return "Starting";
    case kRunning: return "Running";
    case kPreparingForSubRun: return "PreparingForSubRun";
    case kStopping: return "Stopping";
    default: return "";
  }
}

void ORRunContext::LoadRunStartRecord(UInt_t* record)
{
  ORRunDecoder runDecoder;
  fRunNumber = runDecoder.RunNumberOf(record);
  fIsQuickStartRun = runDecoder.IsQuickStartRun(record);
  fSubRunNumber = runDecoder.SubRunNumberOf(record);
  //fRunType = ?? FIXME
  fStartTime = runDecoder.UtimeOf(record);
}

void ORRunContext::SetIdle()
{ 
  if(GetState() == kRunning) {
    ORLog(kWarning) << "Never got a run-stop packet..." << endl;
  } else if(GetState() != kStopping) {
    ORLog(kError) << "Switching to idle from non-stopping state " 
                  << GetState() << endl;
  } 
  SetState(kIdle); 
} 

void ORRunContext::SetStarting()
{ 
  if(GetState() != kIdle) {
    ORLog(kError) << "Starting from non-idle state " << GetState() << endl;
  } 
  SetState(kStarting); 
} 

void ORRunContext::SetRunning() 
{ 
  if(GetState() == kIdle) {
    ORLog(kWarning) << "Never got a run-start packet..." << endl;
  } else if(GetState() != kStarting && GetState() != kPreparingForSubRun) {
    ORLog(kError) << "Switching to running state from non-starting state " 
                  << GetState() << endl;
  }
  SetState(kRunning); 
}

void ORRunContext::SetPreparingForSubRun() 
{ 
  if(GetState() != kRunning) {
    ORLog(kWarning) << "Preparing for a sub run from a non-running state " 
                    << GetState() << endl;
  } 
  SetState(kPreparingForSubRun); 
}

void ORRunContext::SetStopping()
{ 
  if(GetState() != kRunning && GetState() != kPreparingForSubRun) {
    ORLog(kError) << "Stopping from an incorrect state " << GetState() << endl;
  } 
  SetState(kStopping); 
} 


Int_t ORRunContext::WriteBackToSocket(const void* buffer, size_t nBytes)
{
  if(!fWritableSocket) return 0;
  return fWritableSocket->WriteBuffer(buffer, nBytes);
}

