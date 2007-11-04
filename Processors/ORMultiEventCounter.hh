// ORMultiEventCounter.hh

#ifndef _ORMultiEventCounter_hh_
#define _ORMultiEventCounter_hh_

#include "ORVTreeWriter.hh"
#include "ORVEventCounterDecoder.hh"
#include "ORCompoundDataProcessor.hh"

class ORMultiEventCounter : public ORCompoundDataProcessor
{
  public:
    ORMultiEventCounter() {}
    virtual ~ORMultiEventCounter();

    virtual void AddEventCounterDecoder(ORVEventCounterDecoder* eventCounterDecoder);
    virtual void AddProcessor(ORDataProcessor* processor);

    virtual EReturnCode EndRun();
};

#endif
