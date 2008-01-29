// ORCompoundDataProcessor.cc

#include "ORCompoundDataProcessor.hh"

#include "ORLogger.hh"

using namespace std;

ORCompoundDataProcessor::ORCompoundDataProcessor()
{
  SetComponentBreakReturnsFailure();
}

void ORCompoundDataProcessor::SetDataId()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    fDataProcessors[i]->SetDataId();
  }
}

void ORCompoundDataProcessor::SetDecoderDictionary()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    fDataProcessors[i]->SetDecoderDictionary();
  }
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::StartProcessing()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    EReturnCode retCode = fDataProcessors[i]->StartProcessing();
    if (retCode >= kFailure) fDataProcessors[i]->KillProcessor();
    if (retCode >= kBreak) return fBreakRetCode;
    if (retCode >= kAlarm) return retCode;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::StartRun()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    EReturnCode retCode = fDataProcessors[i]->StartRun();
    if (retCode >= kFailure) fDataProcessors[i]->KillRun();
    if (retCode >= kBreak) return fBreakRetCode;
    if (retCode >= kAlarm) return retCode;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::ProcessDataRecord(UInt_t* record)
{
  if (!fDoProcess || !fDoProcessRun) return kFailure;
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    EReturnCode retCode = fDataProcessors[i]->ProcessDataRecord(record);
    if (retCode == kBreak) return fBreakRetCode;
    if (retCode >= kAlarm) return retCode;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::EndRun()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    EReturnCode retCode = fDataProcessors[i]->EndRun();
    if (retCode >= kBreak) return fBreakRetCode;
    if (retCode >= kAlarm) return retCode;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::EndProcessing()
{
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    EReturnCode retCode = fDataProcessors[i]->EndProcessing();
    if (retCode >= kBreak) return fBreakRetCode;
    if (retCode >= kAlarm) return retCode;
  }
  return kSuccess;
}

ORDataProcessor::EReturnCode ORCompoundDataProcessor::ProcessMyDataRecord(UInt_t* /*record*/)
{
  ORLog(kWarning) << "ProcessMyDataRecord() should never be called..." << endl;
  return kFailure;
}

void ORCompoundDataProcessor::SetDoProcessRun()
{
  fDoProcessRun = true;
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    fDataProcessors[i]->SetDoProcessRun();
  }
}

void ORCompoundDataProcessor::SetRunContext(ORRunContext* aContext)
{
  fRunContext = aContext;
  for (size_t i=0; i<fDataProcessors.size(); i++) {
    fDataProcessors[i]->SetRunContext(fRunContext);
  }
}

void ORCompoundDataProcessor::AddProcessor(ORDataProcessor* processor) 
{ 
  if (processor == NULL) {
    ORLog(kWarning) << "AddProcessor(processor): processor can't be NULL!" << endl;
    return;
  }
  processor->SetRunContext(fRunContext);
  fDataProcessors.push_back(processor); 
}
