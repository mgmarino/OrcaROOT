// ORVEventCounterDecoder.hh

#ifndef _ORVEventCounterDecoder_hh_
#define _ORVEventCounterDecoder_hh_

#include "ORVDataDecoder.hh"

class ORVEventCounterDecoder : virtual public ORVDataDecoder
{
  public:
    ORVEventCounterDecoder() {}
    virtual ~ORVEventCounterDecoder() {}

    virtual size_t GetEventCount(UInt_t* record) = 0;
};

#endif
