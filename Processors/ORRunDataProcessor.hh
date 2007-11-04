// ORRunDataProcessor.hh

#ifndef _ORRunDataProcessor_hh_
#define _ORRunDataProcessor_hh_

#include "ORDataProcessor.hh"
#include "ORRunDecoder.hh"


class ORRunDataProcessor : public ORDataProcessor
{
  public:
    ORRunDataProcessor();
    virtual ~ORRunDataProcessor();

    // overloaded from ORDataProcessor
    virtual EReturnCode ProcessDataRecord(UInt_t* record);
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);

    // to be overloaded, if desired
    virtual inline EReturnCode ProcessRunHeartBeat(UInt_t* /*record*/) { return kSuccess; } 
    virtual inline EReturnCode ProcessRunStart(UInt_t* /*record*/) { return kSuccess; } 
    virtual inline EReturnCode ProcessRunStop(UInt_t* /*record*/) { return kSuccess; }

    // ORDataProcManager calls these; controls state of runcontext
    virtual inline void OnStartRunComplete() { fgRunContext.SetRunning(); }
    virtual inline void OnEndRunComplete() { fgRunContext.SetIdle(); }

  protected:
    ORRunDecoder* fRunDecoder;
    UInt_t fByteCount;
};

#endif
