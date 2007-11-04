// OREventCounter.hh

#ifndef _OREventCounter_hh_
#define _OREventCounter_hh_

#include "ORDataProcessor.hh"
#include "ORVEventCounterDecoder.hh"

class OREventCounter : public ORDataProcessor
{
  public:
    OREventCounter(ORVEventCounterDecoder* eventCounterDecoder);
    virtual ~OREventCounter() {}

    virtual EReturnCode StartRun();
    virtual EReturnCode ProcessMyDataRecord(UInt_t* record);
    virtual EReturnCode EndRun();

    virtual size_t GetEventCount() { return fEventCount; }

  protected:
    ORVEventCounterDecoder* fEventCounterDecoder;
    size_t fEventCount;
};

#endif
