// ORRunDataProcessor.hh

#ifndef _ORRunDataProcessor_hh_
#define _ORRunDataProcessor_hh_

#include "ORDataProcessor.hh"
#include "ORRunDecoder.hh"
#include "ORLogger.hh"


class ORDataProcManager;
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

    virtual inline void IncreaseHeartbeatVerbosity(bool doIncrease = true)
      { fIncreaseHeartbeatVerbosity = doIncrease; }

    friend class ORDataProcManager;
  protected:
    // ORDataProcManager calls these; controls state of runcontext
    virtual inline void OnStartRunComplete() { if (fRunContext) fRunContext->SetRunning(); }
    virtual inline void OnEndRunComplete() { if (fRunContext) fRunContext->SetIdle(); }
    ORRunDecoder* fRunDecoder;
    UInt_t fByteCount;
    bool fIncreaseHeartbeatVerbosity;
};

#endif
