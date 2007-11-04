// ORADC2249ADCDecoder.hh

#ifndef _ORADC2249ADCDecoder_hh_
#define _ORADC2249ADCDecoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORADC2249ADCDecoder : public ORVBasicADCDecoder
{
  public:
    ORADC2249ADCDecoder() {}
    virtual ~ORADC2249ADCDecoder() {}

    virtual std::string GetLabel() { return "ADC2249"; }
    virtual std::string GetDataObjectPath() { return "ORADC2249Model:ADC"; }
};

#endif
