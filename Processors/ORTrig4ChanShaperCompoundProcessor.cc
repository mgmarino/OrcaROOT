// ORTrig4ChanShaperCompoundProcessor.cc

#include "ORTrig4ChanShaperCompoundProcessor.hh"

ORTrig4ChanShaperCompoundProcessor::ORTrig4ChanShaperCompoundProcessor()
{
  SetComponentBreakReturnsFailure();
  
  fShaperTreeWriter = new ORBasicTreeWriter(&fShaperDecoder, "shaperTree");
  AddProcessor(fShaperTreeWriter);
  
  fTriggerTreeWriter = new ORTrig4ChanTreeWriter("triggerTree");
  AddProcessor(fTriggerTreeWriter);
  
  fLastTriggerRecordPtr = NULL;
}

ORTrig4ChanShaperCompoundProcessor::~ORTrig4ChanShaperCompoundProcessor()
{
  if(fLastTriggerRecordPtr) {
    delete[] fLastTriggerRecordPtr;
  }
  delete fShaperTreeWriter;
  delete fTriggerTreeWriter;
}
ORDataProcessor::EReturnCode ORTrig4ChanShaperCompoundProcessor::StartRun()
{
  fShaperDataId = fShaperTreeWriter->GetDataId();
  fTriggerDataId = fTriggerTreeWriter->GetDataId();
  
  return ORCompoundDataProcessor::StartRun();
}

ORDataProcessor::EReturnCode ORTrig4ChanShaperCompoundProcessor::EndRun()
{

  if(fLastTriggerRecordPtr) {
    delete[] fLastTriggerRecordPtr;
    fLastTriggerRecordPtr = NULL;
  }
  return ORCompoundDataProcessor::EndRun();
}

ORDataProcessor::EReturnCode ORTrig4ChanShaperCompoundProcessor::ProcessDataRecord(UInt_t* record)
{
  /* Call the parent function here, since it takes care of data swapping. */
  ORDataProcessor::EReturnCode code = 
    ORCompoundDataProcessor::ProcessDataRecord(record);
  if ( code != kSuccess ) return code;
  
  UInt_t thisDataId = fShaperDecoder.DataIdOf(record);
  if( thisDataId == fShaperDataId ) {
    if ( fLastRecordDataId == fShaperDataId ) {
      //duplicate trigger record
      /* Here the last record was a shaper, so we have to duplicate the record. */
      /* Call the trigger tree writer to do this. */
      code = fTriggerTreeWriter->ProcessDataRecord(fLastTriggerRecordPtr);
    }  else {
      code = kSuccess;
    } 
    fLastRecordDataId = thisDataId;  
  } else if( thisDataId == fTriggerDataId ) {
    //normal processing
    if (fLastTriggerRecordPtr==NULL) { 
      /* if not allocated, reallocate a buffer and then copy. */
      fTriggerRecordLength = fShaperDecoder.LengthOf(record);
      fLastTriggerRecordPtr = new UInt_t[fTriggerRecordLength];  
    }
    /* copy */
    memcpy(fLastTriggerRecordPtr, record, fTriggerRecordLength*sizeof(UInt_t));
    fLastRecordDataId = thisDataId;  
    code = kSuccess;
  } 
  return code;
}

