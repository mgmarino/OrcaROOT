// ORTrig4ChanShaperCompoundProcessor.hh

#ifndef _ORTrig4ChanShaperCompoundProcessor_hh_
#define _ORTrig4ChanShaperCompoundProcessor_hh_


#include "ORTrig4ChanDecoder.hh"
#include "ORShaperShaperDecoder.hh"
#include "ORBasicTreeWriter.hh"
#include "ORCompoundDataProcessor.hh"


class ORTrig4ChanShaperCompoundProcessor : public ORCompoundDataProcessor
{
  public:
    ORTrig4ChanShaperCompoundProcessor();
    virtual ~ ORTrig4ChanShaperCompoundProcessor();
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();

  protected:
    ORTrig4ChanDecoder fTriggerDecoder;
    ORBasicTreeWriter* fTriggerTreeWriter;
    UInt_t fTriggerDataId; 
    
    ORShaperShaperDecoder fShaperDecoder;
    ORBasicTreeWriter* fShaperTreeWriter;
    UInt_t fShaperDataId; 
    
    UInt_t fLastTriggerRecord;
    UInt_t fLastRecordDataId;
    size_t fTriggerRecordLength;
    UInt_t* fLastTriggerRecordPtr;
};

#endif
