// ORTrig4ChanShaperFilter.hh

#ifndef _ORTrig4ChanShaperFilter_hh_
#define _ORTrig4ChanShaperFilter_hh_


#include "ORTrig4ChanDecoder.hh"
#include "ORShaperShaperDecoder.hh"
#include "OR64PDHistDrawer.hh"
#include "ORBasicTreeWriter.hh"
#include "ORCompoundDataProcessor.hh"
#include <utility>
#include <map>

using std::multimap;

class ORTrig4ChanShaperFilter : public ORCompoundDataProcessor
{
  public:
    ORTrig4ChanShaperFilter();
    virtual ~ ORTrig4ChanShaperFilter();
    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();
    virtual EReturnCode EndProcessing();

  protected:
    Double_t fTimeCutLength; 
    
    ORTrig4ChanDecoder fTriggerDecoder;
    ORBasicTreeWriter* fTriggerTreeWriter;
    UInt_t fTriggerDataId; 
    size_t fTriggerRecordLength;
    Double_t fMapTriggerTime;
    Double_t fThisTriggerTime;
    UInt_t* fLastTriggerRecordPtr;
    multimap<Double_t, UInt_t*> fBothRecords;

    
    
    ORShaperShaperDecoder fShaperDecoder;
    ORBasicTreeWriter* fShaperTreeWriter;
    UInt_t fShaperDataId; 
    size_t fShaperRecordLength;
    UInt_t* fTempShaperRecordPtr;
    UInt_t fLastChannel;
    UInt_t fLastCard;
    UInt_t fThisChannel;
    UInt_t fThisCard;
 
    OR64PDHistDrawer* f64PDHistDrawer;
    
    UInt_t fLastRecordDataId;
 
};

#endif
