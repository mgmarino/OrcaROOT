// ORDataProcessor.cc

#include "ORDataProcessor.hh"
#include "ORLogger.hh"
#include "ORRunContext.hh"

using namespace std;

ORDataProcessor::ORDataProcessor(ORVDataDecoder* decoder)
{ 
  fDataId = ORVDataDecoder::GetIllegalDataId(); 
  fDoProcess = true;
  fDoProcessRun = true;
  fDataDecoder = decoder;
  fDebugRecord = false;
  fRunContext = NULL;
}

void ORDataProcessor::SetDataId() 
{ 
  if (fRunContext == NULL) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return;
  }
  const ORHeader* header = fRunContext->GetHeader();
  if (header == NULL) {
    ORLog(kError) << "SetDataId(): SetDataId for path " 
                  << fDataDecoder->GetDataObjectPath() 
                  << ": header was NULL!" << endl;
    return;
  }
  UInt_t tempID = header->GetDataId(fDataDecoder->GetDataObjectPath());
  // check to see if this has been set already.
  if (tempID == ORVDataDecoder::GetIllegalDataId()) {
    ORLog(kWarning) << "SetDataId(): Couldn't get a valid data id for data "
                  << "object path " << fDataDecoder->GetDataObjectPath()
		  << endl;
    if (fDataId != ORVDataDecoder::GetIllegalDataId()) {
      ORLog(kWarning) << "SetDataId(): Data id previously set, trying to use old version."
                      << endl << "  This is likely a problem in ORCA, please update your ORCA version." << endl;
    }
  } else {
    fDataId = tempID;
  }
  ORLog(kDebug) << fDataDecoder->GetDataObjectPath() << " got data ID " << fDataId << endl;
}

void ORDataProcessor::SetDecoderDictionary()
{
  if (fRunContext == NULL) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return;
  }
  const ORHardwareDictionary* hardwareDict = fRunContext->GetHardwareDict();
  if (!hardwareDict) {
    ORLog(kError) << "Hardware dictionary not found!  Some info may not be available" << std::endl; 
    return;
  }
  fDataDecoder->SetDecoderDictionary(
    hardwareDict->GetDecoderDictionary(fDataDecoder->GetDictionaryObjectPath()));
  /* This sets to NULL if it doesn't exist. */
}

ORDataProcessor::EReturnCode ORDataProcessor::ProcessDataRecord(UInt_t* record)
{
  if (!fDoProcess || !fDoProcessRun || !fRunContext) return kFailure;
  else if (fDataDecoder->DataIdOf(record) == fDataId) {
    ORLog(kDebug) << fDataDecoder->GetDataObjectPath() 
                  << " (data id = " << fDataId << "): " << endl;
    if(fRunContext->MustSwap() && !fRunContext->IsRecordSwapped()) {
      /* Swapping the record.  This only must be done once! */
      fDataDecoder->Swap(record);
      fRunContext->SetRecordSwapped();
    }
    if(fDebugRecord) {
      if(ORLogger::GetSeverity() > ORLogger::kDebug) {
        ORLog(kRoutine) << fDataDecoder->GetDataObjectPath() 
                        << " (data id = " << fDataId << "): " << endl;
      }
      fDataDecoder->DumpHex(record);
    }
    return ProcessMyDataRecord(record);
  }
  else return kSuccess;
}
