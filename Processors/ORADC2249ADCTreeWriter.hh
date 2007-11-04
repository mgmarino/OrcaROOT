// ORADC2249ADCTreeWriter.hh

#ifndef _ORADC2249ADCTreeWriter_hh_
#define _ORADC2249ADCTreeWriter_hh_

#include "ORBasicRDTreeWriter.hh"
#include "ORADC2249ADCDecoder.hh"

class ORADC2249ADCTreeWriter : public ORBasicRDTreeWriter
{
  public:
    ORADC2249ADCTreeWriter(std::string treeName = "adc2249Tree") : ORBasicRDTreeWriter(new ORADC2249ADCDecoder, treeName)
      { fADCDecoder = dynamic_cast<ORADC2249ADCDecoder*>(fDataDecoder); }
    virtual ~ORADC2249ADCTreeWriter() { delete fADCDecoder; }

  protected:
    ORADC2249ADCDecoder* fADCDecoder;
};

#endif
