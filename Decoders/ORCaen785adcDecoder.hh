// ORCaen785adcDecoder.hh

#ifndef _ORCaen785adcDecoder_hh_
#define _ORCaen785adcDecoder_hh_

#include "ORCaen792qdcDecoder.hh"

class ORCaen785adcDecoder : public ORCaen792qdcDecoder
{
  public:
    ORCaen785adcDecoder() {}
    virtual ~ORCaen785adcDecoder() {}

    virtual std::string GetDataObjectPath() { return "ORCaen785Model:adc"; }
    virtual std::string GetValueName() { return "adc"; }
};

#endif
