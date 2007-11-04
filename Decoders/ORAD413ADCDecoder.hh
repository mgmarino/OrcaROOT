// ORAD413ADCDecoder.hh

#ifndef _ORAD413ADCDecoder_hh_
#define _ORAD413ADCDecoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORAD413ADCDecoder : public ORVBasicADCDecoder
{
  public:
    ORAD413ADCDecoder() {}
    virtual ~ORAD413ADCDecoder() {}

    virtual inline UInt_t ChannelOf(UInt_t* record)
      { if (IsLong(record)) record += 1; return (record[0] & 0x00006000) >> 13; }
    virtual inline UInt_t ADCValueOf(UInt_t* record)
      { if (IsLong(record)) record += 1; return (record[0] & 0x00001fff); }

    virtual inline size_t GetNbinsX() { return 0x1fff+1; }
    virtual inline double GetXHi() { return double(0x1fff) + 0.5; }

    virtual inline size_t GetNChannelsMax() { return 0x6+1; }
    virtual inline std::string GetLabel() { return "AD413"; }
    virtual inline std::string GetDataObjectPath() { return "ORAD413AModel:ADC"; }
};

#endif
