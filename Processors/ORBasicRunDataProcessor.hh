// ORBasicRunDataProcessor.hh

#ifndef _ORBasicRunDataProcessor_hh_
#define _ORBasicRunDataProcessor_hh_

#include "ORDataProcessor.hh"

class ORRunDecoder;

class ORBasicRunDataProcessor : public ORDataProcessor
{
  public:
    ORBasicRunDataProcessor();
    virtual ~ORBasicRunDataProcessor();

    // overloaded from ORDataProcessor
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

    // to be overloaded, if desired
    virtual inline EReturnCode ProcessRunHeartBeat(UInt_t* /*record*/) { return kSuccess; } 
    virtual inline EReturnCode ProcessRunStart(UInt_t* /*record*/) { return kSuccess; } 
    virtual inline EReturnCode ProcessRunStop(UInt_t* /*record*/) { return kSuccess; }
    virtual inline EReturnCode ProcessPrepareForSubRun(UInt_t* /*record*/) { return kSuccess; }
    virtual inline EReturnCode ProcessSubRunStart(UInt_t* /*record*/) { return kSuccess; } 

  protected:
    ORRunDecoder* fRunDecoder;
};

#endif
