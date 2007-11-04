// ORVBasicTreeDecoder.hh

#ifndef _ORVBasicTreeDecoder_hh_
#define _ORVBasicTreeDecoder_hh_

#include <string>
#include "ORVDataDecoder.hh"

class ORVBasicTreeDecoder : virtual public ORVDataDecoder
{
  public:
    ORVBasicTreeDecoder() {}
    virtual ~ORVBasicTreeDecoder() {}

    // functions to overload
    virtual size_t GetNPars() = 0;
    virtual std::string GetParName(size_t iPar) = 0;
    virtual inline size_t GetNRows(UInt_t* /*record*/) { return 1; }
    virtual UInt_t GetPar(UInt_t* record, size_t iPar, size_t iRow) = 0;
};

#endif
