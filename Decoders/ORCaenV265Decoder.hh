// ORAD413ADCDecoder.hh

#ifndef _ORCaenV265Decoder_hh_
#define _ORCaenV265Decoder_hh_

#include "ORVBasicADCDecoder.hh"

class ORCaenV265Decoder : public ORVBasicADCDecoder
{
  public:
    ORCaenV265Decoder() {}
    virtual ~ORCaenV265Decoder() {}

    virtual inline UInt_t ChannelOf(UInt_t* record)
      { return (IsShort(record) ? (record[0] & 0x0000e000) >> 13 : (record[1] & 0x0000e000)); }
    virtual inline UInt_t ADCValueOf(UInt_t* record)
      { return IsShort(record) ? (record[0] & 0x00000fff) : (record[1] & 0x00000fff); }
    virtual inline bool IsTwelveBit(UInt_t* record) 
      { return ! (IsShort(record) ? (record[0] & 0x00001000) >> 12: (record[1] & 0x00001000) >> 12); }

    virtual inline size_t GetNChannelsMax() { return 0x8+1; }
    virtual inline std::string GetLabel() { return "CaenV265"; }
    virtual inline std::string GetDataObjectPath() { return "Caen265"; }
};

#endif
