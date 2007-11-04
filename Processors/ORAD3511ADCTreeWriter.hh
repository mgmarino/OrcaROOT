// ORAD3511ADCTreeWriter.hh

#ifndef _ORAD3511ADCTreeWriter_hh_
#define _ORAD3511ADCTreeWriter_hh_

#include "ORBasicRDTreeWriter.hh"
#include "ORAD3511ADCDecoder.hh"

class ORAD3511ADCTreeWriter : public ORBasicRDTreeWriter
{
  public:
    ORAD3511ADCTreeWriter(std::string treeName = "adc2249Tree") : ORBasicRDTreeWriter(new ORAD3511ADCDecoder, treeName)
      { fADCDecoder = dynamic_cast<ORAD3511ADCDecoder*>(fDataDecoder); }
    virtual ~ORAD3511ADCTreeWriter() { delete fADCDecoder; }

  protected:
    ORAD3511ADCDecoder* fADCDecoder;
};

#endif
