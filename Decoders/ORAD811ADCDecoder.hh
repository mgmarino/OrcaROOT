// ORAD811ADCDecoder.hh

#ifndef _ORAD811ADCDecoder_hh_
#define _ORAD811ADCDecoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORAD811ADCDecoder : public ORVBasicADCDecoder
{
  public:
    ORAD811ADCDecoder() {}
    virtual ~ORAD811ADCDecoder() {}

    virtual inline std::string GetLabel() { return "AD811"; }
    virtual std::string GetDataObjectPath() { return "ORAD811Model:ADC"; }
};

#endif
