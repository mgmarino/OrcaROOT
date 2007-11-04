// ORVBasicRDTreeDecoder.hh

#ifndef _ORVBasicRDTreeDecoder_hh_
#define _ORVBasicRDTreeDecoder_hh_

#include "ORVBasicTreeDecoder.hh"

class ORVBasicRDTreeDecoder : public ORVBasicTreeDecoder
{
  public:
    ORVBasicRDTreeDecoder() {}
    virtual ~ORVBasicRDTreeDecoder() {}

    virtual double ReferenceDateOf(UInt_t* record) = 0;
};

#endif
