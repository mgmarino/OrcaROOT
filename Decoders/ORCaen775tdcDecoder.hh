// ORCaen775tdcDecoder.hh

#ifndef _ORCaen775tdcDecoder_hh_
#define _ORCaen775tdcDecoder_hh_

#include "ORCaen792qdcDecoder.hh"

class ORCaen775tdcDecoder : public ORCaen792qdcDecoder
{
  public:
    ORCaen775tdcDecoder() {}
    virtual ~ORCaen775tdcDecoder() {}

    virtual std::string GetDataObjectPath() { return "ORCaen775Model:tdc"; }
    virtual std::string GetValueName() { return "tdc"; }
};

#endif
