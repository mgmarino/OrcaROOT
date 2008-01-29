// ORDataProcessor.cc

#include "ORDataProcessor.hh"
#include "ORLogger.hh"
#include "ORUtils.hh"

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
  ORHeader* header = fRunContext->GetHeader();
  if (header == NULL) {
    ORLog(kError) << "SetDataId(): SetDataId for path " 
                  << fDataDecoder->GetDataObjectPath() 
                  << ": header was NULL!" << endl;
    return;
  }
  fDataId = header->GetDataId(fDataDecoder->GetDataObjectPath());
  if (fDataId == ORVDataDecoder::GetIllegalDataId()) {
    ORLog(kWarning) << "SetDataId(): Couldn't get a valid data id for data "
                  << "object path " << fDataDecoder->GetDataObjectPath()
		  << endl;
  }
}

void ORDataProcessor::SetDecoderDictionary()
{
  if (fRunContext == NULL) {
    ORLog(kError) << "fRunContext is NULL!" << endl;
    return;
  }
  ORHardwareDictionary* hardwareDict = fRunContext->GetHardwareDict();
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
    if(ORUtils::MustSwap() && !fRunContext->IsRecordSwapped()) {
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
