// ORShaperShaperDecoder.hh

#ifndef _ORShaperShaperDecoder_hh_
#define _ORShaperShaperDecoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORShaperShaperDecoder : public ORVBasicADCDecoder
{
  public:
    ORShaperShaperDecoder() {}
    virtual ~ORShaperShaperDecoder() {}

    virtual inline std::string GetLabel() { return "Shaper"; }
    virtual inline std::string GetDataObjectPath() { return "ORShaperModel:Shaper"; }
};

#endif
